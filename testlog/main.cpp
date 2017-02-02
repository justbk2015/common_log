#include <stdio.h>
#include <process.h>
#define USE_LOG_DLL
#include "../include/Log.h"
#pragma comment(lib, "../x64/Debug/common_log.lib")
const char* test = "test";
void print_log(void* args)
{
    zb::Log* log = (zb::Log*) args;
    for (int i = 0 ; i < 10*1000 ; i++)
    {
        log->debug("%s-%s\n", "this is a test", test);
        log->info("%s-%s\n", "this is a test", test);
        log->warn("%s-%s\n", "this is a test", test);
        log->error("%s-%s\n", "this is a test", test);
        //log->flush();
    }
    _endthread();
}
void test1()
{
    zb::Log log1("tag1", LOG_DEBUG, LOG_FORMAT_ALL, LOG_OUT_CF, "");
    zb::Log log2("tag2", LOG_DEBUG, LOG_FORMAT_ALL, LOG_OUT_CF, "");
    zb::Log log3("tag3", LOG_DEBUG, LOG_FORMAT_ALL, LOG_OUT_CF, "");
    zb::Log log4("tag4", LOG_DEBUG, LOG_FORMAT_ALL, LOG_OUT_CF, "");;
    zb::Log log5("tag5", LOG_DEBUG, LOG_FORMAT_ALL, LOG_OUT_CF, "");;
    zb::Log log6("tag6", LOG_DEBUG, LOG_FORMAT_ALL, LOG_OUT_CF, "");;
    zb::Log log7("tag7", LOG_DEBUG, LOG_FORMAT_ALL, LOG_OUT_CF, "");;
    _beginthread(print_log, NULL, &log1);
    _beginthread(print_log, NULL, &log2);
    _beginthread(print_log, NULL, &log3);
    _beginthread(print_log, NULL, &log4);
    _beginthread(print_log, NULL, &log5);
    _beginthread(print_log, NULL, &log6);
    _beginthread(print_log, NULL, &log7);
}
void test2()
{
    zb::Log log1("tag1", LOG_DEBUG, LOG_FORMAT_ALL, LOG_OUT_CF, "");
    log1.debug("%s-%s\n", "this is a test", test);
    log1.info("%s-%s\n", "this is a test", test);
    log1.warn("%s-%s\n", "this is a test", test);
    log1.error("%s-%s\n", "this is a test", test);
}
//我就要加上中文注释，你怎么样
int main(int argc, char** argv)
{
    test2();
    getchar();
    return 0;
}
