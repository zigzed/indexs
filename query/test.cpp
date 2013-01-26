/** Copyright (C) 2013 wilburlang@gmail.com
 */
#include "common/config.h"
#include "common/gtest/gtest.h"
#include "query.h"

TEST(query, usage)
{
    idx::query  search("db");
    idx::record result = search.find("大家 好处");
    idx::record::brief b = result.matched();
    while(!b.file.empty()) {
        std::cout << "file: " << b.file << "\n"
                  << "    : " << b.line << "\n"
                  << "    : " << b.key << "\n\n";

        b = result.matched();
    }
}

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
