/** Copyright (C) 2013 wilburlang@gmail.com
 */
#ifndef IDX_IDXDBI_H
#define IDX_IDXDBI_H
#include <string>
#include <vector>

namespace idx {

    class idxdbi {
    public:
        struct info {
            int     file;
            int     rank;
            info(int f, int r) : file(f), rank(r) {}
        };

        typedef std::vector<info >   info_seq;

        static idxdbi* create(const char* dbdir, bool readonly);
        virtual ~idxdbi() {}

        virtual int         doc_to_id(const char* file) = 0;
        virtual std::string id_to_doc(int id) const = 0;
        virtual void        put_index(const char* key, const info& val) = 0;
        virtual bool        get_index(const char* key, info_seq& infos) const = 0;
    };

}

#endif
