/** Copyright (C) 2013 wilburlang@gmail.com
 */
#include "common/config.h"
#include "common/gtest/gtest.h"
#include "bigram.h"
#include "index.h"
#include <iomanip>

TEST(bigram, utf8len)
{
    ASSERT_EQ(1, idx::bigram::utf8_len("a", 1));
    ASSERT_EQ(1, idx::bigram::utf8_len("abc", 1));
    ASSERT_EQ(3, idx::bigram::utf8_len("我们", 6));
}

TEST(bigram, filter)
{
    {
        const char* t = "中文english现在";
        const char* e[] = { "中文", "english", "现在" };

        int         i = 0;
        idx::filter f(t, t + strlen(t));
        idx::filter::block b = f.get_block();
        while(b.begin < t + strlen(t)) {
            std::string temp(b.begin, b.end);
            ASSERT_STREQ(temp.c_str(), e[i]);
            b = f.get_block();
            i++;
        }
        ASSERT_EQ(i, 3);
    }
    {
        const char* t = "中文eng12lish现在";
        const char* e[] = { "中文", "eng", "12", "lish", "现在" };

        int         i = 0;
        idx::filter f(t, t + strlen(t));
        idx::filter::block b = f.get_block();
        while(b.begin < t + strlen(t)) {
            std::string temp(b.begin, b.end);
            ASSERT_STREQ(temp.c_str(), e[i]);
            b = f.get_block();
            i++;
        }
        ASSERT_EQ(i, 5);
    }
    {
        const char* t = "中文eng12lish现3在";
        const char* e[] = { "中文", "eng", "12", "lish", "现", "3", "在" };

        int         i = 0;
        idx::filter f(t, t + strlen(t));
        idx::filter::block b = f.get_block();
        while(b.begin < t + strlen(t)) {
            std::string temp(b.begin, b.end);
            ASSERT_STREQ(temp.c_str(), e[i]);
            b = f.get_block();
            i++;
        }
        ASSERT_EQ(i, 7);
    }
    {
        const char* t = "中文eng12lish现3在4";
        const char* e[] = { "中文", "eng", "12", "lish", "现", "3", "在", "4" };

        int         i = 0;
        idx::filter f(t, t + strlen(t));
        idx::filter::block b = f.get_block();
        while(b.begin < t + strlen(t)) {
            std::string temp(b.begin, b.end);
            ASSERT_STREQ(temp.c_str(), e[i]);
            b = f.get_block();
            i++;
        }
        ASSERT_EQ(i, 8);
    }
    {
        const char* t = "中文eng12lish现 在4";
        const char* e[] = { "中文", "eng", "12", "lish", "现", " ", "在", "4" };

        int         i = 0;
        idx::filter f(t, t + strlen(t));
        idx::filter::block b = f.get_block();
        while(b.begin < t + strlen(t)) {
            std::string temp(b.begin, b.end);
            ASSERT_STREQ(temp.c_str(), e[i]);
            b = f.get_block();
            i++;
        }
        ASSERT_EQ(i, 8);
    }
    {
        const char* t = "中文english is very fun 现 在";
        const char* e[] = { "中文", "english", " ", "is", " ", "very", " ", "fun", " ", "现", " ", "在" };

        int         i = 0;
        idx::filter f(t, t + strlen(t));
        idx::filter::block b = f.get_block();
        while(b.begin < t + strlen(t)) {
            std::string temp(b.begin, b.end);
            ASSERT_STREQ(temp.c_str(), e[i]);
            b = f.get_block();
            i++;
        }
        ASSERT_EQ(i, 12);
    }
}

TEST(bigram, segment)
{
    {
        int         i = 0;
        const char* t = "我们";
        const char* e[] = { "我们" };
        idx::bigram b(t, t + strlen(t));
        idx::bigram::word w = b.get_word();
        while(w.begin < t + strlen(t)) {
            std::string temp(w.begin, w.end);
            ASSERT_STREQ(temp.c_str(), e[i++]);
            w = b.get_word();
        }
        ASSERT_EQ(i, 1);
    }
    {
        int         i = 0;
        const char* t = "我们现在";
        const char* e[] = { "我们", "们现", "现在" };
        idx::bigram b(t, t + strlen(t));
        idx::bigram::word w = b.get_word();
        while(w.begin < t + strlen(t)) {
            std::string temp(w.begin, w.end);
            ASSERT_STREQ(temp.c_str(), e[i++]);
            w = b.get_word();
        }
        ASSERT_EQ(i, 3);
    }
    {
        int         i = 0;
        const char* t = "中文english现在";
        const char* e[] = { "中文", "english", "现在" };
        idx::bigram b(t, t + strlen(t));
        idx::bigram::word w = b.get_word();
        while(w.begin < t + strlen(t)) {
            std::string temp(w.begin, w.end);
            ASSERT_STREQ(temp.c_str(), e[i++]);
            w = b.get_word();
        }
        ASSERT_EQ(i, 3);
    }
    {
        int         i = 0;
        const char* t = "中文现english在";
        const char* e[] = { "中文", "文现", "english" };
        idx::bigram b(t, t + strlen(t));
        idx::bigram::word w = b.get_word();
        while(w.begin < t + strlen(t)) {
            std::string temp(w.begin, w.end);
            ASSERT_STREQ(temp.c_str(), e[i++]);
            w = b.get_word();
        }
        ASSERT_EQ(i, 3);
    }
    {
        int         i = 0;
        const char* t = "。谢谢。";
        const char* e[] = { "谢谢" };
        idx::bigram b(t, t + strlen(t));
        idx::bigram::word w = b.get_word();
        while(w.begin < t + strlen(t)) {
            std::string temp(w.begin, w.end);
            ASSERT_STREQ(temp.c_str(), e[i++]);

            w = b.get_word();
        }
        ASSERT_EQ(i, 1);
    }
    {
        int         i = 0;
        const char* t = "’丁春秋";
        const char* e[] = { "丁春", "春秋" };
        idx::bigram b(t, t + strlen(t));
        idx::bigram::word w = b.get_word();
        while(w.begin < t + strlen(t)) {
            std::string temp(w.begin, w.end);
            ASSERT_STREQ(temp.c_str(), e[i++]);

            w = b.get_word();
        }
        ASSERT_EQ(i, 2);
    }
    {
        {
            int         i = 0;
            const char* t = "“丁春秋";
            const char* e[] = { "丁春", "春秋" };
            idx::bigram b(t, t + strlen(t));
            idx::bigram::word w = b.get_word();
            while(w.begin < t + strlen(t)) {
                std::string temp(w.begin, w.end);
                ASSERT_STREQ(temp.c_str(), e[i++]);

                w = b.get_word();
            }
            ASSERT_EQ(i, 2);
        }
    }
    {
        int         i = 0;
        const char* t = "请尝试解析用户面彩信业务，初步需要字段如下所示。数据可用你手头已有的数据，验证数据正在协调中最近有望拿到。谢谢。";
        idx::bigram b(t, t + strlen(t));
        idx::bigram::word w = b.get_word();
        while(w.begin < t + strlen(t)) {
            std::string temp(w.begin, w.end);
            std::cout << "5: " << temp << "," << temp.size() << "\n";

            w = b.get_word();
        }
    }
    {
        int         i = 0;
        const char* t = "/小时";
        idx::bigram b(t, t + strlen(t));
        idx::bigram::word w = b.get_word();
        while(w.begin < t + strlen(t)) {
            std::string temp(w.begin, w.end);
            std::cout << "7: " << temp << "," << temp.size() << "\n";

            w = b.get_word();
        }
    }
    {
        int         i = 0;
        const char* t = "现在时速12KM/小时，你可以开到24公里/小时";
        idx::bigram b(t, t + strlen(t));
        idx::bigram::word w = b.get_word();
        while(w.begin < t + strlen(t)) {
            std::string temp(w.begin, w.end);
            std::cout << "6: " << temp << "," << temp.size() << "\n";

            w = b.get_word();
        }
    }
}

TEST(document, buffer)
{
    {
        idx::document doc;

        const char* t = "I love English.\nDo you I?\n";
        idx::document::item_list list = doc.parse(t, t + strlen(t));
        ASSERT_EQ(list.size(), 5);
        for(size_t i = 0; i < list.size(); ++i) {
            std::cout << "1: " << list[i].rank << "," << list[i].word << "\n";
        }
    }
    {
        idx::document doc;

        const char* t = "中文文法分析，二元分析结合四则运算";
        idx::document::item_list list = doc.parse(t, t + strlen(t));
        ASSERT_EQ(list.size(), 13);
        for(size_t i = 0; i < list.size(); ++i) {
            std::cout << "2: " << list[i].rank << "," << list[i].word << "\n";
        }
    }
    {
        idx::document doc;

        const char* t = "中文和english文法,分析，who care about:anything? 二元分析结合四则运算";
        idx::document::item_list list = doc.parse(t, t + strlen(t));
        ASSERT_EQ(list.size(), 17);
        for(size_t i = 0; i < list.size(); ++i) {
            std::cout << "3: " << list[i].rank << "," << list[i].word << "\n";
        }
    }
}

TEST(document, file)
{
    idx::document doc("doc/test.txt");
    idx::document::item_list list = doc.parse();
    for(size_t i = 0; i < list.size(); ++i) {
        std::cout << std::setw(6) << i << ","
                  << std::setw(6) <<  list[i].rank << ","
                  << std::setw(12) << list[i].word << "\n";
    }
}

TEST(indexer, file)
{
    idx::indexer index("db/test.kct");
    index.index("doc");
}

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
