#pragma once

/// \brief This file contains the definitions of logging and debugging interfaces.
///        The actual implementations of the logging must be provided to the interface as a callback function.
///        Setup is called by the LOGSETUP() macro with an appropriate callback.
///        Logging is called via LOGERR/LOGWARN/LOGINFO for basic messages, and LOGERR_FMT/LOGWARN_FMT/LOGINFO_FMT for formatted logging messages

// What level of logging to use 0 = none, 3 = full
#define LOG_LEVEL 3
#define PROFILE_ENABLE 1


#if LOG_LEVEL > 0
namespace logger_interface
{
  /// \brief The callback function used in logging. Must be multi-thread safe
  typedef void (LogCallback)(int i_level, const char* i_str);

  /// \brief Set the callback function used in logging
  void SetLogCallback(LogCallback* i_logCallback);
  void LogMsg(int i_level, const char* i_str);

  #ifdef _MSC_VER
    #include <sal.h>
    void LogMsgFmt(int i_level, _Printf_format_string_ const char* i_str, ...);
  #else
    void LogMsgFmt(int i_level, const char* i_str, ...) __attribute__((format(printf, 2, 3)));;
  #endif
}
#endif

#if LOG_LEVEL > 0
  #define LOGSETUP(x) logger_interface::SetLogCallback(x)
  #define LOGERR(x) logger_interface::LogMsg(1, x)
  #define LOGERR_FMT(x,...) logger_interface::LogMsgFmt(1, x, __VA_ARGS__)
#else
  #define LOGSETUP(x)
  #define LOGERR(x)
  #define LOGERR_FMT(...)
#endif

#if LOG_LEVEL > 1
#define LOGWARN(x) logger_interface::LogMsg(2, x)
#define LOGWARN_FMT(x,...) logger_interface::LogMsgFmt(2, x, __VA_ARGS__)
#else
#define LOGWARN(x)
#define LOGWARN_FMT(...)
#endif

#if LOG_LEVEL > 2
#define LOGINFO(x) logger_interface::LogMsg(2, x)
#define LOGINFO_FMT(x,...) logger_interface::LogMsgFmt(2, x, __VA_ARGS__)
#else
#define LOGINFO(x)
#define LOGINFO_FMT(...)
#endif



