
#include "nucleus/Parser/XMLParser.h"

#include "nucleus/Parser/Tokenizer.h"

namespace nu {

namespace {

DynamicArray<XMLNode> readXMLAttributes(Tokenizer& tokenizer) {
  DynamicArray<XMLNode> result;
  return result;
}

bool readXMLTagLine(Tokenizer& tokenizer) {
  Token token;

  token = tokenizer.consumeNextToken();
  if (token.type != TokenType::LessThan) {
    return false;
  }

  token = tokenizer.consumeNextToken();
  if (token.type != TokenType::QuestionMark) {
    return false;
  }

  token = tokenizer.consumeNextToken();
  if (token.type != TokenType::Text || token.text.compare("xml") != 0) {
    return false;
  }

  auto attributes = readXMLAttributes(tokenizer);

  return true;
}

}  // namespace

XMLDocument parseXMLDocument(const StringView& source) {
  auto tokenizer = Tokenizer(source);

  XMLDocument result;

  if (!readXMLTagLine(tokenizer)) {
    return {};
  }

  return result;
}

}  // namespace nu
