#pragma once

#include "nucleus/Text/string_view.h"

namespace nu {

class SourceLocation {
public:
  SourceLocation(nu::StringView file, I32 line) : m_file{file}, m_line{line} {}

  StringView file() const {
    return m_file;
  }

  I32 line() const {
    return m_line;
  }

private:
  nu::StringView m_file;
  I32 m_line;
};

#define FROM_HERE (::nu::SourceLocation{__FILE__, __LINE__})

}  // namespace nu
