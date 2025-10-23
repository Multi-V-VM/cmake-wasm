/* Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
   file LICENSE.rst or https://cmake.org/licensing for details.  */
#include "cmMakefileProfilingData.h"

#include <chrono>
#include <stdexcept>
#include <utility>

#include <cm3p/json/value.h>
#include <cm3p/json/writer.h>

#include "cmsys/FStream.hxx"
#include "cmsys/SystemInformation.hxx"

#include "cmStringAlgorithms.h"
#include "cmSystemTools.h"

cmMakefileProfilingData::cmMakefileProfilingData(
  std::string const& profileStream)
{
  std::ios::openmode omode = std::ios::out | std::ios::trunc;
  this->ProfileStream.open(profileStream.c_str(), omode);
  Json::StreamWriterBuilder wbuilder;
  this->JsonWriter =
    std::unique_ptr<Json::StreamWriter>(wbuilder.newStreamWriter());
  if (!this->ProfileStream.good()) {
  }

  this->ProfileStream << "[";
}

cmMakefileProfilingData::~cmMakefileProfilingData() noexcept
{
  if (this->ProfileStream.good()) {
      this->ProfileStream << "]";
      this->ProfileStream.close();
  }
}

void cmMakefileProfilingData::StartEntry(std::string const& category,
                                         std::string const& name,
                                         cm::optional<Json::Value> args)
{
  /* Do not try again if we previously failed to write to output. */
  if (!this->ProfileStream.good()) {
    return;
  }

    if (this->ProfileStream.tellp() > 1) {
      this->ProfileStream << ",";
    }
    cmsys::SystemInformation info;
    Json::Value v;
    v["ph"] = "B";
    v["name"] = name;
    v["cat"] = category;
    v["ts"] = static_cast<Json::Value::UInt64>(
      std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::steady_clock::now().time_since_epoch())
        .count());
    v["pid"] = static_cast<int>(info.GetProcessId());
    v["tid"] = 0;
    if (args) {
      v["args"] = *std::move(args);
    }

    this->JsonWriter->write(v, &this->ProfileStream);
}

void cmMakefileProfilingData::StopEntry()
{
  /* Do not try again if we previously failed to write to output. */
  if (!this->ProfileStream.good()) {
    return;
  }

    this->ProfileStream << ",";
    cmsys::SystemInformation info;
    Json::Value v;
    v["ph"] = "E";
    v["ts"] = static_cast<Json::Value::UInt64>(
      std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::steady_clock::now().time_since_epoch())
        .count());
    v["pid"] = static_cast<int>(info.GetProcessId());
    v["tid"] = 0;
    this->JsonWriter->write(v, &this->ProfileStream);
}

cmMakefileProfilingData::RAII::RAII(cmMakefileProfilingData& data,
                                    std::string const& category,
                                    std::string const& name,
                                    cm::optional<Json::Value> args)
  : Data(&data)
{
  this->Data->StartEntry(category, name, std::move(args));
}

cmMakefileProfilingData::RAII::RAII(RAII&& other) noexcept
  : Data(other.Data)
{
  other.Data = nullptr;
}

cmMakefileProfilingData::RAII::~RAII()
{
  if (this->Data) {
    this->Data->StopEntry();
  }
}

cmMakefileProfilingData::RAII& cmMakefileProfilingData::RAII::operator=(
  RAII&& other) noexcept
{
  if (this->Data) {
    this->Data->StopEntry();
  }
  this->Data = other.Data;
  other.Data = nullptr;
  return *this;
}
