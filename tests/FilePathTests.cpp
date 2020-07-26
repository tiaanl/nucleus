
#include <catch2/catch.hpp>

#include "nucleus/FilePath.h"
#include "nucleus/Macros.h"
#include "nucleus/MemoryDebug.h"

namespace nu {

TEST_CASE("Directory name of a file path is calculated correctly.", "[FilePath]") {
  const struct {
    const char* input;
    const char* expected;
  } cases[] = {
    {"", "."},
    {"aa", "."},
    {"/aa/bb", "/aa"},
    {"/aa/bb/", "/aa"},
    {"/aa/bb//", "/aa"},
    {"/aa/bb/ccc", "/aa/bb"},
    {"/aa", "/"},
    {"/aa/", "/"},
    {"/", "/"},
    {"//", "//"},
    {"///", "/"},
    {"aa/", "."},
    {"aa/bb", "aa"},
    {"aa/bb/", "aa"},
    {"aa/bb//", "aa"},
    {"aa//bb//", "aa"},
    {"aa//bb/", "aa"},
    {"aa//bb", "aa"},
    {"//aa/bb", "//aa"},
    {"//aa/", "//"},
    {"//aa", "//"},
    {"0:", "."},
    {"@:", "."},
    {"[:", "."},
    {"`:", "."},
    {"{:", "."},
    {"\xB3:", "."},
    {"\xC5:", "."},
    {"/aa/../bb/cc", "/aa/../bb"},
#if OS(WIN)
  // {"\x0143:", "."},
#endif  // OS(WIN)
#if defined(FILE_PATH_USES_DRIVE_LETTERS)
    {"c:", "c:"},
    {"C:", "C:"},
    {"A:", "A:"},
    {"Z:", "Z:"},
    {"a:", "a:"},
    {"z:", "z:"},
    {"c:aa", "c:"},
    {"c:/", "c:/"},
    {"c://", "c://"},
    {"c:///", "c:/"},
    {"c:/aa", "c:/"},
    {"c:/aa/", "c:/"},
    {"c:/aa/bb", "c:/aa"},
    {"c:aa/bb", "c:aa"},
#endif  // defined(FILE_PATH_USES_DRIVE_LETTERS)
#if defined(FILE_PATH_USES_WIN_SEPARATORS)
    {"\\aa\\bb", "\\aa"},
    {"\\aa\\bb\\", "\\aa"},
    {"\\aa\\bb\\\\", "\\aa"},
    {"\\aa\\bb\\ccc", "\\aa\\bb"},
    {"\\aa", "\\"},
    {"\\aa\\", "\\"},
    {"\\", "\\"},
    {"\\\\", "\\\\"},
    {"\\\\\\", "\\"},
    {"aa\\", "."},
    {"aa\\bb", "aa"},
    {"aa\\bb\\", "aa"},
    {"aa\\bb\\\\", "aa"},
    {"aa\\\\bb\\\\", "aa"},
    {"aa\\\\bb\\", "aa"},
    {"aa\\\\bb", "aa"},
    {"\\\\aa\\bb", "\\\\aa"},
    {"\\\\aa\\", "\\\\"},
    {"\\\\aa", "\\\\"},
    {"aa\\..\\bb\\c", "aa\\..\\bb"},
#if defined(FILE_PATH_USES_DRIVE_LETTERS)
    {"c:\\", "c:\\"},
    {"c:\\\\", "c:\\\\"},
    {"c:\\\\\\", "c:\\"},
    {"c:\\aa", "c:\\"},
    {"c:\\aa\\", "c:\\"},
    {"c:\\aa\\bb", "c:\\aa"},
    {"c:aa\\bb", "c:aa"},
#endif  // defined(FILE_PATH_USES_WIN_SEPARATORS)
#endif  // defined(FILE_PATH_USES_WIN_SEPARATORS)
  };

  for (auto& c : cases) {
    FilePath input{c.input};
    FilePath observed = input.dirName();
    CHECK(observed.getPath() == StringView{c.expected});
  }
}

TEST_CASE("Base name of a file path is calculated correctly.", "[FilePath]") {
  const struct {
    const char* input;
    const char* expected;
  } cases[] = {
    {"", ""},
    {"aa", "aa"},
    {"/aa/bb", "bb"},
    {"/aa/bb/", "bb"},
    {"/aa/bb//", "bb"},
    {"/aa/bb/ccc", "ccc"},
    {"/aa", "aa"},
    {"/", "/"},
    {"//", "//"},
    {"///", "/"},
    {"aa/", "aa"},
    {"aa/bb", "bb"},
    {"aa/bb/", "bb"},
    {"aa/bb//", "bb"},
    {"aa//bb//", "bb"},
    {"aa//bb/", "bb"},
    {"aa//bb", "bb"},
    {"//aa/bb", "bb"},
    {"//aa/", "aa"},
    {"//aa", "aa"},
    {"0:", "0:"},
    {"@:", "@:"},
    {"[:", "[:"},
    {"`:", "`:"},
    {"{:", "{:"},
    {"\xB3:", "\xB3:"},
    {"\xC5:", "\xC5:"},
#if OS(WIN)
  // {"\x0143:", "\x0143:"},
#endif  // OS(WIN)
#if defined(FILE_PATH_USES_DRIVE_LETTERS)
    {"c:", ""},
    {"C:", ""},
    {"A:", ""},
    {"Z:", ""},
    {"a:", ""},
    {"z:", ""},
    {"c:aa", "aa"},
    {"c:/", "/"},
    {"c://", "//"},
    {"c:///", "/"},
    {"c:/aa", "aa"},
    {"c:/aa/", "aa"},
    {"c:/aa/bb", "bb"},
    {"c:aa/bb", "bb"},
#endif  // defined(FILE_PATH_USES_DRIVE_LETTERS)
#if defined(FILE_PATH_USES_WIN_SEPARATORS)
    {"\\aa\\bb", "bb"},
    {"\\aa\\bb\\", "bb"},
    {"\\aa\\bb\\\\", "bb"},
    {"\\aa\\bb\\ccc", "ccc"},
    {"\\aa", "aa"},
    {"\\", "\\"},
    {"\\\\", "\\\\"},
    {"\\\\\\", "\\"},
    {"aa\\", "aa"},
    {"aa\\bb", "bb"},
    {"aa\\bb\\", "bb"},
    {"aa\\bb\\\\", "bb"},
    {"aa\\\\bb\\\\", "bb"},
    {"aa\\\\bb\\", "bb"},
    {"aa\\\\bb", "bb"},
    {"\\\\aa\\bb", "bb"},
    {"\\\\aa\\", "aa"},
    {"\\\\aa", "aa"},
#if defined(FILE_PATH_USES_DRIVE_LETTERS)
    {"c:\\", "\\"},
    {"c:\\\\", "\\\\"},
    {"c:\\\\\\", "\\"},
    {"c:\\aa", "aa"},
    {"c:\\aa\\", "aa"},
    {"c:\\aa\\bb", "bb"},
    {"c:aa\\bb", "bb"},
#endif  // defined(FILE_PATH_USES_DRIVE_LETTERS)
#endif  // defined(FILE_PATH_USES_WIN_SEPARATORS)
  };

  for (auto& c : cases) {
    FilePath input{c.input};
    FilePath observed = input.baseName();
    CHECK(observed.getPath() == c.expected);
  }
}

TEST_CASE("Can append more components to a file path.", "[FilePath]") {
  const struct {
    const char* inputs[2];
    const char* expected;
  } cases[] = {
#if 0
    {{""), "cc")}, "cc"},
    {{"."), "ff")}, "ff"},
#endif  // 0
    {{".", ""}, "."},
    {{"/", "cc"}, "/cc"},
    {{"/aa", ""}, "/aa"},
    {{"/aa/", ""}, "/aa"},
    {{"//aa", ""}, "//aa"},
    {{"//aa/", ""}, "//aa"},
    {{"//", "aa"}, "//aa"},
#if defined(FILE_PATH_USES_DRIVE_LETTERS)
    {{"c:", "a"}, "c:a"},
    {{"c:", ""}, "c:"},
    {{"c:/", "a"}, "c:/a"},
    {{"c://", "a"}, "c://a"},
    {{"c:///", "a"}, "c:/a"},
#endif  // defined(FILE_PATH_USES_DRIVE_LETTERS)
#if defined(FILE_PATH_USES_WIN_SEPARATORS)
    // Append introduces the default separator character, so these test cases
    // need to be defined with different expected results on platforms that use
    // different default separator characters.
    {{"\\", "cc"}, "\\cc"},
    {{"\\aa", ""}, "\\aa"},
    {{"\\aa\\", ""}, "\\aa"},
    {{"\\\\aa", ""}, "\\\\aa"},
    {{"\\\\aa\\", ""}, "\\\\aa"},
    {{"\\\\", "aa"}, "\\\\aa"},
    {{"/aa/bb", "cc"}, "/aa/bb\\cc"},
    {{"/aa/bb/", "cc"}, "/aa/bb\\cc"},
    {{"aa/bb/", "cc"}, "aa/bb\\cc"},
    {{"aa/bb", "cc"}, "aa/bb\\cc"},
    {{"a/b", "c"}, "a/b\\c"},
    {{"a/b/", "c"}, "a/b\\c"},
    {{"//aa", "bb"}, "//aa\\bb"},
    {{"//aa/", "bb"}, "//aa\\bb"},
    {{"\\aa\\bb", "cc"}, "\\aa\\bb\\cc"},
    {{"\\aa\\bb\\", "cc"}, "\\aa\\bb\\cc"},
    {{"aa\\bb\\", "cc"}, "aa\\bb\\cc"},
    {{"aa\\bb", "cc"}, "aa\\bb\\cc"},
    {{"a\\b", "c"}, "a\\b\\c"},
    {{"a\\b\\", "c"}, "a\\b\\c"},
    {{"\\\\aa", "bb"}, "\\\\aa\\bb"},
    {{"\\\\aa\\", "bb"}, "\\\\aa\\bb"},
#if defined(FILE_PATH_USES_DRIVE_LETTERS)
    {{"c:\\", "a"}, "c:\\a"},
    {{"c:\\\\", "a"}, "c:\\\\a"},
    {{"c:\\\\\\", "a"}, "c:\\a"},
    {{"c:\\", ""}, "c:\\"},
    {{"c:\\a", "b"}, "c:\\a\\b"},
    {{"c:\\a\\", "b"}, "c:\\a\\b"},
#endif  // defined(FILE_PATH_USES_DRIVE_LETTERS)
#else  // defined(FILE_PATH_USES_WIN_SEPARATORS)
    {{"/aa/bb", "cc"}, "/aa/bb/cc"},
    {{"/aa/bb/", "cc"}, "/aa/bb/cc"},
    {{"aa/bb/", "cc"}, "aa/bb/cc"},
    {{"aa/bb", "cc"}, "aa/bb/cc"},
    {{"a/b", "c"}, "a/b/c"},
    {{"a/b/", "c"}, "a/b/c"},
    {{"//aa", "bb"}, "//aa/bb"},
    {{"//aa/", "bb"}, "//aa/bb"},
#if defined(FILE_PATH_USES_DRIVE_LETTERS)
    {{"c:/", "a"}, "c:/a"},
    {{"c:/", ""}, "c:/"},
    {{"c:/a", "b"}, "c:/a/b"},
    {{"c:/a/", "b"}, "c:/a/b"},
#endif  // defined(FILE_PATH_USES_DRIVE_LETTERS)
#endif  // defined(FILE_PATH_USES_WIN_SEPARATORS)
  };

  for (auto& c : cases) {
    FilePath root{c.inputs[0]};
    FilePath leaf{c.inputs[1]};

    FilePath observedStr = root.append(leaf);
    CHECK(observedStr.getPath() == c.expected);

    FilePath observedPath = root.append(FilePath(leaf));
    CHECK(observedPath.getPath() == c.expected);
  }
}

}  // namespace nu
