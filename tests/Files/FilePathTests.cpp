
#include "gtest/gtest.h"
#include "nucleus/Files/FilePath.h"
#include "nucleus/Macros.h"

#include "nucleus/MemoryDebug.h"

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
    {FILE_PATH_LITERAL("/aa/../bb/cc"), FILE_PATH_LITERAL("/aa/../bb")},
#if OS(WIN)
  // {FILE_PATH_LITERAL("\x0143:"), FILE_PATH_LITERAL(".")},
#endif  // OS(WIN)
#if defined(FILE_PATH_USES_DRIVE_LETTERS)
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
#endif  // defined(FILE_PATH_USES_DRIVE_LETTERS)
#if defined(FILE_PATH_USES_WIN_SEPARATORS)
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
    {FILE_PATH_LITERAL("aa\\..\\bb\\c"), FILE_PATH_LITERAL("aa\\..\\bb")},
#if defined(FILE_PATH_USES_DRIVE_LETTERS)
    {FILE_PATH_LITERAL("c:\\"), FILE_PATH_LITERAL("c:\\")},
    {FILE_PATH_LITERAL("c:\\\\"), FILE_PATH_LITERAL("c:\\\\")},
    {FILE_PATH_LITERAL("c:\\\\\\"), FILE_PATH_LITERAL("c:\\")},
    {FILE_PATH_LITERAL("c:\\aa"), FILE_PATH_LITERAL("c:\\")},
    {FILE_PATH_LITERAL("c:\\aa\\"), FILE_PATH_LITERAL("c:\\")},
    {FILE_PATH_LITERAL("c:\\aa\\bb"), FILE_PATH_LITERAL("c:\\aa")},
    {FILE_PATH_LITERAL("c:aa\\bb"), FILE_PATH_LITERAL("c:aa")},
#endif  // defined(FILE_PATH_USES_WIN_SEPARATORS)
#endif  // defined(FILE_PATH_USES_WIN_SEPARATORS)
  };

  for (size_t i = 0; i < ARRAY_SIZE(cases); ++i) {
    FilePath input(cases[i].input);
    FilePath observed = input.dirName();
    EXPECT_EQ(String(cases[i].expected), observed.getPath()) << "i: " << i << ", input: " << input.getPath();
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
  // {FILE_PATH_LITERAL("\x0143:"), FILE_PATH_LITERAL("\x0143:")},
#endif  // OS(WIN)
#if defined(FILE_PATH_USES_DRIVE_LETTERS)
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
#endif  // defined(FILE_PATH_USES_DRIVE_LETTERS)
#if defined(FILE_PATH_USES_WIN_SEPARATORS)
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
#if defined(FILE_PATH_USES_DRIVE_LETTERS)
    {FILE_PATH_LITERAL("c:\\"), FILE_PATH_LITERAL("\\")},
    {FILE_PATH_LITERAL("c:\\\\"), FILE_PATH_LITERAL("\\\\")},
    {FILE_PATH_LITERAL("c:\\\\\\"), FILE_PATH_LITERAL("\\")},
    {FILE_PATH_LITERAL("c:\\aa"), FILE_PATH_LITERAL("aa")},
    {FILE_PATH_LITERAL("c:\\aa\\"), FILE_PATH_LITERAL("aa")},
    {FILE_PATH_LITERAL("c:\\aa\\bb"), FILE_PATH_LITERAL("bb")},
    {FILE_PATH_LITERAL("c:aa\\bb"), FILE_PATH_LITERAL("bb")},
#endif  // defined(FILE_PATH_USES_DRIVE_LETTERS)
#endif  // defined(FILE_PATH_USES_WIN_SEPARATORS)
  };

  for (size_t i = 0; i < ARRAY_SIZE(cases); ++i) {
    FilePath input{cases[i].input};
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
#if 0
    {{FILE_PATH_LITERAL(""), FILE_PATH_LITERAL("cc")}, FILE_PATH_LITERAL("cc")},
    {{FILE_PATH_LITERAL("."), FILE_PATH_LITERAL("ff")}, FILE_PATH_LITERAL("ff")},
#endif  // 0
    {{FILE_PATH_LITERAL("."), FILE_PATH_LITERAL("")}, FILE_PATH_LITERAL(".")},
    {{FILE_PATH_LITERAL("/"), FILE_PATH_LITERAL("cc")}, FILE_PATH_LITERAL("/cc")},
    {{FILE_PATH_LITERAL("/aa"), FILE_PATH_LITERAL("")}, FILE_PATH_LITERAL("/aa")},
    {{FILE_PATH_LITERAL("/aa/"), FILE_PATH_LITERAL("")}, FILE_PATH_LITERAL("/aa")},
    {{FILE_PATH_LITERAL("//aa"), FILE_PATH_LITERAL("")}, FILE_PATH_LITERAL("//aa")},
    {{FILE_PATH_LITERAL("//aa/"), FILE_PATH_LITERAL("")}, FILE_PATH_LITERAL("//aa")},
    {{FILE_PATH_LITERAL("//"), FILE_PATH_LITERAL("aa")}, FILE_PATH_LITERAL("//aa")},
#if defined(FILE_PATH_USES_DRIVE_LETTERS)
    {{FILE_PATH_LITERAL("c:"), FILE_PATH_LITERAL("a")}, FILE_PATH_LITERAL("c:a")},
    {{FILE_PATH_LITERAL("c:"), FILE_PATH_LITERAL("")}, FILE_PATH_LITERAL("c:")},
    {{FILE_PATH_LITERAL("c:/"), FILE_PATH_LITERAL("a")}, FILE_PATH_LITERAL("c:/a")},
    {{FILE_PATH_LITERAL("c://"), FILE_PATH_LITERAL("a")}, FILE_PATH_LITERAL("c://a")},
    {{FILE_PATH_LITERAL("c:///"), FILE_PATH_LITERAL("a")}, FILE_PATH_LITERAL("c:/a")},
#endif  // defined(FILE_PATH_USES_DRIVE_LETTERS)
#if defined(FILE_PATH_USES_WIN_SEPARATORS)
    // Append introduces the default separator character, so these test cases
    // need to be defined with different expected results on platforms that use
    // different default separator characters.
    {{FILE_PATH_LITERAL("\\"), FILE_PATH_LITERAL("cc")}, FILE_PATH_LITERAL("\\cc")},
    {{FILE_PATH_LITERAL("\\aa"), FILE_PATH_LITERAL("")}, FILE_PATH_LITERAL("\\aa")},
    {{FILE_PATH_LITERAL("\\aa\\"), FILE_PATH_LITERAL("")}, FILE_PATH_LITERAL("\\aa")},
    {{FILE_PATH_LITERAL("\\\\aa"), FILE_PATH_LITERAL("")}, FILE_PATH_LITERAL("\\\\aa")},
    {{FILE_PATH_LITERAL("\\\\aa\\"), FILE_PATH_LITERAL("")}, FILE_PATH_LITERAL("\\\\aa")},
    {{FILE_PATH_LITERAL("\\\\"), FILE_PATH_LITERAL("aa")}, FILE_PATH_LITERAL("\\\\aa")},
    {{FILE_PATH_LITERAL("/aa/bb"), FILE_PATH_LITERAL("cc")}, FILE_PATH_LITERAL("/aa/bb\\cc")},
    {{FILE_PATH_LITERAL("/aa/bb/"), FILE_PATH_LITERAL("cc")}, FILE_PATH_LITERAL("/aa/bb\\cc")},
    {{FILE_PATH_LITERAL("aa/bb/"), FILE_PATH_LITERAL("cc")}, FILE_PATH_LITERAL("aa/bb\\cc")},
    {{FILE_PATH_LITERAL("aa/bb"), FILE_PATH_LITERAL("cc")}, FILE_PATH_LITERAL("aa/bb\\cc")},
    {{FILE_PATH_LITERAL("a/b"), FILE_PATH_LITERAL("c")}, FILE_PATH_LITERAL("a/b\\c")},
    {{FILE_PATH_LITERAL("a/b/"), FILE_PATH_LITERAL("c")}, FILE_PATH_LITERAL("a/b\\c")},
    {{FILE_PATH_LITERAL("//aa"), FILE_PATH_LITERAL("bb")}, FILE_PATH_LITERAL("//aa\\bb")},
    {{FILE_PATH_LITERAL("//aa/"), FILE_PATH_LITERAL("bb")}, FILE_PATH_LITERAL("//aa\\bb")},
    {{FILE_PATH_LITERAL("\\aa\\bb"), FILE_PATH_LITERAL("cc")}, FILE_PATH_LITERAL("\\aa\\bb\\cc")},
    {{FILE_PATH_LITERAL("\\aa\\bb\\"), FILE_PATH_LITERAL("cc")}, FILE_PATH_LITERAL("\\aa\\bb\\cc")},
    {{FILE_PATH_LITERAL("aa\\bb\\"), FILE_PATH_LITERAL("cc")}, FILE_PATH_LITERAL("aa\\bb\\cc")},
    {{FILE_PATH_LITERAL("aa\\bb"), FILE_PATH_LITERAL("cc")}, FILE_PATH_LITERAL("aa\\bb\\cc")},
    {{FILE_PATH_LITERAL("a\\b"), FILE_PATH_LITERAL("c")}, FILE_PATH_LITERAL("a\\b\\c")},
    {{FILE_PATH_LITERAL("a\\b\\"), FILE_PATH_LITERAL("c")}, FILE_PATH_LITERAL("a\\b\\c")},
    {{FILE_PATH_LITERAL("\\\\aa"), FILE_PATH_LITERAL("bb")}, FILE_PATH_LITERAL("\\\\aa\\bb")},
    {{FILE_PATH_LITERAL("\\\\aa\\"), FILE_PATH_LITERAL("bb")}, FILE_PATH_LITERAL("\\\\aa\\bb")},
#if defined(FILE_PATH_USES_DRIVE_LETTERS)
    {{FILE_PATH_LITERAL("c:\\"), FILE_PATH_LITERAL("a")}, FILE_PATH_LITERAL("c:\\a")},
    {{FILE_PATH_LITERAL("c:\\\\"), FILE_PATH_LITERAL("a")}, FILE_PATH_LITERAL("c:\\\\a")},
    {{FILE_PATH_LITERAL("c:\\\\\\"), FILE_PATH_LITERAL("a")}, FILE_PATH_LITERAL("c:\\a")},
    {{FILE_PATH_LITERAL("c:\\"), FILE_PATH_LITERAL("")}, FILE_PATH_LITERAL("c:\\")},
    {{FILE_PATH_LITERAL("c:\\a"), FILE_PATH_LITERAL("b")}, FILE_PATH_LITERAL("c:\\a\\b")},
    {{FILE_PATH_LITERAL("c:\\a\\"), FILE_PATH_LITERAL("b")}, FILE_PATH_LITERAL("c:\\a\\b")},
#endif  // defined(FILE_PATH_USES_DRIVE_LETTERS)
#else   // defined(FILE_PATH_USES_WIN_SEPARATORS)
    {{FILE_PATH_LITERAL("/aa/bb"), FILE_PATH_LITERAL("cc")}, FILE_PATH_LITERAL("/aa/bb/cc")},
    {{FILE_PATH_LITERAL("/aa/bb/"), FILE_PATH_LITERAL("cc")}, FILE_PATH_LITERAL("/aa/bb/cc")},
    {{FILE_PATH_LITERAL("aa/bb/"), FILE_PATH_LITERAL("cc")}, FILE_PATH_LITERAL("aa/bb/cc")},
    {{FILE_PATH_LITERAL("aa/bb"), FILE_PATH_LITERAL("cc")}, FILE_PATH_LITERAL("aa/bb/cc")},
    {{FILE_PATH_LITERAL("a/b"), FILE_PATH_LITERAL("c")}, FILE_PATH_LITERAL("a/b/c")},
    {{FILE_PATH_LITERAL("a/b/"), FILE_PATH_LITERAL("c")}, FILE_PATH_LITERAL("a/b/c")},
    {{FILE_PATH_LITERAL("//aa"), FILE_PATH_LITERAL("bb")}, FILE_PATH_LITERAL("//aa/bb")},
    {{FILE_PATH_LITERAL("//aa/"), FILE_PATH_LITERAL("bb")}, FILE_PATH_LITERAL("//aa/bb")},
#if defined(FILE_PATH_USES_DRIVE_LETTERS)
    {{FILE_PATH_LITERAL("c:/"), FILE_PATH_LITERAL("a")}, FILE_PATH_LITERAL("c:/a")},
    {{FILE_PATH_LITERAL("c:/"), FILE_PATH_LITERAL("")}, FILE_PATH_LITERAL("c:/")},
    {{FILE_PATH_LITERAL("c:/a"), FILE_PATH_LITERAL("b")}, FILE_PATH_LITERAL("c:/a/b")},
    {{FILE_PATH_LITERAL("c:/a/"), FILE_PATH_LITERAL("b")}, FILE_PATH_LITERAL("c:/a/b")},
#endif  // defined(FILE_PATH_USES_DRIVE_LETTERS)
#endif  // defined(FILE_PATH_USES_WIN_SEPARATORS)
  };

  for (size_t i = 0; i < ARRAY_SIZE(cases); ++i) {
    FilePath root{cases[i].inputs[0]};
    FilePath::StringType leaf(cases[i].inputs[1]);

    FilePath observedStr = root.append(leaf);
    EXPECT_EQ(String(cases[i].expected), observedStr.getPath())
        << "i: " << i << ", root: " << root.getPath() << ", leaf: " << leaf;

    FilePath observedPath = root.append(FilePath(leaf));
    EXPECT_EQ(String(cases[i].expected), observedPath.getPath())
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
