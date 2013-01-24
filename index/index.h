/** Copyright (C) 2013 wilburlang@gmail.com
 */
#ifndef IDX_INDEX_H
#define IDX_INDEX_H
#include "common/sql/sqlite++.h"
#include <vector>

namespace idx {

    class indexer {
    public:
        indexer();
        bool index(const char* db, const char* path);
    private:
        void create_table(cxx::sqlite3_connection* db);
        void indexing_doc(cxx::sqlite3_connection* db, int id, const char* file);
        void update_index(cxx::sqlite3_connection* db, int id, const char* key, int rank);
        int  get_doc_id  (cxx::sqlite3_connection* db, const char* file);
    };

}

#endif
