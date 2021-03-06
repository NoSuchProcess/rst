// Copyright (c) 2016, Sergey Abbakumov
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
// 1. Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef RST_LOGGER_LOGGER_H_
#define RST_LOGGER_LOGGER_H_

#include <memory>

#include "rst/Noncopyable/Noncopyable.h"

#define LOG_DEBUG(...)                                                 \
  ::rst::Logger::Log(::rst::Logger::Level::kDebug, __FILE__, __LINE__, \
                     __VA_ARGS__)

#define LOG_INFO(...)                                                 \
  ::rst::Logger::Log(::rst::Logger::Level::kInfo, __FILE__, __LINE__, \
                     __VA_ARGS__)

#define LOG_WARNING(...)                                                 \
  ::rst::Logger::Log(::rst::Logger::Level::kWarning, __FILE__, __LINE__, \
                     __VA_ARGS__)

#define LOG_ERROR(...)                                                 \
  ::rst::Logger::Log(::rst::Logger::Level::kError, __FILE__, __LINE__, \
                     __VA_ARGS__)

#define LOG_FATAL(...)                                                 \
  ::rst::Logger::Log(::rst::Logger::Level::kFatal, __FILE__, __LINE__, \
                     __VA_ARGS__)

#ifndef NDEBUG

#define DLOG_DEBUG(...)                                                \
  ::rst::Logger::Log(::rst::Logger::Level::kDebug, __FILE__, __LINE__, \
                     __VA_ARGS__)

#define DLOG_INFO(...)                                                \
  ::rst::Logger::Log(::rst::Logger::Level::kInfo, __FILE__, __LINE__, \
                     __VA_ARGS__)

#define DLOG_WARNING(...)                                                \
  ::rst::Logger::Log(::rst::Logger::Level::kWarning, __FILE__, __LINE__, \
                     __VA_ARGS__)

#define DLOG_ERROR(...)                                                \
  ::rst::Logger::Log(::rst::Logger::Level::kError, __FILE__, __LINE__, \
                     __VA_ARGS__)

#define DLOG_FATAL(...)                                                \
  ::rst::Logger::Log(::rst::Logger::Level::kFatal, __FILE__, __LINE__, \
                     __VA_ARGS__)

#else  // NDEBUG

#define DLOG_DEBUG(...)
#define DLOG_INFO(...)
#define DLOG_WARNING(...)
#define DLOG_ERROR(...)
#define DLOG_FATAL(...)

#endif  // NDEBUG

namespace rst {

class ISink;

// The class for logging to a custom sink.
class Logger : public NonCopyable, public NonMovable {
 public:
  // Severity levels of logging.
  enum class Level { kAll = 0, kDebug, kInfo, kWarning, kError, kFatal, kOff };

  explicit Logger(std::unique_ptr<ISink> sink);

  // Logs a message in printf-like format. If the level is less than level_
  // nothing gets logged.
  static void Log(Level level, const char* filename, int line,
                  const char* format, ...);
  static void SetLogger(Logger* logger);

  void set_level(Level level) { level_ = level; }

 private:
  Level level_ = Level::kAll;

  std::unique_ptr<ISink> sink_;
};

}  // namespace rst

#endif  // RST_LOGGER_LOGGER_H_
