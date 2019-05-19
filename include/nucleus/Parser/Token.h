
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
  MemSize index;

  // The length of the token in the source.
  MemSize length;

  // Token(TokenType tokenType, MemSize index, MemSize length)
  //   : tokenType(tokenType), index(index), length(length) {}
};

}  // namespace nu

#endif  // PARSER_TOKEN_H_
