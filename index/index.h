/** Copyright (C) 2013 wilburlang@gmail.com
 */
#ifndef IDX_INDEX_H
#define IDX_INDEX_H
#include <vector>
#include "kcdbext.h"
#include "kcdb.h"

namespace idx {

    class indexer {
    public:
        indexer(const char* db);
        ~indexer();
        bool index(const char* path);
    private:
        int  get_files_id(const char* file);
        void indexing_doc(int id, const char* file);
        void update_index(int id, const char* key, int rank);

        kyotocabinet::IndexDB*    idb_;
    };

}

#endif
