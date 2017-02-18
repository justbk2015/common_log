#pragma once
#include "platform.h"
#ifdef LOG_USECOUNT_EXPORTS
#define LOG_USECOUNT_INTERFACE EXPORT_SYMBOL
#else
#define  LOG_USECOUNT_INTERFACE IMPORT_SYMBOL
#pragma message("LOG_USECOUNT_INTERFACE import")
#endif
namespace zb
{
    class LOG_USECOUNT_INTERFACE UseCount//引用计数器的实现
    {
    public:
        UseCount(void);
        UseCount(const UseCount& uc);
        ~UseCount(void);
    public:
        bool reattach(const UseCount& uc);//为真表示已经清除了本身所持有的计数空间，为假则表示共享计数空间,与赋值(=)操作类似
        bool only() const;//判断当前是否为唯一
        bool makeonly();//使唯一,为false表示本身已经是唯一了，不需要特殊处理；为真时表示需要经过特殊处理
    private:
        UseCount& operator=(const UseCount& uc){return *this;}//禁止赋值，用reattach替代之
        int* m_use_count;
    };

    template <class T>
    class CopyOnWriteHandle
    {
    public:
        CopyOnWriteHandle():m_t(NULL){}
        CopyOnWriteHandle(T* _t):m_t(_t){}
        virtual CopyOnWriteHandle& operator=(const CopyOnWriteHandle& handle)
        {
            if (m_use.reattach(handle.m_use))
            {
                if (m_t != NULL)
                {
                    delete m_t;
                }
            }
            m_t = handle.m_t;
            return *this;
        }
        virtual ~CopyOnWriteHandle()
        {
            if (m_use.only())
            {
                if (m_t != NULL)
                {
                    delete m_t;
                }
            }
        }
        bool only() const
        {
            return m_use.only();
        }
    protected:
        T* m_t;
        UseCount m_use;
    };
}

