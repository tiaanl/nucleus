
#ifndef NUCLEUS_PARSER_XML_PARSER_H_
#define NUCLEUS_PARSER_XML_PARSER_H_

#include "nucleus/Containers/DynamicArray.h"
#include "nucleus/Optional.h"
#include "nucleus/Text/StringView.h"

namespace nu {

struct XMLAttribute {
  StringView name;
  StringView value;
};

struct XMLNode {
  StringView name;
  DynamicArray<XMLAttribute> attributes;
  DynamicArray<XMLNode> children;
};

struct XMLDocument {
  XMLNode rootNode;
};

Optional<XMLDocument> parseXMLDocument(const StringView& source);

}  // namespace nu

#endif  // NUCLEUS_PARSER_XML_PARSER_H_
