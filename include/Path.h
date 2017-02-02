#pragma once
#include <iostream>
#include <string>
#include "platform.h"
#if PLATFOMR == PLATFORM_WINDOWS
#include <windows.h>
#endif
#include "platform.h"
#ifdef LOG_PATH_EXPORTS
#define LOG_PATH_INTERFACE EXPORT_SYMBOL
#else
#define LOG_PATH_INTERFACE IMPORT_SYMBOL
#pragma message("LOG_PATH_INTERFACE import")
#endif
#define DEFAULT_LOG_PATH "default.log"
namespace zb
{
    class LOG_PATH_INTERFACE Path
    {
    public:
        Path(void):m_path ( new std::string()) {}
        Path(const char* path):m_path(path == NULL? new std::string(): new std::string(path)){}
        Path(std::string& path):m_path(new std::string(path)){}
        Path(const Path& path):m_path(new std::string(*path.m_path)){}
        ~Path(void){ delete m_path ;}
    public:
        bool is_abspath() const ;
        Path abspath() const ;
        std::string& convert_abspath();
    private:
        std::string* m_path;
    public:
        //************************************
        // Method:    get_cur_run_path
        // FullName:  zb::Path::get_cur_run_path
        // Access:    public static 
        // Returns:   std::string
        // Qualifier:获取当前模块的运行绝对路径，带\\
        //************************************
        static std::string get_cur_run_path();
    };
}


