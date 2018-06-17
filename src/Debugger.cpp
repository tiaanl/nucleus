
#include "nucleus/Debugger.h"

#include "nucleus/Config.h"

#if OS(WIN32)
#include "nucleus/Win/WindowsMixin.h"
#endif

namespace nu {

void breakDebugger() {
#if OS(WIN)
  ::__debugbreak();
#elif OS(POSIX)
#if ARCH(CPU_ARMEL)
#define DEBUG_BREAK_ASM() asm("bkpt 0")
#elif ARCH(CPU_ARM64)
#define DEBUG_BREAK_ASM() asm("brk 0")
#elif ARCH(CPU_MIPS_FAMILY)
#define DEBUG_BREAK_ASM() asm("break 2")
#elif ARCH(CPU_X86_FAMILY)
#define DEBUG_BREAK_ASM() asm("int3")
#endif
  DEBUG_BREAK_ASM();
#endif
}

}  // namespace nu
