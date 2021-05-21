#include <catch2/catch.hpp>

#include "nucleus/Parser/tokenizer.h"

namespace nu {

TEST_CASE("Detect end of source") {
  Tokenizer t = Tokenizer("asdf");

  // We don't care about the first token.
  auto unknown = t.consume_next_token();
  CHECK(unknown.type != TokenType::EndOfSource);

  // The next token should be the end of the source.
  auto token = t.consume_next_token();
  CHECK(token.type == TokenType::EndOfSource);
}

TEST_CASE("Token is whitespace") {
  auto t = Tokenizer(" \n\r\t..");

  auto token = t.peek_next_token();
  CHECK(token.type == TokenType::Whitespace);
  CHECK(token.text.length() == 4);
}

TEST_CASE("Tokenizer can skip whitespace while getting next token") {
  auto tokenizer = Tokenizer("  test  blah\n\r");

  SECTION("use consume and peek methods to skip whitespace") {
    auto token1 = tokenizer.consume_next_token(Tokenizer::SkipWhitespace);
    CHECK(token1.type == TokenType::Text);
    CHECK(StringView("test").compare(token1.text) == 0);

    auto token2 = tokenizer.peek_next_token(Tokenizer::SkipWhitespace);
    CHECK(token2.type == TokenType::Text);
    CHECK(StringView("blah").compare(token2.text) == 0);
  }

  SECTION("read whitespace after skipping whitespace") {
    auto token1 = tokenizer.consume_next_token(Tokenizer::SkipWhitespace);
    CHECK(token1.type == TokenType::Text);
    CHECK(StringView("test").compare(token1.text) == 0);

    tokenizer.consume_next_token();
    auto token2 = tokenizer.peek_next_token();
    CHECK(token2.type == TokenType::Whitespace);
  }
}

TEST_CASE("Token is punctuation") {
  auto tokenizer = Tokenizer("`~!@#$%^&*()-_=+[{]};:'\"\\|,<.>/?)");

  CHECK(tokenizer.consume_next_token().type == TokenType::Acute);
  CHECK(tokenizer.consume_next_token().type == TokenType::Tilde);
  CHECK(tokenizer.consume_next_token().type == TokenType::ExclamationMark);
  CHECK(tokenizer.consume_next_token().type == TokenType::At);
  CHECK(tokenizer.consume_next_token().type == TokenType::Hash);
  CHECK(tokenizer.consume_next_token().type == TokenType::DollarSign);
  CHECK(tokenizer.consume_next_token().type == TokenType::Percentage);
  CHECK(tokenizer.consume_next_token().type == TokenType::Caret);
  CHECK(tokenizer.consume_next_token().type == TokenType::Ampersand);
  CHECK(tokenizer.consume_next_token().type == TokenType::Asterisk);
  CHECK(tokenizer.consume_next_token().type == TokenType::OpenParenthesis);
  CHECK(tokenizer.consume_next_token().type == TokenType::ClosedParenthesis);
  CHECK(tokenizer.consume_next_token().type == TokenType::Minus);
  CHECK(tokenizer.consume_next_token().type == TokenType::Underscore);
  CHECK(tokenizer.consume_next_token().type == TokenType::Equals);
  CHECK(tokenizer.consume_next_token().type == TokenType::Plus);
  CHECK(tokenizer.consume_next_token().type == TokenType::OpenBracket);
  CHECK(tokenizer.consume_next_token().type == TokenType::OpenBrace);
  CHECK(tokenizer.consume_next_token().type == TokenType::ClosedBracket);
  CHECK(tokenizer.consume_next_token().type == TokenType::ClosedBrace);
  CHECK(tokenizer.consume_next_token().type == TokenType::SemiColon);
  CHECK(tokenizer.consume_next_token().type == TokenType::Colon);
  CHECK(tokenizer.consume_next_token().type == TokenType::Apostrophe);
  CHECK(tokenizer.consume_next_token().type == TokenType::Quote);
  CHECK(tokenizer.consume_next_token().type == TokenType::Backslack);
  CHECK(tokenizer.consume_next_token().type == TokenType::Pipe);
  CHECK(tokenizer.consume_next_token().type == TokenType::Comma);
  CHECK(tokenizer.consume_next_token().type == TokenType::LessThan);
  CHECK(tokenizer.consume_next_token().type == TokenType::Period);
  CHECK(tokenizer.consume_next_token().type == TokenType::GreaterThan);
  CHECK(tokenizer.consume_next_token().type == TokenType::ForwardSlash);
  CHECK(tokenizer.consume_next_token().type == TokenType::QuestionMark);
}

TEST_CASE("Token is a number") {
  auto tokenizer = Tokenizer{"- 145 -123 678.54 -123.2133"};

  auto other = tokenizer.consume_next_token();
  CHECK(other.type != TokenType::Number);

  tokenizer.consume_next_token();  // Skip whitespace

  auto positive = tokenizer.consume_next_token();
  CHECK(positive.type == TokenType::Number);
  CHECK(positive.text.length() == 3);

  tokenizer.consume_next_token();  // Skip whitespace

  auto negative = tokenizer.consume_next_token();
  CHECK(negative.type == TokenType::Number);
  CHECK(negative.text.length() == 4);

  tokenizer.consume_next_token();  // Skip whitespace

  auto positiveFloat = tokenizer.consume_next_token();
  CHECK(positiveFloat.type == TokenType::Number);
  CHECK(positiveFloat.text.length() == 6);

  tokenizer.consume_next_token();  // Skip whitespace

  auto negativeFloat = tokenizer.consume_next_token();
  CHECK(negativeFloat.type == TokenType::Number);
  CHECK(negativeFloat.text.length() == 9);
}

TEST_CASE("Token is text") {
  auto tokenizer = Tokenizer("12 abc abc123_!##");

  auto other = tokenizer.consume_next_token();
  CHECK(other.type != TokenType::Text);

  tokenizer.consume_next_token();  // Skip whitespace

  auto text = tokenizer.consume_next_token();
  CHECK(text.type == TokenType::Text);
  CHECK(text.text.length() == 3);

  tokenizer.consume_next_token();  // Skip whitespace

  auto textWithNumbersAndPunctuation = tokenizer.consume_next_token();
  CHECK(textWithNumbersAndPunctuation.type == TokenType::Text);
  CHECK(textWithNumbersAndPunctuation.text.length() == 10);
}

}  // namespace nu
