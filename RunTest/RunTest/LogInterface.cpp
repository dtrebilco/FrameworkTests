#include "LogInterface.h"
#include <stdio.h>
#include <stdarg.h>

static void DummyLog(int i_level, const char* i_str)
{
}
LogCallback* g_logCallback = &DummyLog;


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
  vsnprintf(buf, sizeof(buf), i_str, marker);
  
  // Force a null terminator to be safe
  buf[sizeof(buf) - 1] = 0;

  g_logCallback(i_level, buf);

  va_end(marker);
}

