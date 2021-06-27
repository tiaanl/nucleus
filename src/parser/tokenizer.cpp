
#include "nucleus/Parser/tokenizer.h"

#include "nucleus/optional.h"

namespace nu {

namespace {

bool is_whitespace(Char ch) {
  return (ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t');
}

bool is_number(Char ch) {
  return ch >= '0' && ch <= '9';
}

struct PunctuationInfo {
  Char ch;
  TokenType type;
} PUNCTUATION_MAP[] = {
    {'`', TokenType::Acute},
    {'~', TokenType::Tilde},
    {'!', TokenType::ExclamationMark},
    {'@', TokenType::At},
    {'#', TokenType::Hash},
    {'$', TokenType::DollarSign},
    {'%', TokenType::Percentage},
    {'^', TokenType::Caret},
    {'&', TokenType::Ampersand},
    {'*', TokenType::Asterisk},
    {'(', TokenType::OpenParenthesis},
    {')', TokenType::ClosedParenthesis},
    {'-', TokenType::Minus},
    {'_', TokenType::Underscore},
    {'=', TokenType::Equals},
    {'+', TokenType::Plus},
    {'[', TokenType::OpenBracket},
    {'{', TokenType::OpenBrace},
    {']', TokenType::ClosedBracket},
    {'}', TokenType::ClosedBrace},
    {';', TokenType::SemiColon},
    {':', TokenType::Colon},
    {'\'', TokenType::Apostrophe},
    {'"', TokenType::Quote},
    {'\\', TokenType::Backslack},
    {'|', TokenType::Pipe},
    {',', TokenType::Comma},
    {'<', TokenType::LessThan},
    {'.', TokenType::Period},
    {'>', TokenType::GreaterThan},
    {'/', TokenType::ForwardSlash},
    {'?', TokenType::QuestionMark},
};

Optional<TokenType> get_punctuation(Char ch) {
  // TODO: Maybe sort the map and do a binary search?
  for (auto& pi : PUNCTUATION_MAP) {
    if (pi.ch == ch) {
      return pi.type;
    }
  }

  return {};
}

StringView read_whitespace(StringView source) {
  StringLength length = 0;
  for (; is_whitespace(source[length]) && source[length] != 0; ++length) {
  }
  return source.subString(0, length);
}

Optional<StringView> read_number(StringView source) {
  StringLength length;

  // Check the first character.
  if (is_number(source[0]) || (source[0] == '-' && is_number(source[1]))) {
    length = 1;
  } else {
    return {};
  }

  for (; is_number(source[length]) || source[length] == '.'; ++length) {
  }

  return source.subString(0, length);
}

StringView read_text(StringView source) {
  StringLength length = 0;
  for (;;) {
    Char ch = source[length];
    if (is_whitespace(ch) || ch == '\0' || length == source.length()) {
      break;
    }

    ++length;
  }

  return source.subString(0, length);
}

}  // namespace

Tokenizer::Tokenizer(StringView source) : source_{source}, current_{source} {}

Token Tokenizer::peek_next_token(U32 options) {
  auto token = peek_next_token_internal(current_);
  if (NU_BIT_IS_SET(options, SkipWhitespace) && token.type == TokenType::Whitespace) {
    token = peek_next_token_internal(current_.subString(token.text.length()));
  }
  return token;
}

Token Tokenizer::consume_next_token(U32 options) {
  auto token = peek_next_token_internal(current_);
  advance(token.text.length());
  if (NU_BIT_IS_SET(options, SkipWhitespace) && token.type == TokenType::Whitespace) {
    auto token_2 = peek_next_token_internal(current_.subString(token.text.length()));
    advance(token.text.length() + token_2.text.length());
    return token_2;
  }
  return token;
}

void Tokenizer::advance(StringLength length) {
  current_ = current_.subString(length);
}

Token Tokenizer::peek_next_token_internal(StringView source) {
  // EndOfSource
  if (source.empty() || source[0] == '\0') {
    return {};
  }

  Char next_char = source[0];

  // Whitespace

  if (is_whitespace(next_char)) {
    StringView whitespace = read_whitespace(source);
    return {TokenType::Whitespace, whitespace};
  }

  // Number (MUST be read before punctuation, because a number can start with a minus symbol)
  auto maybe_number = read_number(source);
  if (maybe_number.has_value()) {
    return {TokenType::Number, maybe_number.value()};
  }

  // Punctuation
  auto maybe_punctuation = get_punctuation(next_char);
  if (maybe_punctuation.has_value()) {
    return {maybe_punctuation.value(), source.subString(0, 1)};
  }

  // Text
  StringView text = read_text(current_);
  if (text.length() > 0) {
    return Token{TokenType::Text, text};
  }

  return Token{TokenType::EndOfSource, {}};
}

}  // namespace nu
