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

#include "base/logging.h"

#include <cassert>
#include <iostream>
#include <mutex>
#include <thread>

#include "base/config.h"

#if OS(WIN)
#include "base/win/windows_mxin.h"
#endif

namespace base {

namespace detail {

namespace {

const char* kLogEntryNames[] = {
    "Info", "Warning", "Error", "Fatal",
};

const char* logLevelToString(LogEntry::LogLevel logLevel) {
  assert(logLevel >= 0 && logLevel < LogEntry::LogLevel_COUNT);
  return kLogEntryNames[static_cast<int>(logLevel)];
}

std::mutex g_loggingMutex;

}  // namespace

LogEntry::LogEntry(LogLevel logLevel, const char* file, int line)
  : m_logLevel(logLevel), m_file(file), m_line(line) {
}

LogEntry::~LogEntry() {
  std::lock_guard<std::mutex> locker(g_loggingMutex);

  const char* logLevelName = logLevelToString(m_logLevel);

  std::stringstream outStr;

  // Output the thread id.
  outStr << '[';
  outStr.fill('0');
  outStr.width(4);
  outStr << std::hex << std::this_thread::get_id();
  outStr << "] ";

  // Output the log level.
  outStr << '[' << logLevelName << "] ";

  // Output the file name and line number.
  // outStr << '[' << m_file << ':' << m_line << "] ";

  outStr << m_stream.str();

  std::string s{outStr.str()};

  std::cout << s << std::endl;

#if OS(WIN)
  ::OutputDebugString(s.c_str());
#endif
}

}  // namespace detail

}  // namespace base
