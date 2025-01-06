#pragma once

#include <filesystem>
#include <fstream>
#include <list>
#include <optional>
#include <string>
#include <string_view>

namespace parser {

enum class TokenType { END, TEXT, NUMBER, COLON, DASH, COMMENT, INDENT, NEW_LINE };

struct Token {
    TokenType type;
    uint32_t start;
    std::string value;
};

inline Token indent(std::string value) {
    return Token{.type = TokenType::INDENT, .value = std::move(value)};
}

inline Token text(std::string value) {
    return Token{.type = TokenType::TEXT, .value = std::move(value)};
}

inline Token colon() {
    return Token{.type = TokenType::COLON, .value = ":"};
}

inline Token newLine() {
    return Token{.type = TokenType::NEW_LINE, .value = "\n"};
}


struct ListTokenizer {

    ListTokenizer(std::list<Token> tokens) : mTokens(std::move(tokens)), mCurrentToken(mTokens.begin()) {}

    std::optional<const Token&> next() {
        if (mCurrentToken == mTokens.end()) {
            return std::nullopt;
        }
        return *mCurrentToken++;
    }

    std::optional<const Token&> peek() {
        if (mCurrentToken == mTokens.end()) {
            return std::nullopt;
        }
        return *mCurrentToken;
    }

    std::list<Token> mTokens;
    std::list<Token>::iterator mCurrentToken;
};





class StreamTokenizer {
  public:
    StreamTokenizer(const std::filesystem::path &path) : mInput(path) {}

    Token next() {
        // consume the next token
        while (mInput) {
            int c = mInput.get();

            switch (c) {
            case '-':
                return Token{.type = TokenType::DASH, .start = mIndex - 1};
            case ':':
                return Token{.type = TokenType::COLON, .start = mIndex - 1};
            }

            if (std::isspace(c)) {
                mInput.unget();
                return nextSpace();
            }

            if (std::isdigit(c)) {
                mInput.unget();
                return nextNumber();
            }

            if (std::isalnum(c)) {
                mInput.unget();
                return nextText();
            }
        }

        return Token{.type = TokenType::END, .start = mIndex};
    }

  private:
    Token nextSpace() {
        Token token{.type = TokenType::INDENT, .start = mIndex};
        do {
            int c = mInput.get();
            if (!std::isspace(c)) {
                mInput.unget();
                return token;
            }
            token.value += c;
        } while (true);
    }

    Token nextNumber() {
        Token token{.type = TokenType::NUMBER, .start = mIndex};
        int c;

        while (std::isdigit(c = mInput.get())) {
            token.value += c;
        }

        if (c != '.') {
            mInput.unget();
            return token;
        }

        token.value += c;

        while (std::isdigit(c = mInput.get())) {
            token.value += c;
        }

        if (token.value.back() == '.') {
            throw std::runtime_error("Malformed number");
        }

        mInput.unget();
        return token;
    }

    Token nextText() {
        Token token{.type = TokenType::TEXT, .start = mIndex};
        int c;

        while (!std::isspace(c = mInput.get())) {
            token.value += c;
        }

        mInput.unget();
        return token;
    }

    std::ifstream mInput;
    uint32_t mIndex{0};
};

} // namespace parser