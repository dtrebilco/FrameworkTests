#include "Profiler.h"
#include "LogInterface.h"

#include <thread>
#include <atomic>
#include <mutex>
#include <vector>
#include <chrono>

namespace profiler
{
using clock = std::chrono::high_resolution_clock;

struct ProfileData
{
  clock::time_point m_time; // The time of the profile data
  std::thread::id m_threadID;  // The id of the thread
  const char* m_tag = nullptr; // The tag used in profiling - if empty is an end event
};

static std::atomic_bool g_profileEnabled = false;
static std::mutex g_profileAccess;
static std::vector<ProfileData> g_profileData;

static void ProfileBeginCallback(const char* i_str)
{
  if (!g_profileEnabled)
  {
    return;
  }

  // Ensure no null pointers on begin
  if (i_str == nullptr)
  {
    i_str = "Unknown";
  }

  ProfileData newData = {};
  newData.m_threadID = std::this_thread::get_id();
  newData.m_tag = i_str;

  std::lock_guard<std::mutex> lock(g_profileAccess);
  g_profileData.push_back(newData);
  
  // Assign the time as the last possible thing
  g_profileData.back().m_time = clock::now();
}

static void ProfileEndCallback()
{
  if (!g_profileEnabled)
  {
    return;
  }

  ProfileData newData = {};
  newData.m_time = clock::now(); // Always get time as soon as possible
  newData.m_threadID = std::this_thread::get_id();
  newData.m_tag = nullptr;

  std::lock_guard<std::mutex> lock(g_profileAccess);
  g_profileData.push_back(newData);
}

void Register()
{
  PROFILE_SETUP(ProfileBeginCallback, ProfileEndCallback);
}

void Begin()
{
  std::lock_guard<std::mutex> lock(g_profileAccess);
  if (g_profileEnabled)
  {
    return;
  }

  // Clear all data (may have been some extra in buffers from previous enable)
  g_profileData.resize(0);
  g_profileData.reserve(10000);

  g_profileEnabled = true;
}

void End(std::string& o_outString)
{
  std::lock_guard<std::mutex> lock(g_profileAccess);
  if (!g_profileEnabled)
  {
    return;
  }

  g_profileEnabled = false;
}

void EndFileJson(const char* i_fileName)
{
  std::lock_guard<std::mutex> lock(g_profileAccess);
  if (!g_profileEnabled)
  {
    return;
  }

  g_profileEnabled = false;
}

void EndFileHtml(const char* i_fileName)
{
  std::lock_guard<std::mutex> lock(g_profileAccess);
  if (!g_profileEnabled)
  {
    return;
  }

  g_profileEnabled = false;
}

}
