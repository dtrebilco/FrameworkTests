#pragma once
#include <string>

// See:  http://www.gamasutra.com/view/news/176420/Indepth_Using_Chrometracing_to_view_your_inline_profiling_data.php
//       https://aras-p.info/blog/2017/01/23/Chrome-Tracing-as-Profiler-Frontend/
//       https://github.com/mainroach/sandbox/tree/master/chrome-event-trace
//       https://github.com/catapult-project/catapult
//       https://src.chromium.org/viewvc/chrome/trunk/src/base/debug/trace_event.h?view=markup

  //https://akrzemi1.wordpress.com/2011/05/11/parsing-strings-at-compile-time-part-i/
  //https://www.codeproject.com/Tips/1208152/How-to-Check-If-a-String-is-Literal-in-Compile-tim


namespace profiler
{
  void Register();

  void Begin();
  void End(std::string& o_outString);
  void EndFileJson(const char* i_fileName);
  void EndFileHtml(const char* i_fileName);
}
