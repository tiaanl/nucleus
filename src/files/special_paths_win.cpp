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

#include "nucleus/files/special_paths.h"

#include "nucleus/win/windows_mixin.h"

namespace nu {

// static
bool SpecialPath::getExePath(FilePath* pathOut) {
  wchar_t buf[MAX_PATH];
  DWORD result = ::GetModuleFileNameW(::GetModuleHandle(NULL), buf, MAX_PATH);
  if (!result)
    return false;
  buf[result] = L'\0';
  *pathOut = FilePath(buf);
  return true;
}

// static
bool SpecialPath::getConfigRootDir(FilePath* pathOut) {
  wchar_t buf[MAX_PATH];

  if (!SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_PROFILE, NULL, 0, buf)))
    return false;

  *pathOut = FilePath(buf);
  return true;
}

// static
bool SpecialPath::getTempDirectory(FilePath* pathOut) {
  wchar_t dst[MAX_PATH];
  dst[0] = 0;
  if (!::GetTempPathW(MAX_PATH, dst))
    return false;
  *pathOut = FilePath(dst);
  return true;
}

// static
bool SpecialPath::getSourceRoot(FilePath* pathOut) {
  std::string file(__FILE__);
  FilePath root(std::wstring(std::begin(file), std::end(file)));

  root = root.dirName().dirName().dirName().dirName();
  *pathOut = root;

  return true;
}

}  // namespace nu
