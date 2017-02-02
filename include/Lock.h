#pragma once

#include "platform.h"
#if PLATFOMR == PLATFORM_WINDOWS
#include <Windows.h>
#endif
#ifdef LOG_LOCK_EXPORTS
#define LOG_LOCK_INTERFACE EXPORT_SYMBOL
#else
#define  LOG_LOCK_INTERFACE IMPORT_SYMBOL
#pragma message("LOG_LOCK_INTERFACE import")
#endif
namespace zb
{
    class LOG_LOCK_INTERFACE Lock
    {
    public:
        Lock(){init_lock();}
        virtual ~Lock() { release_lock();}
    public:
        void lock() 
        {
            //EnterCriticalSection(&m_lock);
        }
        void unlock()
        {
            //LeaveCriticalSection(&m_lock);
        }
    private:
        Lock(Lock& l){}
        Lock& operator= (Lock& l){return *this;}
        void init_lock()
        {
            InitializeCriticalSection(&m_lock);
        }
        void release_lock()
        {
            DeleteCriticalSection(&m_lock);
        }
        CRITICAL_SECTION m_lock;
    };
}