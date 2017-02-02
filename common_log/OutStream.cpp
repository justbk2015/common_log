#include "OutStream.h"
using namespace zb;
#pragma warning(disable:4996) //禁止fopen的告警
DeubgOutputStream& DeubgOutputStream::write(const std::string& buf) 
{
#if PLATFOMR == PLATFORM_WINDOWS
    OutputDebugStringA(buf.c_str());
#endif
    return *this;
}
DeubgOutputStream& DeubgOutputStream::write(const char* buf)
{
    return this->write(std::string(buf));
}
void DeubgOutputStream::flush()
{
    return;
}

FileOutputStream::~FileOutputStream()
{
    delete m_lock;
    if (m_file_auto_close && m_file != NULL)
    {
        fclose(m_file);
    }
}
FileOutputStream::FileOutputStream(std::string logfile):
    m_lock(new zb::Lock())
{
    if(logfile == NULL_POINT_MAP_KEY)
    {
        m_file = stdout;
        m_file_auto_close = false;
    }
    else
    {
        m_file = fopen(logfile.c_str(), "a+");
        _ASSERT(m_file != NULL);
        m_file_auto_close = true;
    }   
}

DeubgOutputStream& FileOutputStream::write(const std::string& buf)
{
    if(m_file == NULL){ return *this; }
    m_lock->lock();
    fwrite(buf.c_str(), 1, buf.size(),  m_file);
    m_lock->unlock();
    return *this;
}
void FileOutputStream::flush()
{
    if(m_file == NULL) {return;}
    fflush(m_file);
}

DeubgOutputStream& OutputStreamHandle::write(const std::string& buf) 
{
    if (m_t == NULL)
    {
        throw "DeubgOutputStream can\'t be NULL";
    }
    m_t->write(buf);
    return *m_t;
}
DeubgOutputStream& OutputStreamHandle::write(const char* buf)
{
    if (m_t == NULL)
    {
        throw "DeubgOutputStream can\'t be NULL";
    }
    m_t->write(buf);
    return *m_t;
}
void OutputStreamHandle::flush()
{
    if (m_t == NULL)
    {
        throw "DeubgOutputStream can\'t be NULL";
    }
    m_t->flush();
}

//以下是工厂类的成员实现
FileOutputStreamFactory FileOutputStreamFactory::s_factory;
FileOutputStreamFactory::FileOutputStreamFactory()
{
    m_outputs = new MAP_FILEOUTPUTSTREAM();
}
FileOutputStreamFactory::~FileOutputStreamFactory()
{
    m_outputs->clear();
    delete m_outputs;
}
OutputStreamHandle& FileOutputStreamFactory::get_filestream(const char* logfile)
{
    std::string key = find_filestream_key(logfile);
    MAP_FILEOUTPUTSTREAM::iterator it = m_outputs->find(key);
    if (it == m_outputs->end())
    {
        OutputStreamHandle h_fs ( new FileOutputStream(key));
        m_outputs->insert(PAIR_FILEOUTPUTSTREAM(key, h_fs));
    }
    return (*m_outputs)[key];
}
int FileOutputStreamFactory::gc()
{
    int gc_number = 0;
    MAP_FILEOUTPUTSTREAM::iterator it = m_outputs->begin();
    for (;it != m_outputs->end() ;)
    {
        MAP_FILEOUTPUTSTREAM::iterator it_tmp = it ;
        if (it->second.only())
        {
            it_tmp = ++ it;
            m_outputs->erase(--it);
            gc_number ++;
            it = it_tmp;
        }
        else
        {
            it ++;
        }
    }
    return gc_number;
}

std::string FileOutputStreamFactory::find_filestream_key(const char* logfile)
{
    std::string key_of_map ;
    if (logfile == NULL)
    {
        key_of_map = NULL_POINT_MAP_KEY;
    }
    else
    {
        key_of_map = zb::Path(logfile).convert_abspath();
    }
    return key_of_map;
}
FileOutputStreamFactory& FileOutputStreamFactory::getFactory()
{
    return s_factory;
}
#pragma warning(default:4996)