
#include "nucleus/Logging.h"

#include <cassert>
#include <iostream>
#include <mutex>
#include <thread>

#include "nucleus/Config.h"
#include "nucleus/Synchronization/AutoLock.h"
#include "nucleus/Synchronization/Lock.h"

#if OS(WIN)
#include "nucleus/Win/WindowsMixin.h"
#endif

#include "nucleus/MemoryDebug.h"

namespace nu {

namespace detail {

namespace {

const char* kLogEntryNames[] = {
    "Info", "Warning", "Error", "Fatal", "Check",
};

const char* logLevelToString(LogEntry::LogLevel logLevel) {
  assert(logLevel >= 0 && logLevel < LogEntry::LogLevel_COUNT);
  return kLogEntryNames[static_cast<int>(logLevel)];
}

Lock g_loggingLock;

}  // namespace

// static
bool LogEntry::s_showThreadIdInLog{true};
bool LogEntry::s_showLogLevelName{true};
bool LogEntry::s_showFileNameInLog{false};

LogEntry::LogEntry(LogLevel logLevel, const char* file, int line) : m_logLevel(logLevel), m_file(file), m_line(line) {}

LogEntry::~LogEntry() {
  AutoLock<Lock> locker(g_loggingLock);

  std::stringstream outStr;

  // Output the thread id.
  if (s_showThreadIdInLog) {
    outStr << '[';
    outStr.fill('0');
    outStr.width(4);
    //        outStr << std::hex << std::this_thread::get_id();
    outStr << "] ";
  }

  // Output the log level.
  if (s_showLogLevelName) {
    const char* logLevelName = logLevelToString(m_logLevel);
    outStr << '[' << logLevelName << "] ";
  }

  // Output the file name and line number.
  if (s_showFileNameInLog)
    outStr << '[' << m_file << ':' << m_line << "] ";

  outStr << m_stream.str();

  std::string s{outStr.str()};

  std::cout << s << std::endl;

  s.push_back('\n');

#if OS(WIN)
  ::OutputDebugString(s.c_str());
#endif

  // If this is a DCheck message, then we break into the debugger.
  if (m_logLevel == DCheck) {
#if OS(WIN) && COMPILER(MSVC)
    __debugbreak();
#elif OS(MACOSX)
    asm("int3");
#endif
  }
}

}  // namespace detail

}  // namespace nu
