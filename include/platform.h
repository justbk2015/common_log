#pragma once
#define PLATFORM_LINUX     0x1 //linuxƽ̨
#define PLATFORM_WINDOWS   0x2 //windowsƽ̨

#define PLATFORM_32BIT     0x10
#define PLATFORM_64BIT     0x20

#define EXPORT_SYMBOL     __declspec(dllexport)
#define IMPORT_SYMBOL     __declspec(dllimport)  

#ifdef WIN32
#define PLATFOMR PLATFORM_WINDOWS
#else
#define PLATFOMR PLATFORM_LINUX
#endif

#define LOG_MODULE_NAME "common_log.dll"

#ifndef USE_LOG_DLL
#define LOG_MODULE_EXPORTS
#define LOG_STREAM_EXPORTS
#define LOG_PATH_EXPORTS
#define LOG_LOCK_EXPORTS
#define LOG_USECOUNT_EXPORTS
#endif