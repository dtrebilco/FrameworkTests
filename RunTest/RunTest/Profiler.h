#pragma once
#include <string>
#include <ostream>

// See:  http://www.gamasutra.com/view/news/176420/Indepth_Using_Chrometracing_to_view_your_inline_profiling_data.php
//       https://aras-p.info/blog/2017/01/23/Chrome-Tracing-as-Profiler-Frontend/
//       https://github.com/mainroach/sandbox/tree/master/chrome-event-trace
//       https://github.com/catapult-project/catapult
//       https://src.chromium.org/viewvc/chrome/trunk/src/base/debug/trace_event.h?view=markup

  //https://akrzemi1.wordpress.com/2011/05/11/parsing-strings-at-compile-time-part-i/
  //https://www.codeproject.com/Tips/1208152/How-to-Check-If-a-String-is-Literal-in-Compile-tim

// limitations - no string copying, no proper thread ids, no thread names
namespace profiler
{
  // Must be called before any other profile threads are active
  void Register();

  bool Begin(size_t i_bufferSize = 10000000);
  bool End(std::ostream& o_outStream);
  bool End(std::string& o_outString);
  bool EndFileJson(const char* i_fileName, bool i_appendDate = true);
}
