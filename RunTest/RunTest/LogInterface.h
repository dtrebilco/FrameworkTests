#pragma once

/// \brief This file contains the definitions of logging and profiling interfaces.
///  Logging
///        The actual implementations of the logging must be provided to the interface as a callback function.
///        Setup is called by the LOG_SETUP() macro with an appropriate callback.
///        Logging is called via LOG_ERR/LOG_WARN/LOG_INFO for basic messages, and LOG_ERR_FMT/LOG_WARN_FMT/LOG_INFO_FMT for formatted logging messages
///  Profiling
///        The actual implementations of the profiling must be provided to the interface as callback functions.
///        Setup is called by the PROFILE_SETUP() macro with an appropriate callbacks.
///        Profiling is done via PROFILE_BEGIN/PROFILE_END pair calls or a PROFILE_SCOPE call.

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
  #define LOG_SETUP(x) logger_interface::SetLogCallback(x)
  #define LOG_ERR(x) logger_interface::LogMsg(1, x)
  #define LOG_ERR_FMT(x,...) logger_interface::LogMsgFmt(1, x, __VA_ARGS__)
#else
  #define LOG_SETUP(x)
  #define LOG_ERR(x)
  #define LOG_ERR_FMT(...)
#endif

#if LOG_LEVEL > 1
#define LOG_WARN(x) logger_interface::LogMsg(2, x)
#define LOG_WARN_FMT(x,...) logger_interface::LogMsgFmt(2, x, __VA_ARGS__)
#else
#define LOG_WARN(x)
#define LOG_WARN_FMT(...)
#endif

#if LOG_LEVEL > 2
#define LOG_INFO(x) logger_interface::LogMsg(2, x)
#define LOG_INFO_FMT(x,...) logger_interface::LogMsgFmt(2, x, __VA_ARGS__)
#else
#define LOG_INFO(x)
#define LOG_INFO_FMT(...)
#endif

#if PROFILE_ENABLE > 0
namespace profile_interface
{
  typedef void (ProfileBeginCallback)(const char* i_str);
  typedef void (ProfileEndCallback)();

  /// \brief Set the callback functions used in profiling
  void SetProfileCallback(ProfileBeginCallback* i_beginCallback, ProfileEndCallback* i_endCallback);
  void ProfileBegin(const char* i_str);
  void ProfileEnd();

  struct ProfileScope
  {
    ProfileScope(const char * i_str)
    {
      ProfileBegin(i_str);
    }
    ~ProfileScope()
    {
      ProfileEnd();
    }
  };
}

#define PROFILE_SETUP(x,y) profile_interface::SetProfileCallback(x,y)
#define PROFILE_BEGIN(x) profile_interface::ProfileBegin(x)
#define PROFILE_END() profile_interface::ProfileEnd()
#define PROFILE_SCOPE(x) ProfileScope p##__LINE__(x)

#else

#define PROFILE_SETUP(x,y)
#define PROFILE_BEGIN(x)
#define PROFILE_END()
#define PROFILE_SCOPE(x)

#endif
