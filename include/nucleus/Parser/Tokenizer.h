
#ifndef NUCLEUS_PARSER_TOKENIZER_H_
#define NUCLEUS_PARSER_TOKENIZER_H_

#include "nucleus/Text/StringView.h"

namespace nu {

enum class TokenType {
  EndOfSource,
  Whitespace,

  Acute,
  Tilde,
  ExclamationMark,
  At,
  Hash,
  DollarSign,
  Percentage,
  Caret,
  Ampersand,
  Asterisk,
  OpenParenthesis,
  ClosedParenthesis,
  Minus,
  Underscore,
  Equals,
  Plus,
  OpenBracket,
  OpenBrace,
  ClosedBracket,
  ClosedBrace,
  SemiColon,
  Colon,
  Apostrophe,
  Quote,
  Backslack,
  Pipe,
  Comma,
  LessThan,
  Period,
  GreaterThan,
  ForwardSlash,
  QuestionMark,

  Number,
  Text,
};

struct Token {
  TokenType type = TokenType::EndOfSource;
  StringView text;
};

class Tokenizer {
public:
  enum  {
    // SkipWhitespace = 0x01,
  };

  Tokenizer(StringView source);

  Token peekNextToken(U32 options = 0);
  Token consumeNextToken(U32 options = 0);

private:
  Token readWhitespace();
  Token readText();

  StringView m_source;
  StringLength m_currentIndex;
};

}  // namespace nu

#endif  // NUCLEUS_PARSER_TOKENIZER_H_
