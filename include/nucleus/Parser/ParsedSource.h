
#ifndef NUCLEUS_PARSER_PARSED_SOURCE_H_
#define NUCLEUS_PARSER_PARSED_SOURCE_H_

#include "nucleus/Containers/DynamicArray.h"
#include "nucleus/Parser/Token.h"
#include "nucleus/Streams/InputStream.h"

namespace nu {

class ParsedSource {
public:
  ~ParsedSource() = default;

  DynamicArray<Token>::Iterator begin() {
    return m_tokens.begin();
  }

  DynamicArray<Token>::Iterator end() {
    return m_tokens.end();
  }

  const I8* getTokenSource(const Token& token) const {
    return m_source.getData() + token.index;
  }

private:
  friend class Parser;

  ParsedSource(DynamicArray<I8>&& source, DynamicArray<Token>&& tokens)
    : m_source(source), m_tokens(tokens) {}

  DynamicArray<I8> m_source;
  DynamicArray<Token> m_tokens;
};

}  // namespace nu

#endif  // NUCLEUS_PARSER_PARSED_SOURCE_H_
