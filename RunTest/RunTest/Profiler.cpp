#include "Profiler.h"
#include "LogInterface.h"

#include <thread>
#include <atomic>
#include <mutex>
#include <vector>
#include <chrono>
#include <unordered_map>
#include <sstream>
#include <fstream>
#include <memory>
#include <ctime>

namespace profiler
{
using clock = std::chrono::high_resolution_clock;

struct ProfileRecord
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

struct ProfileData
{
  std::atomic_bool m_enabled = false;
  std::mutex m_access;
  clock::time_point m_startTime;
  std::vector<ProfileRecord> m_records;
  size_t m_maxRecords = 0;
};
static std::unique_ptr<ProfileData> g_pData;


static void ProfileBeginCallback(const char* i_str)
{
  if (!g_pData->m_enabled)
  {
    return;
  }

  // Ensure no null pointers on begin
  if (i_str == nullptr)
  {
    i_str = "Unknown";
  }

  // Create the profile record
  ProfileRecord newData = {};
  newData.m_threadID = std::this_thread::get_id();
  newData.m_tag = i_str;

  std::lock_guard<std::mutex> lock(g_pData->m_access);
  if (g_pData->m_records.size() < g_pData->m_maxRecords)
  {
    g_pData->m_records.push_back(newData);

    // Assign the time as the last possible thing
    g_pData->m_records.back().m_time = clock::now();
  }
}

static void ProfileEndCallback()
{
  if (!g_pData->m_enabled)
  {
    return;
  }

  ProfileRecord newData = {};
  newData.m_time = clock::now(); // Always get time as soon as possible
  newData.m_threadID = std::this_thread::get_id();
  newData.m_tag = nullptr;

  std::lock_guard<std::mutex> lock(g_pData->m_access);
  if (g_pData->m_records.size() < g_pData->m_maxRecords)
  {
    g_pData->m_records.push_back(newData);
  }
}

void Register()
{
  if (!g_pData)
  {
    g_pData = std::make_unique<ProfileData>();
    PROFILE_SETUP(ProfileBeginCallback, ProfileEndCallback);
  }
}

bool Begin(size_t i_bufferSize)
{
  if (!g_pData)
  {
    return false;
  }

  std::lock_guard<std::mutex> lock(g_pData->m_access);
  if (g_pData->m_enabled)
  {
    return false;
  }

  // Clear all data (may have been some extra in buffers from previous enable)
  g_pData->m_maxRecords = i_bufferSize / sizeof(ProfileRecord);
  g_pData->m_records.resize(0);
  g_pData->m_records.reserve(g_pData->m_maxRecords);
  g_pData->m_startTime = clock::now();

  g_pData->m_enabled = true;
  return true;
}

bool End(std::ostream& o_outStream)
{
  if (!g_pData)
  {
    return false;
  }

  std::lock_guard<std::mutex> lock(g_pData->m_access);
  if (!g_pData->m_enabled)
  {
    return false;
  }
  g_pData->m_enabled = false;

  // Init this thread as the primary thread
  std::unordered_map<std::thread::id, Tags> threadStack;
  threadStack[std::this_thread::get_id()].m_index = 0;

  bool first = true;
  int32_t threadCounter = 0;

  std::string cleanTag;
  o_outStream << "{\"traceEvents\": [\n";
  for (const ProfileRecord& entry : g_pData->m_records)
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

    // Markup invalid json characters
    if (strchr(tag, '"') != nullptr ||
        strchr(tag, '\\') != nullptr)
    {
      cleanTag = tag;
      size_t startPos = 0;
      while ((startPos = cleanTag.find_first_of("\\\"", startPos)) != std::string::npos)
      {
        cleanTag.insert(startPos, 1, '\\');
        startPos += 2;
      }
      tag = cleanTag.c_str();
    }

    // Get the microsecond count
    long long msCount = std::chrono::duration_cast<std::chrono::microseconds>(entry.m_time - g_pData->m_startTime).count();

    if (!first)
    {
      o_outStream << ",\n";
    }
    first = false;

    char msString[64];
    snprintf(msString, sizeof(msString), "%lld", msCount);

    char indexString[64];
    snprintf(indexString, sizeof(indexString), "%d", stack.m_index);

    // Format the string
    o_outStream <<
      "{\"name\": \"" << tag <<
      "\", \"ph\": \"" << typeTag << 
      "\", \"ts\":" << msString << 
      ", \"tid\":" << indexString << 
      ", \"cat\":\"\", \"pid\" : 0, \"args\" : {} }";
  }
  o_outStream << "\n]\n}\n";
  return true;
}

bool End(std::string& o_outString)
{
  std::stringstream ss;
  bool retval = End(ss);
  o_outString = ss.str();
  return retval;
}

bool EndFileJson(const char* i_fileName, bool i_appendDate)
{
  std::ofstream file;
  if (i_appendDate)
  {
    // Create a filename with the current date in it
    std::time_t t = std::time(nullptr);
    tm timeBuf;
    localtime_s(&timeBuf, &t);
    char timeStr[100];
    if (std::strftime(timeStr, sizeof(timeStr), "%Y%m%d-%H%M%S", &timeBuf) == 0)
    {
      return false;
    }

    char newFilename[1024];
    snprintf(newFilename, sizeof(newFilename), "%s_%s.json", i_fileName, timeStr);
    file.open(newFilename);
  }
  else
  {
    file.open(i_fileName);
  }

  if (!file.is_open())
  {
    return false;
  }
  return End(file);
}

}
