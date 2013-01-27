/** Copyright (C) 2013 wilburlang@gmail.com
 */
#include <string.h>
#include <algorithm>
#include "query.h"
#include "ngram/bigram.h"
#include "ngram/idxdbi.h"

namespace idx {

    class find_result {
    public:
        typedef idxdbi::info            result;
        typedef std::vector<result >    result_set;

        void add_doc(const result_set& result);
        void and_doc(const result_set& result);
        result_set get_doc() const;
    private:
        result_set  result_;
    };

    query::query(const char *db)
        : db_(NULL)
    {
        db_ = idxdbi::create(db, true);
    }

    query::~query()
    {
        delete db_;
    }

    record query::find(const char *keyword)
    {
        idx::document doc;
        idx::document::item_list item = doc.parse(keyword, keyword + strlen(keyword));

        find_result result;
        record      rec;

        for(size_t i = 0; i < item.size(); ++i) {
            find_result::result_set tmp;
            if(db_->get_index(item[i].word.c_str(), tmp)) {
                rec.add_key(item[i].word, item[i].rank);
                if(i == 0) {
                    result.add_doc(tmp);
                }
                else {
                    result.and_doc(tmp);
                }
            }
        }

        const find_result::result_set& files = result.get_doc();
        for(size_t i = 0; i < files.size(); ++i) {
            std::string name = db_->id_to_doc(files[i].file);
            if(!name.empty()) {
                rec.add_doc(name.c_str(), files[i].rank);
            }
        }
        return rec;
    }


    ////////////////////////////////////////////////////////////////////////////
    static bool less_info_name(const find_result::result& lhs,
                               const find_result::result& rhs)
    {
        return lhs.file < rhs.file;
    }

    static bool less_info_rank(const find_result::result& lhs,
                               const find_result::result& rhs)
    {
        return lhs.rank > rhs.rank;
    }

    static bool equal_info_name(const find_result::result& lhs,
                                const find_result::result& rhs)
    {
        return lhs.file == rhs.file;
    }

    void find_result::add_doc(const result_set &result)
    {
        for(size_t i = 0; i < result.size(); ++i) {
            std::pair<result_set::iterator, result_set::iterator > ret = std::equal_range(result_.begin(), result_.end(), result[i], less_info_name);
            if(ret.first != result_.end()) {
                (*ret.first).rank += result[i].rank;
            }
            else {
                result_.push_back(result[i]);
                std::sort(result_.begin(), result_.end(), less_info_name);
            }
        }
    }

    void find_result::and_doc(const result_set &result)
    {
        result_set found;
        for(size_t i = 0; i < result.size(); ++i) {
            std::pair<result_set::iterator, result_set::iterator > ret = std::equal_range(result_.begin(), result_.end(), result[i], less_info_name);
            if(ret.first != result_.end()) {
                (*ret.first).rank += result[i].rank;
                found.push_back(*ret.first);
            }
        }
        std::sort(found.begin(), found.end(), less_info_name);
        found.swap(result_);
    }

    find_result::result_set find_result::get_doc() const
    {
        result_set result = result_;
        std::sort(result.begin(), result.end(), less_info_rank);
        return result;
    }

}
