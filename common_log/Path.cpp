#include "Path.h"
using namespace zb;
std::string Path::get_cur_run_path()
{
    char tmp_path[_MAX_PATH] = {0};
    GetModuleFileNameA(GetModuleHandleA(LOG_MODULE_NAME), tmp_path , _MAX_PATH);
    char* p = strrchr(tmp_path, '\\');
    *(p+1) = 0;
    return std::string(tmp_path);
}
bool Path::is_abspath() const 
{
    return (*m_path).find(':') != std::string::npos;;
}
Path Path::abspath() const 
{
    Path path = *this;
    path.convert_abspath();
    return path;
}
std::string& Path::convert_abspath()
{
    if (!is_abspath())
    {
        std::string run_path = get_cur_run_path();
        std::string tmp_path = (*m_path).empty()? (run_path + DEFAULT_LOG_PATH) : run_path + *m_path;
        delete m_path;
        m_path = new std::string(tmp_path);
    }
    return *m_path;
}