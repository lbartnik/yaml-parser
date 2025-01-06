#pragma once

#include "ast.h"
#include "tokenizer.h"

#include <list>
#include <optional>

namespace parser {

struct NodeParser {
    std::optional<Node> parse(ListTokenizer& tokenizer);
};

struct ObjectParser {
    std::optional<Node> parse(ListTokenizer& tokenizer);
};

} // namespace parser