
#include "nucleus/FilePath.h"
#include "nucleus/Macros.h"
#include "nucleus/Testing.h"

#include "nucleus/MemoryDebug.h"

namespace nu {

TEST_CASE("DirName") {
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

  for (auto& i : cases) {
    FilePath input{i.input};
    FilePath observed = input.dirName();
    CHECK(observed.getPath() == StringView{i.expected});
  }
}

TEST_CASE("BaseName") {
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

  for (size_t i = 0; i < NU_ARRAY_SIZE(cases); ++i) {
    FilePath input{cases[i].input};
    FilePath observed = input.baseName();
    CHECK(observed.getPath() == StringView{cases[i].expected});
  }
}

TEST_CASE("Append") {
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
#else   // defined(FILE_PATH_USES_WIN_SEPARATORS)
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

  for (size_t i = 0; i < NU_ARRAY_SIZE(cases); ++i) {
    FilePath root{cases[i].inputs[0]};
    FilePath leaf{cases[i].inputs[1]};

    FilePath observedStr = root.append(leaf);
    CHECK(observedStr.getPath() == StringView{cases[i].expected});
    // << "i: " << i << ", root: " << root.getPath() << ", leaf: " << leaf;

    FilePath observedPath = root.append(FilePath(leaf));
    CHECK(observedPath.getPath() == StringView{cases[i].expected});
    // << "i: " << i << ", root: " << root.getPath() << ", leaf: " << leaf;

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

TEST_CASE("print some general uses of FilePath") {
  FilePath resources{"resources"};
  FilePath root = getCurrentWorkingDirectory() / resources / "default.png";
  FilePath dirName = root.dirName();
  FilePath baseName = root.baseName();

  LOG(Info) << "Path: " << root.getPath();
  LOG(Info) << "Dir name: " << dirName.getPath();
  LOG(Info) << "Base name: " << baseName.getPath();
}

}  // namespace nu
