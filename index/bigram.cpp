/** Copyright (C) 2013 wilburlang@gmail.com
 */
#include "bigram.h"
#include <ctype.h>
#include <cassert>
#include <set>

namespace idx {

    //------------------------------------------------------------------------//
    struct Tab {
        int     cmask;
        int     cval;
        int     shift;
        long    lmask;
        long    lval;
    };

    static const Tab tab[] = {
        0x80,	0x00,	0*6,	0x7F,		0,          /* 1 byte sequence */
        0xE0,	0xC0,	1*6,	0x7FF,		0x80,		/* 2 byte sequence */
        0xF0,	0xE0,	2*6,	0xFFFF,		0x800,		/* 3 byte sequence */
        0xF8,	0xF0,	3*6,	0x1FFFFF,	0x10000,	/* 4 byte sequence */
        0xFC,	0xF8,	4*6,	0x3FFFFFF,	0x200000,	/* 5 byte sequence */
        0xFE,	0xFC,	5*6,	0x7FFFFFFF,	0x4000000,	/* 6 byte sequence */
        0,                                              /* end of table */
    };

    int bigram::utf8_len(const char *s, size_t n)
    {
        if(s == NULL || n == 0)
            return 0;

        int nc = 0;
        if(n <= nc) {
            assert(false);
            return -1;
        }

        int c0 = *s & 0xFF;
        for(const Tab* t = tab; t->cmask; t++) {
            nc++;
            if((c0 & t->cmask) == t->cval) {
                return nc;
            }
            if(n <= nc) {
                assert(false);
                return -1;
            }
            s++;
            if(((*s ^ 0x80) & 0xFF) & 0xC0) {
                assert(false);
                return -1;
            }
        }
        assert(false);
        return -1;
    }

    static const char* stop_words[] = {
        "，", "。", "、", "？", "~", "！", "￥", "……",
        "（", "）", "——", "·", "「", "：", "；", "《",
        "》", " ", ",", "\\", "/", "~", "`", "!", "@",
        "#", "$", "%", "^", "&", "*", "(", ")", "-",
        "+", "=", "{", "}", "[", "]", ":", ";", "\"",
        "\'", "<", ">", ",", ".", "?"
    };

    static int is_stop_words(const char* begin, const char* end)
    {
        static std::set<std::string >* stop_words_set = NULL;
        if(!stop_words_set) {
            stop_words_set = new std::set<std::string >();
            for(size_t i = 0; i < sizeof(stop_words)/sizeof(stop_words[0]); ++i) {
                stop_words_set->insert(stop_words[i]);
            }
        }

        int         len = bigram::utf8_len(begin, end - begin);
        std::string tmp(begin, begin + len);
        if(stop_words_set->count(tmp)) {
            return len;
        }
        return 0;
    }

    //-------------------------------------------------------------------------/
    static int get_line_ascii(const char* begin, const char* end)
    {
        const char* ptr = begin;
        while(ptr < end && isascii(*ptr) && !isdigit(*ptr) && !isspace(*ptr)) {
            ptr++;
        }
        return ptr - begin;
    }

    static int get_line_digit(const char* begin, const char* end)
    {
        const char* ptr = begin;
        while(ptr < end && isdigit(*ptr)) {
            ptr++;
        }
        return ptr - begin;
    }

    static int get_line_space(const char* begin, const char* end)
    {
        const char* ptr = begin;
        while(ptr < end && isspace(*ptr)) {
            ptr++;
        }
        return ptr - begin;
    }

    static int get_line_cjk(const char* begin, const char* end)
    {
        const char* ptr = begin;
        while(ptr < end) {
            int len = bigram::utf8_len(ptr, end - ptr);
            if(len <= 1 || is_stop_words(ptr, end) > 0) {
                break;
            }
            ptr += len;
        }
//        if(ptr == begin) {
//            return 1;
//        }
        return ptr - begin;
    }

    static int get_line_sym(const char* begin, const char* end)
    {
        const char* ptr = begin;
        while(ptr < end) {
            int len = is_stop_words(ptr, end);
            if(len > 0)
                ptr += len;
            else
                break;
        }
        return ptr - begin;
    }

    int get_line(filter::category cate, const char* begin, const char* end)
    {
        int len = 0;
        switch(cate) {
        case filter::DIGIT:
            len = get_line_digit(begin, end);
            break;
        case filter::SPACE:
            len = get_line_space(begin, end);
            break;
        case filter::ASCII:
            len = get_line_ascii(begin, end);
            break;
        case filter::CJK:
            len = get_line_cjk(begin, end);
            break;
        case filter::SYM:
            len = get_line_sym(begin, end);
            break;
        }
        return len;
    }

    ////////////////////////////////////////////////////////////////////////////
    filter::filter(const char *begin, const char *end)
        : begin_(begin), end_(end), ptr_(begin)
    {
    }

    filter::category filter::peek()
    {
        if(isalpha(*ptr_)) {
            return ASCII;
        }
        if(isdigit(*ptr_)) {
            return DIGIT;
        }
        if(isspace(*ptr_)) {
            return SPACE;
        }
        if(is_stop_words(ptr_, end_)) {
            return SYM;
        }
        return CJK;
    }

    filter::block filter::get_block()
    {
        block    ret = { ASCII, ptr_, end_ };
        if(ptr_ < end_) {
            category cat = peek();
            ret.type = cat;
            int      len = get_line(cat, ptr_, end_);
            ptr_ += len;
            ret.end = ptr_;
        }
        return ret;
    }

    bigram::bigram(const char *begin, const char *end)
        : begin_(begin), end_(end), ptr_(begin)
    {
    }

    bigram::word bigram::get_word()
    {
        filter  lang(ptr_, end_);
        filter::block block = lang.get_block();

        word    retw = { end_, end_ };
        word    cjkw;
        while(block.begin < end_) {
            switch(block.type) {
            case filter::SPACE:
            case filter::SYM:
                ptr_ = block.end;
                break;
            case filter::ASCII:
            case filter::DIGIT:
                retw.begin = block.begin;
                retw.end   = block.end;
                ptr_ = block.end;
                return retw;
            case filter::CJK:
                cjkw = make_cjk(block.begin, block.end);
                if(cjkw.begin != cjkw.end) {
                    return cjkw;
                }
                assert(ptr_ == cjkw.end);
                break;
            default:
                assert(false);
                ptr_ = block.end;
                break;
            }
            block = lang.get_block();
        }
        return retw;
    }

    bigram::word bigram::make_cjk(const char *begin, const char *end)
    {
        word        result = { end, end };
        const char* start  = ptr_;
        if(ptr_ < end) {
            int len = utf8_len(ptr_, end - ptr_);
            assert(len > 1);
            ptr_ += len;
            if(ptr_ < end) {
                int len2 = utf8_len(ptr_, end - ptr_);
                assert(len2 > 1);
                result.begin= start;
                result.end  = ptr_ + len2;
            }
        }
        return result;
    }

    document::document(const char *file)
        : file_(file)
    {
    }

    document::item_list document::parse()
    {
    }



}
