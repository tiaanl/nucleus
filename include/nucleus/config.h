#pragma once

// BUILD()

#define BUILD(Feature) (BUILD_##Feature)

#if defined(NDEBUG)
#define BUILD_RELEASE 1
#else
#define BUILD_DEBUG 1
#endif

// OS()

#define OS(Feature) (OS_##Feature)

#if defined(__APPLE__)
#define OS_MACOSX 1
#if defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE
#define OS_IOS 1
#endif
#elif defined(ANDROID)
#define OS_ANDROID 1
#elif defined(__linux__)
#define OS_LINUX 1
#elif defined(_WIN32)
#define OS_WIN 1
#elif defined(__FreeBSD__)
#define OS_FREEBSD 1
#elif defined(__OpenBSD__)
#define OS_OPENBSD 1
#elif defined(__sun)
#define OS_SOLARIS 1
#else
#error Please add support for your platform in nucleus/config.h
#endif

// For access to standard BSD features, use OS_BSD instead of a
// more specific macro.
#if OS(FREEBSD) || OS(OPENBSD)
#define OS_BSD 1
#endif

// For access to standard POSIX features, use OS_POSIX instead of a
// more specific macro.
#if OS(MACOSX) || OS(LINUX) || OS(FREEBSD) || OS(OPENBSD) || OS(SOLARIS) || OS(ANDROID)
#define OS_POSIX 1
#endif

#if OS(POSIX) && !OS(MACOSX) && !OS(ANDROID) && !OS(NACL)
#define USE_X11 1  // Use X for graphics.
#endif

// COMPILER()

#define COMPILER(Feature) (COMPILER_##Feature)

#if defined(__GNUC__)
#define COMPILER_GCC 1
// If we're using GCC on windows, it's MingW.
#if OS(WIN)
#define COMPILER_MINGW 1
#endif
#elif defined(_MSC_VER)
#define COMPILER_MSVC 1
#else
#error Please add support for your compiler in nucleus/config.h
#endif

// ARCH()

#define ARCH(Feature) (ARCH_##Feature)

#if defined(_M_X64) || defined(__x86_64__)
#define ARCH_CPU_X86_FAMILY 1
#define ARCH_CPU_X86_64 1
#define ARCH_CPU_64_BITS 1
#define ARCH_CPU_LITTLE_ENDIAN 1
#elif defined(_M_IX86) || defined(__i386__)
#define ARCH_CPU_X86_FAMILY 1
#define ARCH_CPU_X86 1
#define ARCH_CPU_32_BITS 1
#define ARCH_CPU_LITTLE_ENDIAN 1
#elif defined(__ARMEL__)
#define ARCH_CPU_ARM_FAMILY 1
#define ARCH_CPU_ARMEL 1
#define ARCH_CPU_32_BITS 1
#define ARCH_CPU_LITTLE_ENDIAN 1
#elif defined(__MIPSEL__)
#define ARCH_CPU_MIPS_FAMILY 1
#define ARCH_CPU_MIPSEL 1
#define ARCH_CPU_32_BITS 1
#define ARCH_CPU_LITTLE_ENDIAN 1
#elif defined(__ppc64__)
#define ARCH_CPU_PPC_FAMILY 1
#define ARCH_CPU_PPC_64 1
#define ARCH_CPU_64_BITS 1
#define ARCH_CPU_BIG_ENDIAN 1
#elif defined(__ppc__)
#define ARCH_CPU_PPC_FAMILY 1
#define ARCH_CPU_PPC 1
#define ARCH_CPU_32_BITS 1
#define ARCH_CPU_BIG_ENDIAN 1
#else
#error Please add support for your architecture in nucleus/config.h
#endif
