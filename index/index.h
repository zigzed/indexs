/** Copyright (C) 2013 wilburlang@gmail.com
 */
#ifndef IDX_INDEX_H
#define IDX_INDEX_H
#include <vector>

namespace idx {

    class idxdbi;

    class indexer {
    public:
        indexer(const char* db);
        ~indexer();
        bool index(const char* path);
    private:
        void indexing_doc(int id, const char* file);
        void update_index(int id, const char* key, int rank);

        idxdbi* db_;
    };

}

#endif
