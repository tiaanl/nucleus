
#include <gtest/gtest.h>

#include "nucleus/strings/string_utils.h"

namespace nu {

TEST(StringUtilTest, StartsWith) {
  EXPECT_TRUE(StartsWithASCII("javascript:url", "javascript", true));
  EXPECT_FALSE(StartsWithASCII("JavaScript:url", "javascript", true));
  EXPECT_TRUE(StartsWithASCII("javascript:url", "javascript", false));
  // EXPECT_TRUE(StartsWithASCII("JavaScript:url", "javascript", false));
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

}  // namespace nu
