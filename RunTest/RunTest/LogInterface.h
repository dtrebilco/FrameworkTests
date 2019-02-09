#pragma once

// What level of logging to use 0 = none, 3 = full
#define LOG_LEVEL 3
#define PROFILE_ENABLE 1

#if LOG_LEVEL > 0

  typedef void (LogCallback)(int i_level, const char* i_str);
  void SetLogCallback(LogCallback* i_logCallback);
  void LogMsg(int i_level, const char* i_str);
  
#ifdef _MSC_VER
  #include <sal.h>
  void LogMsgFmt(int i_level, _Printf_format_string_ const char* i_str, ...);
#else
  void LogMsgFmt(int i_level, const char* i_str, ...) __attribute__((format(printf, 2, 3)));;
#endif

#endif

#if LOG_LEVEL > 0
  #define LOGERR(x) LogMsg(1, x)
  #define LOGERR_FMT(x,...) LogMsgFmt(1, x, ...)
#else
  #define LOGERR(x)
  #define LOGERR_FMT(x)
#endif

#if LOG_LEVEL > 1
#define LOGWARN(x) LogMsg(2, x)
#define LOGWARN_FMT(x,...) LogMsgFmt(2, x, ...)
#else
#define LOGWARN(x)
#define LOGWARN_FMT(x)
#endif

#if LOG_LEVEL > 2
#define LOGINFO(x) LogMsg(2, x)
#define LOGINFO_FMT(x,...) LogMsgFmt(2, x, ...)
#else
#define LOGINFO(x)
#define LOGINFO_FMT(x)
#endif



