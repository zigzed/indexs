/** Copyright (C) 2013 wilburlang@gmail.com
 */
#ifndef IDX_QUERY_H
#define IDX_QUERY_H
#include <string>

namespace idx {

    class idxdbi;

    class record {
    public:
        struct brief {
            std::string file;   /// matched file name
            std::string line;   /// matched line
            std::string key;    /// matched keyword
        };

        record(const char* keyword) {}
        void    add_doc(const char* file, int rank) {}
        brief   matched() { return brief(); }
    private:
    };

    class query {
    public:
        query(const char* db);
        ~query();
        record find(const char* keyword);
    private:
        void load_doc();
        idxdbi* db_;
    };

}

#endif
