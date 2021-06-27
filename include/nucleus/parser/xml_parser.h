#pragma once

#include "nucleus/Containers/dynamic_array.h"
#include "nucleus/Text/string_view.h"

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

XMLDocument parseXMLDocument(const StringView& source);

}  // namespace nu
