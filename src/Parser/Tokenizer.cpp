
#include "nucleus/Parser/Tokenizer.h"

#include "nucleus/Logging.h"

namespace nu {

namespace {

bool isWhitespace(Char ch) {
  return (ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t');
}

bool isNumber(Char ch) {
  return ch >= '0' && ch <= '9';
}

struct PunctuationInfo {
  Char ch;
  TokenType type;
} kPunctuationMap[] = {
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

// Return the type of punctuation or EndOfSource if it is not a punctuation character.
TokenType getPunctuation(Char ch) {
  // TODO: Maybe sort the map and do a binary search?
  for (auto& pi : kPunctuationMap) {
    if (pi.ch == ch) {
      return pi.type;
    }
  }
  return TokenType::EndOfSource;
}

StringView readWhitespace(const StringView& source) {
  StringLength length = 0;
  for (; isWhitespace(source[length]) && source[length] != 0; ++length) {
  }
  return source.subString(0, length);
}

StringView readNumber(const StringView& text) {
  StringLength length = 0;

  // Check the first character.
  if (isNumber(text[0]) || (text[0] == '-' && isNumber(text[1]))) {
    length = 1;
  } else {
    return StringView{};
  }

  for (; isNumber(text[length]) || text[length] == '.'; ++length) {
  }

  return StringView{text, length};
}

StringView readText(const StringView& source) {
  StringLength length = 0;
  for (;;) {
    Char ch = source[length];
    if (isWhitespace(ch) || ch == '\0' || length == source.getLength()) {
      break;
    }

    ++length;
  }

  return source.subString(0, length);
}

}  // namespace

Tokenizer::Tokenizer(const StringView& source) : m_source{source}, m_current(source) {}

Token Tokenizer::peekNextTokenInternal(const StringView& source) {
  Char nextChar = source[0];

  // EndOfSource

  if (nextChar == '\0' || source.getLength() == 0) {
    return {};
  }

  // Whitespace

  if (isWhitespace(nextChar)) {
    StringView whitespace = readWhitespace(source);
    return Token{TokenType::Whitespace, whitespace};
  }

  // Number (MUST be read before punctuation, because a number can start with a minus symbol)
  StringView number = readNumber(source);
  if (number.getLength() > 0) {
    return {TokenType::Number, number};
  }

  // Punctuation

  TokenType punctuation = getPunctuation(nextChar);
  if (punctuation != TokenType::EndOfSource) {
    return {punctuation, source.subString(0, 1)};
  }

  // Text

  StringView text = readText(m_current);
  if (text.getLength() > 0) {
    return Token{TokenType::Text, text};
  }

  return Token{TokenType::EndOfSource, StringView{}};
}

Token Tokenizer::peekNextToken(U32 options) {
  auto token = peekNextTokenInternal(m_current);
  if (options & SkipWhitespace && token.type == TokenType::Whitespace) {
    token = peekNextTokenInternal(m_current.subString(token.text.getLength()));
  }
  return token;
}

Token Tokenizer::consumeNextToken(U32 options) {
  auto token = peekNextTokenInternal(m_current);
  advance(token.text.getLength());
  if (options & SkipWhitespace && token.type == TokenType::Whitespace) {
    auto token2 = peekNextTokenInternal(m_current.subString(token.text.getLength()));
    advance(token.text.getLength() + token2.text.getLength());
    return token2;
  }
  return token;
}

void Tokenizer::advance(StringLength length) {
  m_current = m_current.subString(length);
}

}  // namespace nu
