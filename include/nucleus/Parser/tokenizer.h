#pragma once

#include "nucleus/Macros.h"
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
  enum {
    SkipWhitespace = NU_BIT(1),
  };

  explicit Tokenizer(StringView source);

  Token peek_next_token(U32 options = 0);
  Token consume_next_token(U32 options = 0);

private:
  void advance(StringLength length);
  Token peek_next_token_internal(StringView source);

  StringView source_;
  StringView current_;
};

}  // namespace nu
