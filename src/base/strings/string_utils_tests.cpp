// Copyright (c) 2015, Tiaan Louw
//
// Permission to use, copy, modify, and/or distribute this software for any
// purpose with or without fee is hereby granted, provided that the above
// copyright notice and this permission notice appear in all copies.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
// REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
// AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
// INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
// LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
// OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
// PERFORMANCE OF THIS SOFTWARE.

#include <gtest/gtest.h>

#include "base/strings/string_utils.h"

namespace base {

TEST(StringUtilTest, StartsWith) {
  EXPECT_TRUE(StartsWithASCII("javascript:url", "javascript", true));
  EXPECT_FALSE(StartsWithASCII("JavaScript:url", "javascript", true));
  EXPECT_TRUE(StartsWithASCII("javascript:url", "javascript", false));
  EXPECT_TRUE(StartsWithASCII("JavaScript:url", "javascript", false));
  EXPECT_FALSE(StartsWithASCII("java", "javascript", true));
  EXPECT_FALSE(StartsWithASCII("java", "javascript", false));
  EXPECT_FALSE(StartsWithASCII(std::string(), "javascript", false));
  EXPECT_FALSE(StartsWithASCII(std::string(), "javascript", true));
  EXPECT_TRUE(StartsWithASCII("java", std::string(), false));
  EXPECT_TRUE(StartsWithASCII("java", std::string(), true));

#if 0
  EXPECT_TRUE(StartsWith(ASCIIToUTF16("javascript:url"),
                         ASCIIToUTF16("javascript"), true));
  EXPECT_FALSE(StartsWith(ASCIIToUTF16("JavaScript:url"),
                          ASCIIToUTF16("javascript"), true));
  EXPECT_TRUE(StartsWith(ASCIIToUTF16("javascript:url"),
                         ASCIIToUTF16("javascript"), false));
  EXPECT_TRUE(StartsWith(ASCIIToUTF16("JavaScript:url"),
                         ASCIIToUTF16("javascript"), false));
  EXPECT_FALSE(
      StartsWith(ASCIIToUTF16("java"), ASCIIToUTF16("javascript"), true));
  EXPECT_FALSE(
      StartsWith(ASCIIToUTF16("java"), ASCIIToUTF16("javascript"), false));
  EXPECT_FALSE(StartsWith(string16(), ASCIIToUTF16("javascript"), false));
  EXPECT_FALSE(StartsWith(string16(), ASCIIToUTF16("javascript"), true));
  EXPECT_TRUE(StartsWith(ASCIIToUTF16("java"), string16(), false));
  EXPECT_TRUE(StartsWith(ASCIIToUTF16("java"), string16(), true));
#endif  // 0
}

}  // namespace base
