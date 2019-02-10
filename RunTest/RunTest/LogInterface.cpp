#include "LogInterface.h"
#include <stdio.h>
#include <stdarg.h>

#if LOG_LEVEL > 0

namespace logger_interface
{

static void DummyLog(int i_level, const char* i_str)
{
}
static LogCallback* g_logCallback = &DummyLog;

void SetLogCallback(LogCallback* i_logCallback)
{
  if (i_logCallback == nullptr)
  {
    g_logCallback = &DummyLog;
  }
  else
  {
    g_logCallback = i_logCallback;
  }
}

void LogMsg(int i_level, const char* i_str)
{
  g_logCallback(i_level, i_str);
}

void LogMsgFmt(int i_level, const char* i_str, ...)
{
  va_list marker;
  va_start(marker, i_str);

  //Write the data to a buffer
  char buf[1024];
  buf[0] = 0;
  vsnprintf(&buf[0], sizeof(buf) - 1, i_str, marker);
  
  // Force a null terminator to be safe from bad implementations
  buf[sizeof(buf) - 1] = 0;

  g_logCallback(i_level, &buf[0]);

  va_end(marker);
}

} // namespace logger_interface
#endif // LOG_LEVEL > 0


#if PROFILE_ENABLE > 0
namespace profile_interface
{

static void DummyBegin(const char* i_str)
{
}

static void DummyEnd()
{
}

static ProfileBeginCallback* g_profileBeginCallback = &DummyBegin;
static ProfileEndCallback* g_profileEndCallback = &DummyEnd;


void SetProfileCallback(ProfileBeginCallback* i_beginCallback, ProfileEndCallback* i_endCallback)
{
  if (i_beginCallback == nullptr)
  {
    g_profileBeginCallback = &DummyBegin;
  }
  else
  {
    g_profileBeginCallback = i_beginCallback;
  }

  if (i_endCallback == nullptr)
  {
    g_profileEndCallback = &DummyEnd;
  }
  else
  {
    g_profileEndCallback = i_endCallback;
  }
}

void ProfileBegin(const char* i_str)
{
  g_profileBeginCallback(i_str);
}

void ProfileEnd()
{
  g_profileEndCallback();
}

} // namespace profile_interface
#endif // PROFILE_ENABLE > 0

