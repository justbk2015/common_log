#pragma once
#include <process.h>
#include "platform.h"
#include "Lock.h"
#include "UseCount.h"
#ifdef LOG_THREAD_EXPORTS
#define LOG_THREAD_INTERFACE EXPORT_SYMBOL
#else
#define  LOG_THREAD_INTERFACE IMPORT_SYMBOL
#pragma message("LOG_THREAD_INTERFACE import")
#endif
namespace zb 
{
    enum ThreadStatus 
    {
        STOP        =   -2,
        TERMINAL    =   -1,
        NOT_START   =   0,
        RUNNING     =   1  
    };
    template <class T>
    class Thread
    {
    public:
        //run是函数对象，实现了void*方法和复制构造方法 ，请确保不要出现死循环，否则线程无法退出
        Thread(T run):m_run(run),
            m_status(new ThreadStatus(ThreadStatus::NOT_START)),
            m_lock(new zb::Lock())
        {

        }
        Thread& operator=(const Thread& _t) //为了保证允许匿名引用,必须有该接口，否则该对象过快的析构将导致线程引用的对象变为空指针
        {
            if (_t.m_status == ThreadStatus::NOT_START) //只有_t未开始运行，才允许赋值
            {
                if (this->m_use.reattach(_t))
                {
                    delete m_lock;
                    delete m_status;
                }
                this->m_lock = _t.m_lock;
                this->m_status = _t.m_status;
                this->m_run = _t.m_run;
            }
            throw "can\'t exec operator=,because thread already run!"
        }
        virtual ~Thread() //
        {
            if (m_use.only())
            {
                delete m_lock;
                delete m_status;
            }
        }
    public:
        virtual void start() //直接开始线程
        {
            //注意这里必须传入指针，否则对象过快析构而导致空指针引用
            _beginthread(&Thread::work, NULL, new Thread<T>(*this));
        }
        virtual void stop() //直接停止线程
        {
            if (state() == ThreadStatus::RUNNING)
            {
                state(ThreadStatus::TERMINAL);
            }
        }
        virtual void begin()//开始线程，等待线程实际运行起来才返回
        {
            start();
            while (1)
            {
                ThreadStatus _state = state();
                if(_state != ThreadStatus::NOT_START)
                {
                    break;
                }
                ::Sleep(20);
            }
        }
        virtual void end()//停止线程，等待线程实际停止才返回
        {
            stop();
            while(state() == ThreadStatus::RUNNING)
            {
                ::Sleep(20);
            }
        }
        ThreadStatus state()
        {
            ThreadStatus _state ;
            m_lock->lock();
            _state = *m_status;
            m_lock->unlock();
            return _state;
        }
        void state(ThreadStatus _state) 
        {
            if (_state == ThreadStatus::NOT_START) //不允许设置为NOT_START状态
            {
                throw "don\'t set NOT_START state!";
            }
            m_lock->lock();
            *m_status = _state;
            m_lock->unlock();
        }
        T& getRunner() const 
        {
            return m_run;
        }
    protected:
    protected:
        static void work(void* p)
        {
            {
                CopyOnWriteHandle<Thread<T>>  handle(static_cast<Thread<T>*>(p));
                handle->state(ThreadStatus::RUNNING);
                handle->m_run( ((void*)( handle.get())));
                if (handle->state() == ThreadStatus::RUNNING)
                {
                    handle->state(ThreadStatus::STOP);
                }
            } //let CopyOnWriteHandle destroy, otherwise memory leak!
            _endthread();
        }
    private:
        T m_run;
        zb::Lock* m_lock;  //锁操作
        enum ThreadStatus* m_status; //  记录当前线程状态，初始为NOT_START
        zb::UseCount m_use;//  引用计数器
    };
}