#pragma once
#if defined(__GNUG__) || defined(__GNUC__)
#define OS_LINUX
#elif defined(_MAC)
#define OS_MAC
#elif defined(_WIN32)
#define OS_WIN
#endif