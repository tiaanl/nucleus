
#ifndef NUCLEUS_LOGGING_H_
#define NUCLEUS_LOGGING_H_

#include <sstream>

#include "nucleus/config.h"

namespace nu {

namespace detail {

class LogEntry {
public:
  enum LogLevel {
    Info,
    Warning,
    Error,
    Fatal,
    DCheck,
    LogLevel_COUNT,
  };

  // Construct the log entry with a log level, file name and line number for
  // where the log entry was created.
  LogEntry(LogLevel logLevel, const char* file, int line);

  // This does the output part of the logging.
  ~LogEntry();

  std::ostream& getStream() {
    return m_stream;
  }

private:
  // Fields that control how the log messages are printed.
  static bool s_showThreadIdInLog;
  static bool s_showLogLevelName;
  static bool s_showFileNameInLog;

  const LogLevel m_logLevel;
  const char* m_file;
  const int m_line;

  std::stringstream m_stream;
};

class LogEntryVoidify {
public:
  // This has to to an operator with a precedence lower than << but higher than
  // ?.
  void operator&(std::ostream&) {}
};

}  // namespace detail

}  // namespace nu

#define LAZY_STREAM(Stream, Condition) !(Condition) ? static_cast<void>(0) : ::nu::detail::LogEntryVoidify() & (Stream)

#define LOG_STREAM(LogLevel) ::nu::detail::LogEntry(::nu::detail::LogEntry::LogLevel, __FILE__, __LINE__).getStream()

// LOG/DLOG

//#define LOG(LogLevel) LAZY_STREAM(LOG_STREAM(LogLevel), LOG_IS_ON(LogLevel))
#define LOG(LogLevel) LAZY_STREAM(LOG_STREAM(LogLevel), 1)

#if BUILD(DEBUG)
#define DLOG(LogLevel) LAZY_STREAM(LOG_STREAM(LogLevel), 1)
#else
#define DLOG(LogLevel) LAZY_STREAM(LOG_STREAM(LogLevel), 0)
#endif

// DCHECK

#if BUILD(DEBUG)
#define DCHECK_IS_ON() 1
#else
#define DCHECK_IS_ON() 0
#endif

#define DCHECK(condition)                                                                                              \
  LAZY_STREAM(LOG_STREAM(DCheck), DCHECK_IS_ON() ? !(condition) : false) << "Check failed: " #condition ". "

// NOTREACHED

#define NOTREACHED() DCHECK(false)

#endif  // NUCLEUS_LOGGING_H_
