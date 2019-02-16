#include "Profiler.h"
#include "LogInterface.h"


static void ProfileBeginCallback(const char* i_str)
{

}

static void ProfileEndCallback()
{

}

namespace profiler
{

void Register()
{
  PROFILE_SETUP(ProfileBeginCallback, ProfileEndCallback);
}

void Begin()
{

}

void End(std::string& o_outString)
{

}

void EndFileJson(const char* i_fileName)
{

}

void EndFileHtml(const char* i_fileName)
{

}

}
