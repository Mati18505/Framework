#pragma once 
#define _WIN32_WINNT _WIN32_WINNT_WIN8 
#include <sdkddkver.h> 

#define WIN32_LEAN_AND_MEAN 
#define NOMINMAX 
#define STRICT 

#ifndef ZC_FULL_WINTARD 
#define NOGDICAPMASKS 
#define NOSYSMETRICS 
#define NOMENUS 
#define NOICONS 
#define NOSYSCOMMANDS 
#define NORASTEROPS 
#define OEMRESOURCE 
#define NOATOM 
#define NOCLIPBOARD 
#define NOCOLOR 
#define NOCTLMGR 
#define NODRAWTEXT 
#define NOKERNEL 
#define NONLS 
#define NOMEMMGR 
#define NOMETAFILE 
#define NOOPENFILE 
#define NOSCROLL 
#define NOSERVICE 
#define NOSOUND 
#define NOTEXTMETRIC 
#define NOWH 
#define NOCOMM 
#define NOKANJI 
#define NOHELP 
#define NOPROFILER 
#define NODEFERWINDOWPOS 
#define NOMCX 
#define NORPC 
#define NOPROXYSTUB 
#define NOIMAGE 
#define NOTAPE 
#endif 

#include <windows.h>
#undef GetMessage