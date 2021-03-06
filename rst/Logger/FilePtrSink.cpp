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

#include "rst/Logger/FilePtrSink.h"

#include "rst/Check/Check.h"

using std::FILE;
using std::mutex;
using std::string;
using std::unique_lock;

namespace rst {

FilePtrSink::FilePtrSink(FILE* file, string prologue_format, bool should_close)
    : prologue_format_(std::move(prologue_format)) {
  RST_DCHECK(file != nullptr);

  if (should_close) {
    log_file_.reset(file);
    file_ = log_file_.get();
  } else {
    non_closing_log_file_.reset(file);
    file_ = non_closing_log_file_.get();
  }
}

void FilePtrSink::Log(const char* filename, int line,
                      const char* severity_level, const char* format,
                      va_list args) {
  RST_DCHECK(filename != nullptr);
  RST_DCHECK(line > 0);
  RST_DCHECK(severity_level != nullptr);
  RST_DCHECK(format != nullptr);

  unique_lock<mutex> lock(mutex_);

  auto val = std::fprintf(file_, prologue_format_.c_str(), filename, line,
                          severity_level);
  RST_CHECK(val >= 0);

  val = std::vfprintf(file_, format, args);
  RST_CHECK(val >= 0);

  val = std::fprintf(file_, "\n");
  RST_CHECK(val >= 0);

  val = std::fflush(file_);
  RST_CHECK(val >= 0);
}

}  // namespace rst
