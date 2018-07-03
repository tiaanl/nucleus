
#ifndef PARSER_TOKEN_H_
#define PARSER_TOKEN_H_

namespace nu {

enum class TokenType {
  Identifier,
  String,
  Number,
  Punctuation,
  Unknown,
};

struct Token {
  TokenType tokenType;

  // The start index inside the source. This ignored line endings.
  USize index;

  // The length of the token in the source.
  USize length;

  // Token(TokenType tokenType, USize index, USize length)
  //   : tokenType(tokenType), index(index), length(length) {}
};

}  // namespace nu

#endif  // PARSER_TOKEN_H_
