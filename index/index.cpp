/** Copyright (C) 2013 wilburlang@gmail.com
 */
#include "index/index.h"
#include <stdio.h>
#include <iostream>
#include <sstream>
#include "common/sys/filesystem.h"
#include "ngram/bigram.h"
#include "ngram/idxdbi.h"

namespace idx {

    indexer::indexer(const char *db)
    {
        db_ = idxdbi::create(db, false);
    }

    indexer::~indexer()
    {
        delete db_;
    }

    bool indexer::index(const char *path)
    {
        cxx::sys::DirIterator   dir(path);
        while(dir != cxx::sys::DirIterator()) {
            cxx::sys::Path pathname(path);
            pathname.append(dir.name().c_str());
            int id = db_->doc_to_id(pathname.name());
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

    void indexer::update_index(int id, const char *key, int rank)
    {
        db_->put_index(key, idxdbi::info(id, rank));
    }

}
