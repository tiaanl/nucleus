
#include "nucleus/Parser/Parser.h"

#include "nucleus/Logging.h"

#include <utility>

namespace nu {

Parser::~Parser() = default;

// static
ParsedSource Parser::parseStream(InputStream* stream) {
  LOG(Info) << "Parsing stream...";

  constexpr InputStream::SizeType kTempBufferSize = 1024;

  I8 temp[kTempBufferSize];
  DynamicArray<I8> source;

  // Stream in the source in 1K chunks.
  for (;;) {
    auto bytesRead = stream->read(temp, kTempBufferSize);
    source.pushBack(temp, temp + bytesRead);
    if (bytesRead < kTempBufferSize) {
      break;
    }
  }

  // Add a null terminator.
  source.pushBack(0);

  LOG(Info) << "Source: " << source.getData();

  Parser parser(std::move(source));

  parser.skipWhitespace();
  while (parser.parseToken()) {
    parser.skipWhitespace();
  }

  return ParsedSource(std::move(parser.m_source), std::move(parser.m_tokens));
}

Parser::Parser(DynamicArray<CharType>&& source) : m_source(source) {}

bool Parser::skipWhitespace() {
  while (isWhitespace(m_source[m_currentIndex])) {
    ++m_currentIndex;
  }

  return shouldContinue();
}

bool Parser::parseToken() {
  CharType current = m_source[m_currentIndex];

  if (isNumber(current)) {
    parseNumber();
  } else if (isAlpha(current)) {
    parseIdentifier();
  } else if (current == '"') {
    parseString();
  } else if (isPunctuation(current)) {
    parsePunctuation();
  } else {
    LOG(Error) << "Illegal character found!";
    return false;
  }

  return shouldContinue();
}

bool Parser::parseNumber() {
  MemSize startIndex = m_currentIndex;
  MemSize length = 0;

  while (isNumber(m_source[m_currentIndex]) || m_source[m_currentIndex] == '.') {
    ++length;
    ++m_currentIndex;
  }

  if (length > 0) {
    addToken(TokenType::Number, startIndex, length);
  }

  return shouldContinue();
}

bool Parser::parseIdentifier() {
  MemSize startIndex = m_currentIndex;
  MemSize length = 0;

  while (isAlpha(m_source[m_currentIndex]) || isNumber(m_source[m_currentIndex])) {
    ++length;
    ++m_currentIndex;
  }

  if (length > 0) {
    addToken(TokenType::Identifier, startIndex, length);
  }

  return shouldContinue();
}

bool Parser::parseString() {
  // Skip the first quote.
  MemSize startIndex = ++m_currentIndex;
  MemSize length = 0;

  while (m_source[m_currentIndex] != '"') {
    ++length;
    ++m_currentIndex;
  }

  if (length > 0) {
    addToken(TokenType::String, startIndex, length - 1);
  }

  return shouldContinue();
}

bool Parser::parsePunctuation() {
  addToken(TokenType::Punctuation, m_currentIndex++, 1);

  return shouldContinue();
}

bool Parser::shouldContinue() const {
  return m_currentIndex < m_source.getSize() && m_source[m_currentIndex] != 0;
}

void Parser::addToken(TokenType tokenType, MemSize index, MemSize length) {
  m_tokens.emplaceBack(tokenType, index, length);
}

// static
bool Parser::isWhitespace(CharType ch) {
  return (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\t');
}

// static
bool Parser::isNumber(CharType ch) {
  return (ch >= '0' && ch <= '9');
}

// static
bool Parser::isAlpha(CharType ch) {
  return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

// static
bool Parser::isPunctuation(CharType ch) {
  auto punctuation = R"eof(`~!@#$%^&*()-_=+[{]}\|;:',<.>/?)eof";
  return std::strchr(punctuation, ch) != nullptr;
}

}  // namespace nu
