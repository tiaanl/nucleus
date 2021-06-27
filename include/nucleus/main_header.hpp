#pragma once

#include "nucleus/config.h"

#if OS(WIN)
#include "nucleus/Win/includes.h"
#define NU_MAIN_HEADER int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
#else
#define NU_MAIN_HEADER int main(int, char*[])
#endif
