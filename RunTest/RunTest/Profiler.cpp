#include "Profiler.h"
#include "LogInterface.h"

#include <thread>
#include <atomic>
#include <mutex>
#include <vector>
#include <chrono>
#include <unordered_map>
#include <sstream>

namespace profiler
{
using clock = std::chrono::high_resolution_clock;

struct ProfileData
{
  clock::time_point m_time; // The time of the profile data
  std::thread::id m_threadID;  // The id of the thread
  const char* m_tag = nullptr; // The tag used in profiling - if empty is an end event
};

struct Tags
{
  int32_t m_index = -1;
  std::vector<const char*> m_tags;
};

static std::atomic_bool g_profileEnabled = false;
static std::mutex g_profileAccess;
static clock::time_point g_startTime;
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
  g_startTime = clock::now();

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

  // Init this thread as the primary thread
  std::unordered_map<std::thread::id, Tags> threadStack;
  threadStack[std::this_thread::get_id()].m_index = 0;

  bool first = true;
  int32_t threadCounter = 0;
  std::stringstream ss;
  ss << "{\"traceEvents\": [\n";
  for (const ProfileData& entry : g_profileData)
  {
    auto& stack = threadStack[entry.m_threadID];
    
    // Assign a unique index to each thread
    if (stack.m_index < 0)
    {
      stack.m_index = threadCounter;
      threadCounter++;
    }

    // Get the name tags
    const char* tag = entry.m_tag;
    const char* typeTag = "B";
    if (tag != nullptr)
    {
      stack.m_tags.push_back(tag);
    }
    else
    {
      typeTag = "E";
      if (stack.m_tags.size() == 0)
      {
        tag = "Unknown";
      }
      else
      {
        tag = stack.m_tags.back();
        stack.m_tags.pop_back();
      }
    }

    // Get the microsecond count
    auto msCount = std::chrono::duration_cast<std::chrono::microseconds>(entry.m_time - g_startTime).count();

    if (!first)
    {
      ss << ",\n";
    }
    first = false;

    // Format the string
    ss << "{\"name\": \"" << tag << 
          "\", \"ph\": \"" << typeTag << 
          "\", \"ts\":" << msCount << 
          ", \"tid\":" << stack.m_index << 
          ", \"cat\":\"\", \"pid\" : 0, \"args\" : {} }";
  }
  ss << "\n]\n}\n";

  o_outString = ss.str();
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
