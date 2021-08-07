#include <catch2/catch.hpp>

#include "nucleus/parser/tokenizer.h"

namespace nu {

TEST_CASE("Tokenizer") {
  SECTION("Detect end of source") {
    Tokenizer t("text");

    // We don't care about the first token.
    auto unknown = t.consume_next_token();
    CHECK(unknown.type != TokenType::EndOfSource);

    // The next token should be the end of the source.
    auto token = t.consume_next_token();
    CHECK(token.type == TokenType::EndOfSource);
  }

  SECTION("Token is whitespace") {
    Tokenizer t(" \n\r\t..");

    auto token = t.peek_next_token();
    CHECK(token.type == TokenType::Whitespace);
    CHECK(token.text.length() == 4);
  }

  SECTION("Tokenizer can skip whitespace while getting next token") {
    Tokenizer t("  test  blah\n\r");

    SECTION("use consume and peek methods to skip whitespace") {
      auto token_1 = t.consume_next_token(Tokenizer::SkipWhitespace);
      CHECK(token_1.type == TokenType::Text);
      CHECK(StringView("test").compare(token_1.text) == 0);

      auto token_2 = t.peek_next_token(Tokenizer::SkipWhitespace);
      CHECK(token_2.type == TokenType::Text);
      CHECK(StringView("blah").compare(token_2.text) == 0);
    }

    SECTION("read whitespace after skipping whitespace") {
      auto token_1 = t.consume_next_token(Tokenizer::SkipWhitespace);
      CHECK(token_1.type == TokenType::Text);
      CHECK(StringView("test").compare(token_1.text) == 0);

      t.consume_next_token();
      auto token_2 = t.peek_next_token();
      CHECK(token_2.type == TokenType::Whitespace);
    }
  }

  SECTION("Token is punctuation") {
    Tokenizer t("`~!@#$%^&*()-_=+[{]};:'\"\\|,<.>/?)");

    CHECK(t.consume_next_token().type == TokenType::Acute);
    CHECK(t.consume_next_token().type == TokenType::Tilde);
    CHECK(t.consume_next_token().type == TokenType::ExclamationMark);
    CHECK(t.consume_next_token().type == TokenType::At);
    CHECK(t.consume_next_token().type == TokenType::Hash);
    CHECK(t.consume_next_token().type == TokenType::DollarSign);
    CHECK(t.consume_next_token().type == TokenType::Percentage);
    CHECK(t.consume_next_token().type == TokenType::Caret);
    CHECK(t.consume_next_token().type == TokenType::Ampersand);
    CHECK(t.consume_next_token().type == TokenType::Asterisk);
    CHECK(t.consume_next_token().type == TokenType::OpenParenthesis);
    CHECK(t.consume_next_token().type == TokenType::ClosedParenthesis);
    CHECK(t.consume_next_token().type == TokenType::Minus);
    CHECK(t.consume_next_token().type == TokenType::Underscore);
    CHECK(t.consume_next_token().type == TokenType::Equals);
    CHECK(t.consume_next_token().type == TokenType::Plus);
    CHECK(t.consume_next_token().type == TokenType::OpenBracket);
    CHECK(t.consume_next_token().type == TokenType::OpenBrace);
    CHECK(t.consume_next_token().type == TokenType::ClosedBracket);
    CHECK(t.consume_next_token().type == TokenType::ClosedBrace);
    CHECK(t.consume_next_token().type == TokenType::SemiColon);
    CHECK(t.consume_next_token().type == TokenType::Colon);
    CHECK(t.consume_next_token().type == TokenType::Apostrophe);
    CHECK(t.consume_next_token().type == TokenType::Quote);
    CHECK(t.consume_next_token().type == TokenType::Backslash);
    CHECK(t.consume_next_token().type == TokenType::Pipe);
    CHECK(t.consume_next_token().type == TokenType::Comma);
    CHECK(t.consume_next_token().type == TokenType::LessThan);
    CHECK(t.consume_next_token().type == TokenType::Period);
    CHECK(t.consume_next_token().type == TokenType::GreaterThan);
    CHECK(t.consume_next_token().type == TokenType::ForwardSlash);
    CHECK(t.consume_next_token().type == TokenType::QuestionMark);
  }

  SECTION("Token is a number") {
    Tokenizer t{"- 145 -123 678.54 -123.2133"};

    auto other = t.consume_next_token();
    CHECK(other.type != TokenType::Number);

    t.consume_next_token();  // Skip whitespace

    auto positive = t.consume_next_token();
    CHECK(positive.type == TokenType::Number);
    CHECK(positive.text.length() == 3);

    t.consume_next_token();  // Skip whitespace

    auto negative = t.consume_next_token();
    CHECK(negative.type == TokenType::Number);
    CHECK(negative.text.length() == 4);

    t.consume_next_token();  // Skip whitespace

    auto positive_float = t.consume_next_token();
    CHECK(positive_float.type == TokenType::Number);
    CHECK(positive_float.text.length() == 6);

    t.consume_next_token();  // Skip whitespace

    auto negative_float = t.consume_next_token();
    CHECK(negative_float.type == TokenType::Number);
    CHECK(negative_float.text.length() == 9);
  }

  SECTION("Token is text") {
    auto tokenizer = Tokenizer("12 abc abc123_!##");

    auto other = tokenizer.consume_next_token();
    CHECK(other.type != TokenType::Text);

    tokenizer.consume_next_token();  // Skip whitespace

    auto text = tokenizer.consume_next_token();
    CHECK(text.type == TokenType::Text);
    CHECK(text.text.length() == 3);

    tokenizer.consume_next_token();  // Skip whitespace

    auto text_with_numbers_and_punctuation = tokenizer.consume_next_token();
    CHECK(text_with_numbers_and_punctuation.type == TokenType::Text);
    CHECK(text_with_numbers_and_punctuation.text.length() == 10);
  }

  SECTION("consume until end of line") {
    struct {
        const char* source;
        const char* expected;
    } tests[] = {
        {"line 1\nline 2", "line 1"},
        {"line 1\n\nline 3", "line 1"},
        {"line 1\r\nline 2", "line 1"},
        {"line 1\n\rline 2", "line 1"},
        {"line 1\n\n\rline 2", "line 1"},
        {"line 1\r\r\nline 2", "line 1"},
    };

    for (auto& test : tests) {
        Tokenizer t{test.source};
        auto token = t.consume_until_eol();
        CHECK(token.text == test.expected);
    }
  }
}

}  // namespace nu
