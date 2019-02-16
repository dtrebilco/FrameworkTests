#pragma once
#include <string>

namespace profiler
{
  void Register();

  void Begin();
  void End(std::string& o_outString);
  void EndFileJson(const char* i_fileName);
  void EndFileHtml(const char* i_fileName);
}
