#include <filesystem>
#include <fstream>
#include <string>
#include <string_view>

namespace parser {

enum class TokenType { END, WORD, NUMBER, COLON, DASH, COMMENT, SPACE };

struct Token {
    TokenType type;
    uint32_t start;
    std::string value;
};

class Tokenizer {
  public:
    Tokenizer(const std::filesystem::path &path) : mInput(path) {}

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
                return nextWord();
            }
        }

        return Token{.type = TokenType::END, .start = mIndex};
    }

  private:
    Token nextSpace() {
        Token token{.type = TokenType::SPACE, .start = mIndex};
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

    Token nextWord() {
        Token token{.type = TokenType::WORD, .start = mIndex};
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