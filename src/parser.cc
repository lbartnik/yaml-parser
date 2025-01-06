#include "parser.h"

namespace parser {

std::optional<Node> NodeParser::parse(ListTokenizer& tokenizer)
{
    auto maybeToken = tokenizer.peek();

    if (!maybeToken) {
        return std::nullopt;
    }

    if (maybeToken->type == TokenType::TEXT) {
        ObjectParser p;
        auto maybeObject = p.parse(tokenizer);
        if (maybeObject) {
            return maybeObject;
        }
    }

    if (maybeToken->type == TokenType::NUMBER || maybeToken->type == TokenType::TEXT) {
        tokenizer.next();
        return Node{.value = maybeToken->value};
    }

    throw std::runtime_error("Unknown node type");
}

std::optional<Node> ObjectParser::parse(ListTokenizer& tokenizer) {
    std::vector<std::tuple<std::string, Node*>> elements;

    auto maybeToken = tokenizer.peek();

    if (!maybeToken) {
        return std::nullopt;
    }

    do {
        // key
        if (maybeToken->type != TokenType::TEXT) {
            throw std::runtime_error("Expected key");
        }

        const auto& key = maybeToken->value;

        tokenizer.next();

        // value
        NodeParser np;
        auto maybeValue = np.parse(tokenizer);
        if (maybeValue) {
            elements.push_back(std::make_tuple(key, new Node(*maybeValue)));
        }

        // TODO now expect either:
        // (1) a continuation of the current object: a new line, indentation and key: value pair
        // (2) the end of this object, which means exiting this parser
    } while (true);
}

} // namespace parser