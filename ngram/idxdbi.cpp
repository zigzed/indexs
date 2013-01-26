/** Copyright (C) 2013 wilburlang@gmail.com
 */
#include "kcdb.h"
#include "kcdbext.h"
#include "idxdbi.h"

namespace idx {

    class kc_idxdb : public idxdbi {
    public:
        kc_idxdb(const char* dbdir, bool readonly);
        ~kc_idxdb();

        int         doc_to_id(const char* file);
        std::string id_to_doc(int id) const;
        void        put_index(const char *key, const info &val);
        bool        get_index(const char *key, info_seq &infos) const;
    private:
        void        get_error(kyotocabinet::IndexDB* db) const;

        kyotocabinet::IndexDB*  idb_;
        kyotocabinet::IndexDB*  ddb_;
    };

    idxdbi* idxdbi::create(const char *dbdir, bool readonly)
    {
        return new kc_idxdb(dbdir, readonly);
    }

    ////////////////////////////////////////////////////////////////////////////
    kc_idxdb::kc_idxdb(const char *dbdir, bool readonly)
        : idb_(NULL), ddb_(NULL)
    {
        std::string doc_db(dbdir);
        std::string idx_db(dbdir);
        doc_db += "/d.kct";
        idx_db += "/i.kct";

        uint32_t mode = kyotocabinet::BasicDB::OWRITER | kyotocabinet::BasicDB::OCREATE;
        if(readonly) {
            mode = kyotocabinet::BasicDB::OREADER;
        }
        ddb_ = new kyotocabinet::IndexDB();
        if(!ddb_->open(doc_db.c_str(), mode)) {
            get_error(ddb_);
        }
        idb_ = new kyotocabinet::IndexDB();
        if(!idb_->open(idx_db.c_str(), mode)) {
            get_error(idb_);
        }
    }

    kc_idxdb::~kc_idxdb()
    {
        if(idb_) {
            idb_->close();
            delete idb_;
        }
        if(ddb_) {
            ddb_->close();
            delete ddb_;
        }
    }

    int kc_idxdb::doc_to_id(const char *file)
    {
        int doc_id = -1;
        int max_id = 0;

        kyotocabinet::BasicDB::Cursor* ptr = ddb_->cursor();
        ptr->jump();
        std::string key, val;
        while(ptr->get(&key, &val, true)) {
            assert(key.size() == sizeof(int));
            int tmp = *(int* )key.data();
            if(tmp > max_id) {
                max_id = tmp;
            }

            if(val == file) {
                doc_id = tmp;
                break;
            }
        }
        delete ptr;

        if(doc_id == -1) {
            doc_id = max_id + 1;

            if(!ddb_->set((char* )&doc_id, sizeof(doc_id), file, strlen(file))) {
                get_error(ddb_);
            }
        }
        return doc_id;
    }

    std::string kc_idxdb::id_to_doc(int id) const
    {
        std::string key, val;
        key.insert(key.end(), (char* )&id, (char* )(&id + 1));
        if(!ddb_->get(key, &val)) {
            get_error(ddb_);
        }
        return val;
    }

    void kc_idxdb::put_index(const char *key, const info &val)
    {
        if(!idb_->append(key, strlen(key), (char* )&val, sizeof(val))) {
            get_error(idb_);
        }
    }

    bool kc_idxdb::get_index(const char *key, info_seq &infos) const
    {
        std::string val;
        if(!idb_->get(key, &val)) {
            get_error(idb_);
            return false;
        }
        assert(val.size() % sizeof(info) == 0);

        int step = sizeof(info);
        for(size_t i = 0; i < val.size(); i += step) {
            info o = *(info* )((char* )val.data() + i);
            infos.push_back(o);
        }
        return true;
    }

    void kc_idxdb::get_error(kyotocabinet::IndexDB *db) const
    {
        kyotocabinet::BasicDB::Error e = db->error();
        printf("index error: %s, %s, %s\n",
               e.codename(e.code()), e.name(), e.message());
    }

}
