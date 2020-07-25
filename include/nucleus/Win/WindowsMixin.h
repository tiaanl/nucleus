
#ifndef NUCLEUS_WIN_WINDOWS_MIXIN_H_
#define NUCLEUS_WIN_WINDOWS_MIXIN_H_

#include "nucleus/Config.h"

#if OS(WIN)

#define WIN32_LEAN_AND_MEAN 1

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#endif  // OS(WIN)

#endif  // NUCLEUS_WIN_WINDOWS_MIXIN_H_
