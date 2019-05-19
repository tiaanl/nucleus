
#ifndef NUCLEUS_PARSER_PARSER_H_
#define NUCLEUS_PARSER_PARSER_H_

#include "nucleus/Containers/DynamicArray.h"
#include "nucleus/Parser/ParsedSource.h"
#include "nucleus/Streams/InputStream.h"

namespace nu {

class Parser {
public:
  MOVE_DELETE(Parser);
  COPY_DELETE(Parser);

  static ParsedSource parseStream(InputStream* stream);

  ~Parser();

private:
  using CharType = I8;

  explicit Parser(DynamicArray<CharType>&& source);

  // Advance the `m_currentIndex` past all whitespace.  Returns true if we should process more
  // tokens.
  bool skipWhitespace();

  // Parse a token, adding it to the result, and return true if we should process more tokens.
  bool parseToken();
  bool parseNumber();
  bool parseIdentifier();
  bool parseString();
  bool parsePunctuation();

  // Returns true if we should continue processing tokens.  Returns false if we are at the end of
  // the source.
  bool shouldContinue() const;

  // Adds a `Token` to the `ParsedSource` result.
  void addToken(TokenType tokenType, MemSize index, MemSize length);

  static bool isWhitespace(CharType ch);
  static bool isNumber(CharType ch);
  static bool isAlpha(CharType ch);
  static bool isPunctuation(CharType ch);

  DynamicArray<CharType> m_source;
  MemSize m_currentIndex = 0;
  DynamicArray<Token> m_tokens;
};

}  // namespace nu

#endif  // NUCLEUS_PARSER_PARSER_H_
