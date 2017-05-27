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

#ifndef RST_LOGGER_FILENAMESINK_H_
#define RST_LOGGER_FILENAMESINK_H_

#include <cstdio>
#include <memory>
#include <mutex>
#include <string>

#include "rst/Logger/ISink.h"

namespace rst {

// The class for sinking to a file by its filename.
class FileNameSink : public ISink {
 public:
  // Opens a filename for writing.
  FileNameSink(const std::string& filename, std::string prologue_format);
  // Thread safe logging function.
  void Log(const char* filename, int line, const char* severity_level,
           const char* format, va_list args) override;

 private:
  // A RAII-wrapper around std::FILE.
  std::unique_ptr<std::FILE, void (*)(std::FILE*)> log_file_{nullptr,
                                                             [](std::FILE* f) {
                                                               if (f != nullptr)
                                                                 std::fclose(f);
                                                             }};

  // Prologue printf-like format for filename, line in a file and severity
  // level.
  std::string prologue_format_;

  // Mutex for thread-safe Log function.
  std::mutex mutex_;
};

}  // namespace rst

#endif  // RST_LOGGER_FILENAMESINK_H_
