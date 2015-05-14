#ifndef Z_ASSERT_H_
#define Z_ASSERT_H_

#include "Core.h"

void Assert (bool condition, const char* file, int line, const char* format, ...);

#define assertion(condition, format, ...) \
    Assert(condition, __FILE__, __LINE__, format, ##__VA_ARGS__)

#endif
