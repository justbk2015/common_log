#include "Log.h"

using namespace zb;
#pragma warning(disable:4996)
Log::Log():
    m_level(0), m_tag(new std::string(( DEFAULT_TAG))),m_format(0),m_writers(new VECTOR_STREAM())
{

}
Log::Log(const char* tag, int level, int format, int output, const char* file):
    m_level(level), m_tag(new std::string((NULL_STRING(tag))? DEFAULT_TAG:tag)),m_format(format)
{
    m_writers = new VECTOR_STREAM();
    if (output & LOG_OUT_CONSOLE)
    {
        m_writers->push_back(zb::FileOutputStreamFactory::getFactory().get_filestream(NULL));
    }
    if (output & LOG_OUT_TRACE)
    {
        m_writers->push_back(OutputStreamHandle(new DeubgOutputStream(zb::DeubgOutputStream())));
    }
    if (output & LOG_OUT_FILE && file != NULL)
    {
        m_writers->push_back(zb::FileOutputStreamFactory::getFactory().get_filestream(file));
    }
}

Log& Log::operator=(const Log& l)
{
    if (m_use.reattach(l.m_use))
    {
        m_writers->clear();
        delete m_tag;
        delete m_writers;
    }
    m_tag = l.m_tag;
    m_writers = l.m_writers;
    m_level = l.m_level;
    m_format = l.m_format;
    return *this;
}
Log::~Log()
{
    if (m_use.only())
    {
        m_writers->clear();
        delete m_tag;
        delete m_writers;
    }
}
void Log::debug(const char* format, ...)
{
    if (m_level > LOG_DEBUG){ return ;}
    va_list  args ;
    va_start(args, format);
    write(LOG_DEBUG, format, args);
    va_end(args);
}
void Log::info(const char* format, ...)
{
    if (m_level > LOG_INFO){ return ;}
    va_list  args ;
    va_start(args, format);
    write(LOG_INFO, format, args);
    va_end(args);
}
void Log::warn(const char* format, ...)
{
    if (m_level > LOG_WARN){ return ;}
    va_list  args ;
    va_start(args, format);
    write(LOG_WARN, format, args);
    va_end(args);
}
void Log::error(const char* format, ...)
{
    if (m_level > LOG_ERROR){ return ;}
    va_list  args ;
    va_start(args, format);
    write(LOG_ERROR, format, args);
    va_end(args);
}
void Log::flush()
{
    for (VECTOR_STREAM::iterator it = m_writers->begin() ; it != m_writers->end() ; it++)
    {
        (it)->flush();
    }
}
void Log::write(int level, const char* format, va_list args)
{
    std::string log = format_log(level, format, args);
    for (VECTOR_STREAM::iterator it = m_writers->begin() ; it != m_writers->end() ; it++)
    {
        (it)->write(log);
    }
}
std::string  Log::format_log(int level, const char* format, va_list args)
{
    std::string total_log ;
    total_log += format_prefix(level);
    char tmp_buf[LOG_MAX_SIZE] = {0};
    vsprintf_s(tmp_buf, LOG_MAX_SIZE, format, args);
    total_log += tmp_buf;
    return total_log;
}
std::string Log::format_prefix(int level)
{
    //time  level tag pid  
    char out[LOG_PREFIX_HEAD_SIZE] = {0};
    if (m_format & LOG_FORMAT_TIME)
    {
        char time[LOG_TIME_MAX_SIZE] = {0};
        format_curtime(time); 
        sprintf(out, "%s%s%18s", out,out[0]==0?"":" ", time);
    }
    if (m_format & LOG_FORMAT_LEVEL)
    {
        const char* str_level[] = {"DEBUG", "INFO", "WARN", "ERROR"};
        sprintf(out, "%s%s%5s", out,out[0]==0?"":"|", str_level[level-1]);
    }
    if (m_format & LOG_FORMAT_TAG)
    {
        sprintf(out, "%s%s%16s", out,out[0]==0?"":"|", m_tag->c_str());
    }
    if (m_format & LOG_FORMAT_PID)
    {
        sprintf(out, "%s%s%5ld", out,out[0]==0?"":"|", GetCurrentThreadId());
    }
    if (out[0] !=0)
    {
        strcat(out, "|");
    }
    return std::string(out);
}
void Log::format_curtime(char* out)
{
    if (out == NULL) { return; }
    struct tm now;
    struct timeb tb;
    ftime(&tb);
    localtime_s(&now, &tb.time);
    sprintf(out, "%04d-%02d-%02d %02d:%02d:%02d-%03d",
        now.tm_year+1900,
        now.tm_mon+1,
        now.tm_mday,
        now.tm_hour,
        now.tm_min,
        now.tm_sec,
        tb.millitm);
}
#pragma warning(default:4996)