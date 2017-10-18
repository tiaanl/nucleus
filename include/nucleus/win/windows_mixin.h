
#ifndef NUCLEUS_WIN_WINDOWS_MIXIN_H_
#define NUCLEUS_WIN_WINDOWS_MIXIN_H_

#include "nucleus/config.h"

#if OS(WIN)

#define WIN32_LEAN_AND_MEAN
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>

#endif  // OS(WIN)

#endif  // NUCLEUS_WIN_WINDOWS_MIXIN_H_
