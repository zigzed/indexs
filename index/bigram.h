/** Copyright (C) 2013 wilburlang@gmail.com
 */
#ifndef IDX_BIGRAM_H
#define IDX_BIGRAM_H
#include <list>
#include <string>
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
            uint32_t    rank;
        };
        typedef std::list<item >    item_list;

        document(const char* file);
        document(const char* buffer, const char* end);
        item_list parse();
    private:
        std::string file_;
    };

}

#endif
