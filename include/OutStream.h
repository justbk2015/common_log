#pragma once
#include <stdio.h>
#include <iostream>
#include <string>
#include <map>
#include <algorithm>
#include "platform.h"
#include "UseCount.h"
#include "Path.h"
#include "Lock.h"
#if PLATFOMR == PLATFORM_WINDOWS
#include <Windows.h>
#endif
#ifdef LOG_STREAM_EXPORTS
#define LOG_STREAM_INTERFACE EXPORT_SYMBOL
#else
#define  LOG_STREAM_INTERFACE IMPORT_SYMBOL
#pragma message("LOG_STREAM_INTERFACE import")
#endif
#define NULL_POINT_MAP_KEY  "NULL"
namespace zb
{
    class FileOutputStreamFactory;
    //windows平台专用于输出调试信息流
    class DeubgOutputStream
    {
    public:
        virtual ~DeubgOutputStream(){}
    public:
        virtual DeubgOutputStream& write(const std::string& buf) ;
        virtual DeubgOutputStream& write(const char* buf);
        virtual void flush();
    };

    //文件日志写入流
    class FileOutputStream:public DeubgOutputStream
    {
        friend class FileOutputStreamFactory;
    public:
        virtual ~FileOutputStream();//允许外部删除此对象
    private://显示禁止赋值和构造
        FileOutputStream(std::string logfile);//注意这是唯一一个可以被工厂所调用的构造函数
        FileOutputStream(const FileOutputStream& fs){}
        virtual FileOutputStream& operator=(const FileOutputStream& fs){return *this;}
 
    public:
        virtual DeubgOutputStream& write(const std::string& buf);
        virtual void flush();
    private:
        FILE* m_file;//底层的写指针
        bool m_file_auto_close;//是否需要自动关闭,为真表示需要自动关闭
        zb::Lock* m_lock;
    };
    class LOG_STREAM_INTERFACE OutputStreamHandle:public zb::CopyOnWriteHandle<DeubgOutputStream>
    {
    public:
        OutputStreamHandle():CopyOnWriteHandle<DeubgOutputStream>(){}
        OutputStreamHandle(DeubgOutputStream* _t):CopyOnWriteHandle<DeubgOutputStream>(_t){}
    public:
        DeubgOutputStream& write(const std::string& buf) ;
        DeubgOutputStream& write(const char* buf);
        void flush();
    };
    class LOG_STREAM_INTERFACE FileOutputStreamFactory
    {
        typedef std::map<std::string, OutputStreamHandle> MAP_FILEOUTPUTSTREAM ;
        typedef std::pair<std::string, OutputStreamHandle> PAIR_FILEOUTPUTSTREAM;
    public:
        ~FileOutputStreamFactory();
    public:
        OutputStreamHandle& get_filestream(const char* logfile);
        int gc();//清理仅被工厂所持有的底层对象，返回被清理的个数
    private:
        FileOutputStreamFactory();
        FileOutputStreamFactory& operator=(const FileOutputStreamFactory& fs_factory) {return *this;}
        std::string find_filestream_key(const char* logfile);
        MAP_FILEOUTPUTSTREAM* m_outputs;
    public:
        static FileOutputStreamFactory& getFactory();
    private:
        static  FileOutputStreamFactory s_factory;
        
    };
}

