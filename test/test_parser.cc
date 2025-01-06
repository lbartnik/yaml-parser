#include "parser.h"
#include <gtest/gtest.h>

namespace parser {

TEST(ParserTest, test) {
    ObjectParser p;
    std::list<Token> tokens({text("a"), colon(), text("b")});

    p.parse(tokens);
}

} // namespace parser