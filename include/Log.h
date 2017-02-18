#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string>
#include <sys/timeb.h>
#include <time.h>
#include <stdlib.h>
#include "platform.h"
#include "UseCount.h"
#include "OutStream.h"
#ifdef LOG_MODULE_EXPORTS
#define LOG_MODULE_INTERFACE EXPORT_SYMBOL
#else
#define  LOG_MODULE_INTERFACE IMPORT_SYMBOL
#pragma message("LOG_LOG_INTERFACE import")
#endif

#define LOG_DEBUG (0x1)
#define LOG_INFO  (0x2)
#define LOG_WARN  (0x3)
#define LOG_ERROR (0x4)

#define LOG_FORMAT_TAG  (0x1)
#define LOG_FORMAT_TIME  (0x2)
#define LOG_FORMAT_PID  (0x4)
#define LOG_FORMAT_LEVEL  (0x8)
#define LOG_FORMAT_ALL    (LOG_FORMAT_TAG | LOG_FORMAT_TIME | LOG_FORMAT_PID | LOG_FORMAT_LEVEL)

#define LOG_OUT_CONSOLE  (0x1)
#define LOG_OUT_TRACE    (0x2)
#define LOG_OUT_FILE     (0x4)
#define LOG_OUT_CF       (LOG_OUT_CONSOLE | LOG_OUT_FILE)
#define LOG_OUT_TF       (LOG_OUT_TRACE | LOG_OUT_FILE)
#define LOG_OUT_ALL      (LOG_OUT_CF | LOG_OUT_TRACE)

#define LOG_LEVEL_MAX_SIZE   (6) //log level 字段的最大长度 ，用于格式控制
#define LOG_TAG_MAX_SIZE   (16) //tag 标签的最大长度，用于格式控制
#define LOG_PID_MAX_SIZE    (8)//
#define LOG_TIME_MAX_SIZE   (32)
#define LOG_PREFIX_HEAD_SIZE   (128) //日志的前缀输出不能超过该长度
#define LOG_MAX_SIZE  (4*1024) //单条日志输出，最大的buf长度

#define DEFAULT_TAG "test"
#define NULL_STRING(str) (str == NULL || str[0]==0 )

namespace zb 
{
	class LOG_MODULE_INTERFACE Log
	{
        typedef std::vector<OutputStreamHandle> VECTOR_STREAM;
	public:
        Log();
		//************************************
		// Method:    Log log的构造函数
		// FullName:  zb::Log::Log
		// Access:    public 
		// Returns:   
		// Qualifier:
		// Parameter: const char * tag 此日志输出的标签名，为NULL时默认为test
		// Parameter: int level  允许日志输出的等级，若设置为LOG_WARN，则该对象的debug和info不产生输出
		// Parameter: int format 日志前缀包含的格式，顺序为 时间 |级别|标签|PID|  ......
		// Parameter: int output 输出日志的位置，包含有控制台，文件，以及TRACE(仅windows平台有效)
		// Parameter: const char * file  输出文件的位置，为NULL则重定向到控制台，为""则为dll目录下的default.log文件，可以传入绝对路径
		//************************************
		Log(const char* tag, int level, int format, int output, const char* file);
        Log& operator=(const Log& l);
		~Log(); 
	public:
		void debug(const char* format, ...);
		void info(const char* format, ...);
		void warn(const char* format, ...);
		void error(const char* format, ...);
        void flush();
    private:
        void write(int level, const char* format, va_list args);

        std::string  format_log(int level, const char* format, va_list args);
        std::string format_prefix(int level);
        static void format_curtime(char* out);
    private:
        VECTOR_STREAM* m_writers;
        std::string* m_tag;
        UseCount m_use;
        int m_level;
        int m_format;
	};
}



