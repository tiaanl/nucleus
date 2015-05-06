// Copyright (c) 2015, Tiaan Louw
//
// Permission to use, copy, modify, and/or distribute this software for any
// purpose with or without fee is hereby granted, provided that the above
// copyright notice and this permission notice appear in all copies.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
// REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
// AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
// INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
// LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
// OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
// PERFORMANCE OF THIS SOFTWARE.

#include <gtest/gtest.h>

#include <cstdarg>
#include <cstdio>
#include <string>

#include "nucleus/utils/signals.h"

namespace {

static std::string accu;

static char floatCallback(float f, int, std::string) {
  std::stringstream ss;
  ss.setf(std::ios::fixed, std:: ios::floatfield);
  ss.precision(2);
  ss << "float: " << f << "\n";
  accu.append(ss.str());
  return 0;
}

struct Foo {
  char fooBool(float f, int i, std::string s) {
    std::stringstream ss;
    ss.setf(std::ios::fixed, std:: ios::floatfield);
    ss.precision(2);
    ss << "Foo: " << f + i + s.size() << "\n";
    accu.append(ss.str());
    return true;
  }
};

}  // namespace

namespace nu {

TEST(SignalsTest, Basic) {
  accu.clear();

  Signal<char(float, int, std::string)> sig1;

  size_t id1 = sig1.connect(floatCallback);

  size_t id2 = sig1.connect([](float, int i, std::string) {
    std::stringstream ss;
    ss << "int: " << i << "\n";
    accu.append(ss.str());
    return 0;
  });

  size_t id3 = sig1.connect([](float, int, const std::string& s) {
    std::stringstream ss;
    ss << "string: " << s << "\n";
    accu.append(ss.str());
    return 0;
  });

  sig1.emit(0.3f, 4, "huhu");

  ASSERT_TRUE(sig1.disconnect(id1));
  ASSERT_FALSE(sig1.disconnect(id1));
  ASSERT_TRUE(sig1.disconnect(id2));
  ASSERT_TRUE(sig1.disconnect(id3));
  ASSERT_FALSE(sig1.disconnect(id3));
  ASSERT_FALSE(sig1.disconnect(id2));

  Foo foo;
  sig1.connect(slot(&Foo::fooBool, foo));
  sig1.connect(slot(&Foo::fooBool, &foo));
  sig1.emit(0.5f, 1, "12");

  Signal<void(std::string, int)> sig2;

  sig2.connect([](std::string msg, int) {
    std::stringstream ss;
    ss << "msg: " << msg;
    accu.append(ss.str());
  });

  sig2.connect([](std::string, int d) {
    std::stringstream ss;
    ss << " *" << d << "*\n";
    accu.append(ss.str());
  });

  sig2.emit("in sig2", 17);

  accu.append("DONE");

  const char* expected = 
        "float: 0.30\n"
        "int: 4\n"
        "string: huhu\n"
        "Foo: 3.50\n"
        "Foo: 3.50\n"
        "msg: in sig2 *17*\n"
        "DONE";

  EXPECT_STREQ(expected, accu.c_str());
}

}  // namespace nu
