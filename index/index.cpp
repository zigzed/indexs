/** Copyright (C) 2013 wilburlang@gmail.com
 */
#include "index.h"
#include <stdio.h>
#include <iostream>
#include "common/sys/filesystem.h"
#include "bigram.h"

namespace idx {

    indexer::indexer()
    {
    }

    bool indexer::index(const char *db, const char *path)
    {
        ::remove(db);

        try {
            cxx::sqlite3_connection database(db);
            create_table(&database);

            cxx::sys::DirIterator   dir(path);
            while(dir != cxx::sys::DirIterator()) {
                cxx::sys::Path pathname(path);
                pathname.append(dir.name().c_str());
                int id = get_doc_id(&database, pathname.name());
                indexing_doc(&database, id, pathname.name());
                ++dir;
            }
        }
        catch(const cxx::sqlite3_error& e) {
            std::cout << "sqlite error: " << e.what() << "\n";
        }
    }

    void indexer::indexing_doc(cxx::sqlite3_connection *db, int id, const char *file)
    {
        idx::document doc(file);
        idx::document::item_list list = doc.parse();
        for(size_t i = 0; i < list.size(); ++i) {
            update_index(db, id, list[i].word.c_str(), list[i].rank);
            printf(".");
        }
    }

    void indexer::create_table(cxx::sqlite3_connection *db)
    {
        db->executedml("create table doc ("
                       "id int, "
                       "path text)");
        db->executedml("create table iii ("
                       "key text, "
                       "ref blob)");
    }

    int indexer::get_doc_id(cxx::sqlite3_connection *db, const char *file)
    {
        {
            cxx::sqlite3_command    select(*db, "select id from doc where path = ?");
            select.bind(1, file);
            cxx::sqlite3_reader     reader = select.execute((cxx::sqlite3_reader*)NULL);
            while(!reader.iseof()) {
                int id = reader.get(0, (int* )NULL);
                return id;
            }
        }
        {
            int                     fileid = 0;
            cxx::sqlite3_command    select(*db, "select max(id) from doc");
            cxx::sqlite3_reader     reader = select.execute((cxx::sqlite3_reader*)NULL);
            while(!reader.iseof()) {
                fileid = reader.get(0, (int* )NULL) + 1;
            }

            if(fileid == 0) {
                fileid = 1;
            }

            cxx::sqlite3_command    insert(*db, "insert into doc values (?, ?)");
            insert.bind(1, fileid);
            insert.bind(2, file);
            insert.execute();
        }
    }

    void indexer::update_index(cxx::sqlite3_connection *db, int id, const char *key, int rank)
    {
        std::string ref;
        {
            cxx::sqlite3_command    select(*db, "select ref from iii where key = ?");
            select.bind(1, key);
            cxx::sqlite3_reader     reader = select.execute((cxx::sqlite3_reader*)NULL);
            while(!reader.iseof()) {
                ref = reader.get_blob(0);
            }
        }
        ref.insert(ref.end(), (char* )&id, (char* )&id + sizeof(int));
        ref.insert(ref.end(), (char* )&rank, (char* )&rank + sizeof(int));
        {
            if(ref.size() > 8) {
                cxx::sqlite3_command    update(*db, "update iii set ref = ? where key = ?");
                update.bind(1, ref);
                update.bind(2, key);
                update.execute();
            }
            else {
                cxx::sqlite3_command    insert(*db, "insert into iii values (?, ?)");
                insert.bind(1, key);
                insert.bind(2, ref.data(), ref.size());
                insert.execute();
            }
        }
    }

}
