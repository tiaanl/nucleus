
#include "nucleus/Parser/Tokenizer.h"
#include "nucleus/Testing.h"

namespace nu {

TEST_CASE("Detect end of source") {
  Tokenizer t = Tokenizer("asdf");

  // We don't care about the first token.
  auto unknown = t.consumeNextToken();
  CHECK(unknown.type != TokenType::EndOfSource);

  // The next token should be the end of the source.
  auto token = t.consumeNextToken();
  CHECK(token.type == TokenType::EndOfSource);
}

TEST_CASE("Token is whitespace") {
  auto t = Tokenizer(" \n\r\t..");

  auto token = t.peekNextToken();
  CHECK(token.type == TokenType::Whitespace);
  CHECK(token.text.getLength() == 4);
}

TEST_CASE("Token is punctuation") {
  auto tokenizer = Tokenizer("`~!@#$%^&*()-_=+[{]};:'\"\\|,<.>/?)");

  CHECK(tokenizer.consumeNextToken().type == TokenType::Acute);
  CHECK(tokenizer.consumeNextToken().type == TokenType::Tilde);
  CHECK(tokenizer.consumeNextToken().type == TokenType::ExclamationMark);
  CHECK(tokenizer.consumeNextToken().type == TokenType::At);
  CHECK(tokenizer.consumeNextToken().type == TokenType::Hash);
  CHECK(tokenizer.consumeNextToken().type == TokenType::DollarSign);
  CHECK(tokenizer.consumeNextToken().type == TokenType::Percentage);
  CHECK(tokenizer.consumeNextToken().type == TokenType::Caret);
  CHECK(tokenizer.consumeNextToken().type == TokenType::Ampersand);
  CHECK(tokenizer.consumeNextToken().type == TokenType::Asterisk);
  CHECK(tokenizer.consumeNextToken().type == TokenType::OpenParenthesis);
  CHECK(tokenizer.consumeNextToken().type == TokenType::ClosedParenthesis);
  CHECK(tokenizer.consumeNextToken().type == TokenType::Minus);
  CHECK(tokenizer.consumeNextToken().type == TokenType::Underscore);
  CHECK(tokenizer.consumeNextToken().type == TokenType::Equals);
  CHECK(tokenizer.consumeNextToken().type == TokenType::Plus);
  CHECK(tokenizer.consumeNextToken().type == TokenType::OpenBracket);
  CHECK(tokenizer.consumeNextToken().type == TokenType::OpenBrace);
  CHECK(tokenizer.consumeNextToken().type == TokenType::ClosedBracket);
  CHECK(tokenizer.consumeNextToken().type == TokenType::ClosedBrace);
  CHECK(tokenizer.consumeNextToken().type == TokenType::SemiColon);
  CHECK(tokenizer.consumeNextToken().type == TokenType::Colon);
  CHECK(tokenizer.consumeNextToken().type == TokenType::Apostrophe);
  CHECK(tokenizer.consumeNextToken().type == TokenType::Quote);
  CHECK(tokenizer.consumeNextToken().type == TokenType::Backslack);
  CHECK(tokenizer.consumeNextToken().type == TokenType::Pipe);
  CHECK(tokenizer.consumeNextToken().type == TokenType::Comma);
  CHECK(tokenizer.consumeNextToken().type == TokenType::LessThan);
  CHECK(tokenizer.consumeNextToken().type == TokenType::Period);
  CHECK(tokenizer.consumeNextToken().type == TokenType::GreaterThan);
  CHECK(tokenizer.consumeNextToken().type == TokenType::ForwardSlash);
  CHECK(tokenizer.consumeNextToken().type == TokenType::QuestionMark);
}

TEST_CASE("Token is a number") {
  auto tokenizer = Tokenizer("- 145 -123 678.54 -123.2133");

  auto other = tokenizer.consumeNextToken();
  CHECK(other.type != TokenType::Number);

  tokenizer.consumeNextToken();  // Skip whitespace

  auto positive = tokenizer.consumeNextToken();
  CHECK(positive.type == TokenType::Number);
  CHECK(positive.text.getLength() == 3);

  tokenizer.consumeNextToken();  // Skip whitespace

  auto negative = tokenizer.consumeNextToken();
  CHECK(negative.type == TokenType::Number);
  CHECK(negative.text.getLength() == 4);

  tokenizer.consumeNextToken();  // Skip whitespace

  auto positiveFloat = tokenizer.consumeNextToken();
  CHECK(positiveFloat.type == TokenType::Number);
  CHECK(positiveFloat.text.getLength() == 6);

  tokenizer.consumeNextToken();  // Skip whitespace

  auto negativeFloat = tokenizer.consumeNextToken();
  CHECK(negativeFloat.type == TokenType::Number);
  CHECK(negativeFloat.text.getLength() == 9);
}

TEST_CASE("Token is text") {
  auto tokenizer = Tokenizer("12 abc abc123_!##");

  auto other = tokenizer.consumeNextToken();
  CHECK(other.type != TokenType::Text);

  tokenizer.consumeNextToken();  // Skip whitespace

  auto text = tokenizer.consumeNextToken();
  CHECK(text.type == TokenType::Text);
  CHECK(text.text.getLength() == 3);

  tokenizer.consumeNextToken();  // Skip whitespace

  auto textWithNumbersAndPunctuation = tokenizer.consumeNextToken();
  CHECK(textWithNumbersAndPunctuation.type == TokenType::Text);
  CHECK(textWithNumbersAndPunctuation.text.getLength() == 10);
}

}  // namespace nu
