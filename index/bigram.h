/** Copyright (C) 2013 wilburlang@gmail.com
 */
#ifndef IDX_BIGRAM_H
#define IDX_BIGRAM_H
#include <vector>
#include <string>
#include <map>
#include "common/config.h"

namespace idx {

    class filter {
    public:
        enum category { ASCII, CJK, SPACE, DIGIT, SYM };
        struct block {
            category    type;
            const char* begin;
            const char* end;
        };
        filter(const char* begin, const char* end);
        block get_block();
    private:
        category peek();
        const char* begin_;
        const char* end_;
        const char* ptr_;
    };

    class bigram {
    public:
        struct word {
            const char* begin;
            const char* end;
        };

        bigram(const char* begin, const char* end);
        word        get_word();
        static int  utf8_len(const char* s, size_t n);
    private:
        word        make_cjk(const char* begin, const char* end);

        const char* begin_;
        const char* end_;
        const char* ptr_;
    };

    class document {
    public:
        struct item {
            std::string word;
            std::size_t rank;
            bool operator< (const item& rhs) const;
        };
        typedef std::vector<item >    item_list;

        document() {}
        document(const char* file);
        item_list parse();
        item_list parse(const char* begin, const char* end) const;
    private:
        typedef std::map<std::string, size_t >  WordRank;

        void parse(WordRank& words, const char* begin, const char* end) const;

        std::string file_;
        WordRank    rank_;
    };

}

#endif
