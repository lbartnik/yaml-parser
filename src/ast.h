#pragma once

#include "tokenizer.h"

#include <optional>
#include <tuple>
#include <vector>

namespace parser {

struct Node {
    
    ~Node();

    std::string value;
    std::vector<std::tuple<std::string, Node*>> elements;
};


} // namespace parser