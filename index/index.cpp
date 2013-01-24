/** Copyright (C) 2013 wilburlang@gmail.com
 */
#include "index.h"
#include <stdio.h>
#include <iostream>
#include <sstream>
#include "common/sys/filesystem.h"
#include "bigram.h"

namespace idx {

    indexer::indexer(const char *db)
        : idb_(NULL)
    {
        idb_ = new kyotocabinet::IndexDB();
        if(!idb_->open(db)) {
            kyotocabinet::BasicDB::Error e = idb_->error();
            printf("open(%s) error: %s, %s, %s\n",
                   db,
                   e.codename(e.code()), e.name(), e.message());
        }
    }

    indexer::~indexer()
    {
        idb_->close();
        delete idb_;
    }

    bool indexer::index(const char *path)
    {
        cxx::sys::DirIterator   dir(path);
        while(dir != cxx::sys::DirIterator()) {
            cxx::sys::Path pathname(path);
            pathname.append(dir.name().c_str());
            int id = get_files_id(pathname.name());
            indexing_doc(id, pathname.name());
            ++dir;
        }
    }

    void indexer::indexing_doc(int id, const char *file)
    {
        idx::document doc(file);
        idx::document::item_list list = doc.parse();
        printf("parsing %s done\n", file);

        for(size_t i = 0; i < list.size(); ++i) {
            update_index(id, list[i].word.c_str(), list[i].rank);
        }
    }


    int indexer::get_files_id(const char *file)
    {
        static int maxid = 0;
        int id = ++maxid;

        std::ostringstream str;
        str << "doc.id:" << id;
        std::string key = str.str();
        idb_->set(key.c_str(), key.size(), file, strlen(file));

        return id;
    }

    void indexer::update_index(int id, const char *key, int rank)
    {
        std::string kbuf = "idx.key:";
        kbuf += key;
        std::string kval;
        kval.insert(kval.end(), (char* )&id, (char* )&id + sizeof(id));
        kval.insert(kval.end(), (char* )&rank, (char* )&rank + sizeof(rank));

        if(!idb_->append(kbuf, kval)) {
            kyotocabinet::BasicDB::Error e = idb_->error();
            printf("update_index(%d,%s,%d) error: %s, %s, %s\n",
                   id, key, rank,
                   e.codename(e.code()), e.name(), e.message());
        }
    }

}
