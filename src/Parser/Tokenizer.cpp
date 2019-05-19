
#include "nucleus/Parser/Tokenizer.h"

#include "nucleus/Logging.h"

namespace nu {

namespace {

bool isWhitespace(Char ch) {
  return (ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t');
}

bool isAlpha(Char ch) {
  return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
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
  for (auto i = 0; i < sizeof(kPunctuationMap) / sizeof(PunctuationInfo); ++i) {
    if (kPunctuationMap[i].ch == ch) {
      return kPunctuationMap[i].type;
    }
  }
  return TokenType::EndOfSource;
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

}  // namespace

Tokenizer::Tokenizer(StringView source) : m_source{source}, m_currentIndex(0) {}

Token Tokenizer::peekNextToken(U32 options) {
  Char nextChar = m_source[m_currentIndex];

  // EndOfSource

  if (nextChar == '\0' || m_currentIndex == m_source.getLength()) {
    return {};
  }

  // Whitespace

  if (isWhitespace(nextChar)) {
    return readWhitespace();
  }

  // Number (MUST be read before punctuation, because a number can start with a minus symbol)
  StringView number = readNumber(m_source.subString(m_currentIndex));
  if (number.getLength() > 0) {
    return {TokenType::Number, number};
  }

  // Punctuation

  TokenType punctuation = getPunctuation(nextChar);
  if (punctuation != TokenType::EndOfSource) {
    return {punctuation, m_source.subString(m_currentIndex, 1)};
  }

  // Text

  return readText();
}

Token Tokenizer::consumeNextToken(U32 options) {
  Token token = peekNextToken(options);
  m_currentIndex += token.text.getLength();
  return token;
}

Token Tokenizer::readWhitespace() {
  DCHECK(isWhitespace(m_source[m_currentIndex]));

  StringLength length = 0;
  for (;;) {
    Char ch = m_source[m_currentIndex + length];
    if (!isWhitespace(ch)) {
      break;
    }

    ++length;
  }

  return {TokenType::Whitespace, m_source.subString(m_currentIndex, length)};
}

Token Tokenizer::readText() {
  DCHECK(!isWhitespace(m_source[m_currentIndex]));

  StringLength length = 0;
  for (;;) {
    Char ch = m_source[m_currentIndex + length];
    if (isWhitespace(ch)) {
      break;
    }

    ++length;
  }

  return {TokenType::Text, m_source.subString(m_currentIndex, length)};
}

}  // namespace nu
