#pragma once

#include "nucleus/Config.h"

#if OS(WIN)
#include "nucleus/Win/WindowsMixin.h"
#define NU_MAIN_HEADER int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
#else
#define NU_MAIN_HEADER int main(int, char*[])
#endif
