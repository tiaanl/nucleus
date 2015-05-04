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

#include "nucleus/files/file_path.h"
#include "nucleus/macros.h"

#define FPL(Str) FILE_PATH_LITERAL(Str)

namespace nu {

TEST(FilePathTest, DirName) {
  const struct {
    const FilePath::CharType* input;
    const FilePath::CharType* expected;
  } cases[] = {
    {FILE_PATH_LITERAL(""), FILE_PATH_LITERAL(".")},
    {FILE_PATH_LITERAL("aa"), FILE_PATH_LITERAL(".")},
    {FILE_PATH_LITERAL("/aa/bb"), FILE_PATH_LITERAL("/aa")},
    {FILE_PATH_LITERAL("/aa/bb/"), FILE_PATH_LITERAL("/aa")},
    {FILE_PATH_LITERAL("/aa/bb//"), FILE_PATH_LITERAL("/aa")},
    {FILE_PATH_LITERAL("/aa/bb/ccc"), FILE_PATH_LITERAL("/aa/bb")},
    {FILE_PATH_LITERAL("/aa"), FILE_PATH_LITERAL("/")},
    {FILE_PATH_LITERAL("/aa/"), FILE_PATH_LITERAL("/")},
    {FILE_PATH_LITERAL("/"), FILE_PATH_LITERAL("/")},
    {FILE_PATH_LITERAL("//"), FILE_PATH_LITERAL("//")},
    {FILE_PATH_LITERAL("///"), FILE_PATH_LITERAL("/")},
    {FILE_PATH_LITERAL("aa/"), FILE_PATH_LITERAL(".")},
    {FILE_PATH_LITERAL("aa/bb"), FILE_PATH_LITERAL("aa")},
    {FILE_PATH_LITERAL("aa/bb/"), FILE_PATH_LITERAL("aa")},
    {FILE_PATH_LITERAL("aa/bb//"), FILE_PATH_LITERAL("aa")},
    {FILE_PATH_LITERAL("aa//bb//"), FILE_PATH_LITERAL("aa")},
    {FILE_PATH_LITERAL("aa//bb/"), FILE_PATH_LITERAL("aa")},
    {FILE_PATH_LITERAL("aa//bb"), FILE_PATH_LITERAL("aa")},
    {FILE_PATH_LITERAL("//aa/bb"), FILE_PATH_LITERAL("//aa")},
    {FILE_PATH_LITERAL("//aa/"), FILE_PATH_LITERAL("//")},
    {FILE_PATH_LITERAL("//aa"), FILE_PATH_LITERAL("//")},
    {FILE_PATH_LITERAL("0:"), FILE_PATH_LITERAL(".")},
    {FILE_PATH_LITERAL("@:"), FILE_PATH_LITERAL(".")},
    {FILE_PATH_LITERAL("[:"), FILE_PATH_LITERAL(".")},
    {FILE_PATH_LITERAL("`:"), FILE_PATH_LITERAL(".")},
    {FILE_PATH_LITERAL("{:"), FILE_PATH_LITERAL(".")},
    {FILE_PATH_LITERAL("\xB3:"), FILE_PATH_LITERAL(".")},
    {FILE_PATH_LITERAL("\xC5:"), FILE_PATH_LITERAL(".")},
#if OS(WIN)
    {FILE_PATH_LITERAL("\x0143:"), FILE_PATH_LITERAL(".")},
    {FILE_PATH_LITERAL("c:"), FILE_PATH_LITERAL("c:")},
    {FILE_PATH_LITERAL("C:"), FILE_PATH_LITERAL("C:")},
    {FILE_PATH_LITERAL("A:"), FILE_PATH_LITERAL("A:")},
    {FILE_PATH_LITERAL("Z:"), FILE_PATH_LITERAL("Z:")},
    {FILE_PATH_LITERAL("a:"), FILE_PATH_LITERAL("a:")},
    {FILE_PATH_LITERAL("z:"), FILE_PATH_LITERAL("z:")},
    {FILE_PATH_LITERAL("c:aa"), FILE_PATH_LITERAL("c:")},
    {FILE_PATH_LITERAL("c:/"), FILE_PATH_LITERAL("c:/")},
    {FILE_PATH_LITERAL("c://"), FILE_PATH_LITERAL("c://")},
    {FILE_PATH_LITERAL("c:///"), FILE_PATH_LITERAL("c:/")},
    {FILE_PATH_LITERAL("c:/aa"), FILE_PATH_LITERAL("c:/")},
    {FILE_PATH_LITERAL("c:/aa/"), FILE_PATH_LITERAL("c:/")},
    {FILE_PATH_LITERAL("c:/aa/bb"), FILE_PATH_LITERAL("c:/aa")},
    {FILE_PATH_LITERAL("c:aa/bb"), FILE_PATH_LITERAL("c:aa")},
    {FILE_PATH_LITERAL("\\aa\\bb"), FILE_PATH_LITERAL("\\aa")},
    {FILE_PATH_LITERAL("\\aa\\bb\\"), FILE_PATH_LITERAL("\\aa")},
    {FILE_PATH_LITERAL("\\aa\\bb\\\\"), FILE_PATH_LITERAL("\\aa")},
    {FILE_PATH_LITERAL("\\aa\\bb\\ccc"), FILE_PATH_LITERAL("\\aa\\bb")},
    {FILE_PATH_LITERAL("\\aa"), FILE_PATH_LITERAL("\\")},
    {FILE_PATH_LITERAL("\\aa\\"), FILE_PATH_LITERAL("\\")},
    {FILE_PATH_LITERAL("\\"), FILE_PATH_LITERAL("\\")},
    {FILE_PATH_LITERAL("\\\\"), FILE_PATH_LITERAL("\\\\")},
    {FILE_PATH_LITERAL("\\\\\\"), FILE_PATH_LITERAL("\\")},
    {FILE_PATH_LITERAL("aa\\"), FILE_PATH_LITERAL(".")},
    {FILE_PATH_LITERAL("aa\\bb"), FILE_PATH_LITERAL("aa")},
    {FILE_PATH_LITERAL("aa\\bb\\"), FILE_PATH_LITERAL("aa")},
    {FILE_PATH_LITERAL("aa\\bb\\\\"), FILE_PATH_LITERAL("aa")},
    {FILE_PATH_LITERAL("aa\\\\bb\\\\"), FILE_PATH_LITERAL("aa")},
    {FILE_PATH_LITERAL("aa\\\\bb\\"), FILE_PATH_LITERAL("aa")},
    {FILE_PATH_LITERAL("aa\\\\bb"), FILE_PATH_LITERAL("aa")},
    {FILE_PATH_LITERAL("\\\\aa\\bb"), FILE_PATH_LITERAL("\\\\aa")},
    {FILE_PATH_LITERAL("\\\\aa\\"), FILE_PATH_LITERAL("\\\\")},
    {FILE_PATH_LITERAL("\\\\aa"), FILE_PATH_LITERAL("\\\\")},
    {FILE_PATH_LITERAL("c:\\"), FILE_PATH_LITERAL("c:\\")},
    {FILE_PATH_LITERAL("c:\\\\"), FILE_PATH_LITERAL("c:\\\\")},
    {FILE_PATH_LITERAL("c:\\\\\\"), FILE_PATH_LITERAL("c:\\")},
    {FILE_PATH_LITERAL("c:\\aa"), FILE_PATH_LITERAL("c:\\")},
    {FILE_PATH_LITERAL("c:\\aa\\"), FILE_PATH_LITERAL("c:\\")},
    {FILE_PATH_LITERAL("c:\\aa\\bb"), FILE_PATH_LITERAL("c:\\aa")},
    {FILE_PATH_LITERAL("c:aa\\bb"), FILE_PATH_LITERAL("c:aa")},
#endif  // OS(WIN)
  };

  for (size_t i = 0; i < ARRAY_SIZE(cases); ++i) {
    FilePath input(cases[i].input);
    FilePath observed = input.dirName();
    EXPECT_EQ(FilePath::StringType(cases[i].expected), observed.getPath())
        << "i: " << i << ", input: " << input.getPath();
  }
}

TEST(FilePathTest, BaseName) {
  const struct {
    const FilePath::CharType* input;
    const FilePath::CharType* expected;
  } cases[] = {
    {FILE_PATH_LITERAL(""), FILE_PATH_LITERAL("")},
    {FILE_PATH_LITERAL("aa"), FILE_PATH_LITERAL("aa")},
    {FILE_PATH_LITERAL("/aa/bb"), FILE_PATH_LITERAL("bb")},
    {FILE_PATH_LITERAL("/aa/bb/"), FILE_PATH_LITERAL("bb")},
    {FILE_PATH_LITERAL("/aa/bb//"), FILE_PATH_LITERAL("bb")},
    {FILE_PATH_LITERAL("/aa/bb/ccc"), FILE_PATH_LITERAL("ccc")},
    {FILE_PATH_LITERAL("/aa"), FILE_PATH_LITERAL("aa")},
    {FILE_PATH_LITERAL("/"), FILE_PATH_LITERAL("/")},
    {FILE_PATH_LITERAL("//"), FILE_PATH_LITERAL("//")},
    {FILE_PATH_LITERAL("///"), FILE_PATH_LITERAL("/")},
    {FILE_PATH_LITERAL("aa/"), FILE_PATH_LITERAL("aa")},
    {FILE_PATH_LITERAL("aa/bb"), FILE_PATH_LITERAL("bb")},
    {FILE_PATH_LITERAL("aa/bb/"), FILE_PATH_LITERAL("bb")},
    {FILE_PATH_LITERAL("aa/bb//"), FILE_PATH_LITERAL("bb")},
    {FILE_PATH_LITERAL("aa//bb//"), FILE_PATH_LITERAL("bb")},
    {FILE_PATH_LITERAL("aa//bb/"), FILE_PATH_LITERAL("bb")},
    {FILE_PATH_LITERAL("aa//bb"), FILE_PATH_LITERAL("bb")},
    {FILE_PATH_LITERAL("//aa/bb"), FILE_PATH_LITERAL("bb")},
    {FILE_PATH_LITERAL("//aa/"), FILE_PATH_LITERAL("aa")},
    {FILE_PATH_LITERAL("//aa"), FILE_PATH_LITERAL("aa")},
    {FILE_PATH_LITERAL("0:"), FILE_PATH_LITERAL("0:")},
    {FILE_PATH_LITERAL("@:"), FILE_PATH_LITERAL("@:")},
    {FILE_PATH_LITERAL("[:"), FILE_PATH_LITERAL("[:")},
    {FILE_PATH_LITERAL("`:"), FILE_PATH_LITERAL("`:")},
    {FILE_PATH_LITERAL("{:"), FILE_PATH_LITERAL("{:")},
    {FILE_PATH_LITERAL("\xB3:"), FILE_PATH_LITERAL("\xB3:")},
    {FILE_PATH_LITERAL("\xC5:"), FILE_PATH_LITERAL("\xC5:")},
#if OS(WIN)
    {FILE_PATH_LITERAL("\x0143:"), FILE_PATH_LITERAL("\x0143:")},
    {FILE_PATH_LITERAL("c:"), FILE_PATH_LITERAL("")},
    {FILE_PATH_LITERAL("C:"), FILE_PATH_LITERAL("")},
    {FILE_PATH_LITERAL("A:"), FILE_PATH_LITERAL("")},
    {FILE_PATH_LITERAL("Z:"), FILE_PATH_LITERAL("")},
    {FILE_PATH_LITERAL("a:"), FILE_PATH_LITERAL("")},
    {FILE_PATH_LITERAL("z:"), FILE_PATH_LITERAL("")},
    {FILE_PATH_LITERAL("c:aa"), FILE_PATH_LITERAL("aa")},
    {FILE_PATH_LITERAL("c:/"), FILE_PATH_LITERAL("/")},
    {FILE_PATH_LITERAL("c://"), FILE_PATH_LITERAL("//")},
    {FILE_PATH_LITERAL("c:///"), FILE_PATH_LITERAL("/")},
    {FILE_PATH_LITERAL("c:/aa"), FILE_PATH_LITERAL("aa")},
    {FILE_PATH_LITERAL("c:/aa/"), FILE_PATH_LITERAL("aa")},
    {FILE_PATH_LITERAL("c:/aa/bb"), FILE_PATH_LITERAL("bb")},
    {FILE_PATH_LITERAL("c:aa/bb"), FILE_PATH_LITERAL("bb")},
    {FILE_PATH_LITERAL("\\aa\\bb"), FILE_PATH_LITERAL("bb")},
    {FILE_PATH_LITERAL("\\aa\\bb\\"), FILE_PATH_LITERAL("bb")},
    {FILE_PATH_LITERAL("\\aa\\bb\\\\"), FILE_PATH_LITERAL("bb")},
    {FILE_PATH_LITERAL("\\aa\\bb\\ccc"), FILE_PATH_LITERAL("ccc")},
    {FILE_PATH_LITERAL("\\aa"), FILE_PATH_LITERAL("aa")},
    {FILE_PATH_LITERAL("\\"), FILE_PATH_LITERAL("\\")},
    {FILE_PATH_LITERAL("\\\\"), FILE_PATH_LITERAL("\\\\")},
    {FILE_PATH_LITERAL("\\\\\\"), FILE_PATH_LITERAL("\\")},
    {FILE_PATH_LITERAL("aa\\"), FILE_PATH_LITERAL("aa")},
    {FILE_PATH_LITERAL("aa\\bb"), FILE_PATH_LITERAL("bb")},
    {FILE_PATH_LITERAL("aa\\bb\\"), FILE_PATH_LITERAL("bb")},
    {FILE_PATH_LITERAL("aa\\bb\\\\"), FILE_PATH_LITERAL("bb")},
    {FILE_PATH_LITERAL("aa\\\\bb\\\\"), FILE_PATH_LITERAL("bb")},
    {FILE_PATH_LITERAL("aa\\\\bb\\"), FILE_PATH_LITERAL("bb")},
    {FILE_PATH_LITERAL("aa\\\\bb"), FILE_PATH_LITERAL("bb")},
    {FILE_PATH_LITERAL("\\\\aa\\bb"), FILE_PATH_LITERAL("bb")},
    {FILE_PATH_LITERAL("\\\\aa\\"), FILE_PATH_LITERAL("aa")},
    {FILE_PATH_LITERAL("\\\\aa"), FILE_PATH_LITERAL("aa")},
    {FILE_PATH_LITERAL("c:\\"), FILE_PATH_LITERAL("\\")},
    {FILE_PATH_LITERAL("c:\\\\"), FILE_PATH_LITERAL("\\\\")},
    {FILE_PATH_LITERAL("c:\\\\\\"), FILE_PATH_LITERAL("\\")},
    {FILE_PATH_LITERAL("c:\\aa"), FILE_PATH_LITERAL("aa")},
    {FILE_PATH_LITERAL("c:\\aa\\"), FILE_PATH_LITERAL("aa")},
    {FILE_PATH_LITERAL("c:\\aa\\bb"), FILE_PATH_LITERAL("bb")},
    {FILE_PATH_LITERAL("c:aa\\bb"), FILE_PATH_LITERAL("bb")},
#endif  // OS(WIN)
  };

  for (size_t i = 0; i < ARRAY_SIZE(cases); ++i) {
    FilePath input(cases[i].input);
    FilePath observed = input.baseName();
    EXPECT_EQ(FilePath::StringType(cases[i].expected), observed.getPath())
        << "i: " << i << ", input: " << input.getPath();
  }
}

TEST(FilePathTest, Append) {
  const struct {
    const FilePath::CharType* inputs[2];
    const FilePath::CharType* expected;
  } cases[] = {
    {{FPL(""), FPL("cc")}, FPL("cc")},
    {{FPL("."), FPL("ff")}, FPL("ff")},
    {{FPL("/"), FPL("cc")}, FPL("/cc")},
    {{FPL("/aa"), FPL("")}, FPL("/aa")},
    {{FPL("/aa/"), FPL("")}, FPL("/aa")},
    {{FPL("//aa"), FPL("")}, FPL("//aa")},
    {{FPL("//aa/"), FPL("")}, FPL("//aa")},
    {{FPL("//"), FPL("aa")}, FPL("//aa")},
#if OS(WIN)
    {{FPL("c:"), FPL("a")}, FPL("c:a")},
    {{FPL("c:"), FPL("")}, FPL("c:")},
    {{FPL("c:/"), FPL("a")}, FPL("c:/a")},
    {{FPL("c://"), FPL("a")}, FPL("c://a")},
    {{FPL("c:///"), FPL("a")}, FPL("c:/a")},
    // Append introduces the default separator character, so these test cases
    // need to be defined with different expected results on platforms that use
    // different default separator characters.
    {{FPL("\\"), FPL("cc")}, FPL("\\cc")},
    {{FPL("\\aa"), FPL("")}, FPL("\\aa")},
    {{FPL("\\aa\\"), FPL("")}, FPL("\\aa")},
    {{FPL("\\\\aa"), FPL("")}, FPL("\\\\aa")},
    {{FPL("\\\\aa\\"), FPL("")}, FPL("\\\\aa")},
    {{FPL("\\\\"), FPL("aa")}, FPL("\\\\aa")},
    {{FPL("/aa/bb"), FPL("cc")}, FPL("/aa/bb\\cc")},
    {{FPL("/aa/bb/"), FPL("cc")}, FPL("/aa/bb\\cc")},
    {{FPL("aa/bb/"), FPL("cc")}, FPL("aa/bb\\cc")},
    {{FPL("aa/bb"), FPL("cc")}, FPL("aa/bb\\cc")},
    {{FPL("a/b"), FPL("c")}, FPL("a/b\\c")},
    {{FPL("a/b/"), FPL("c")}, FPL("a/b\\c")},
    {{FPL("//aa"), FPL("bb")}, FPL("//aa\\bb")},
    {{FPL("//aa/"), FPL("bb")}, FPL("//aa\\bb")},
    {{FPL("\\aa\\bb"), FPL("cc")}, FPL("\\aa\\bb\\cc")},
    {{FPL("\\aa\\bb\\"), FPL("cc")}, FPL("\\aa\\bb\\cc")},
    {{FPL("aa\\bb\\"), FPL("cc")}, FPL("aa\\bb\\cc")},
    {{FPL("aa\\bb"), FPL("cc")}, FPL("aa\\bb\\cc")},
    {{FPL("a\\b"), FPL("c")}, FPL("a\\b\\c")},
    {{FPL("a\\b\\"), FPL("c")}, FPL("a\\b\\c")},
    {{FPL("\\\\aa"), FPL("bb")}, FPL("\\\\aa\\bb")},
    {{FPL("\\\\aa\\"), FPL("bb")}, FPL("\\\\aa\\bb")},
    {{FPL("c:\\"), FPL("a")}, FPL("c:\\a")},
    {{FPL("c:\\\\"), FPL("a")}, FPL("c:\\\\a")},
    {{FPL("c:\\\\\\"), FPL("a")}, FPL("c:\\a")},
    {{FPL("c:\\"), FPL("")}, FPL("c:\\")},
    {{FPL("c:\\a"), FPL("b")}, FPL("c:\\a\\b")},
    {{FPL("c:\\a\\"), FPL("b")}, FPL("c:\\a\\b")},
#else
    {{FPL("/aa/bb"), FPL("cc")}, FPL("/aa/bb/cc")},
    {{FPL("/aa/bb/"), FPL("cc")}, FPL("/aa/bb/cc")},
    {{FPL("aa/bb/"), FPL("cc")}, FPL("aa/bb/cc")},
    {{FPL("aa/bb"), FPL("cc")}, FPL("aa/bb/cc")},
    {{FPL("a/b"), FPL("c")}, FPL("a/b/c")},
    {{FPL("a/b/"), FPL("c")}, FPL("a/b/c")},
    {{FPL("//aa"), FPL("bb")}, FPL("//aa/bb")},
    {{FPL("//aa/"), FPL("bb")}, FPL("//aa/bb")},
    {{FPL("c:/"), FPL("a")}, FPL("c:/a")},
    {{FPL("c:/"), FPL("")}, FPL("c:/")},
    {{FPL("c:/a"), FPL("b")}, FPL("c:/a/b")},
    {{FPL("c:/a/"), FPL("b")}, FPL("c:/a/b")},
#endif  // OS(WIN)
  };

  for (size_t i = 0; i < ARRAY_SIZE(cases); ++i) {
    FilePath root(cases[i].inputs[0]);
    FilePath::StringType leaf(cases[i].inputs[1]);
    FilePath observedStr = root.append(leaf);
    EXPECT_EQ(FilePath::StringType(cases[i].expected), observedStr.getPath())
        << "i: " << i << ", root: " << root.getPath() << ", leaf: " << leaf;
    FilePath observedPath = root.append(FilePath(leaf));
    EXPECT_EQ(FilePath::StringType(cases[i].expected), observedPath.getPath())
        << "i: " << i << ", root: " << root.getPath() << ", leaf: " << leaf;

#if 0
#if defined(OS_WIN)
    std::string ascii = WideToUTF8(leaf);
#elif defined(OS_POSIX)
    std::string ascii = leaf;
#endif
    observed_str = root.AppendASCII(ascii);
    EXPECT_EQ(FilePath::StringType(cases[i].expected), observed_str.value()) <<
              "i: " << i << ", root: " << root.value() << ", leaf: " << leaf;
#endif  // 0
  }
}

}  // namespace nu
