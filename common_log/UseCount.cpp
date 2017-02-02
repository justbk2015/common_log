#include "UseCount.h"

using namespace zb;
UseCount::UseCount(void):
    m_use_count(new int(1))
{
}
UseCount::UseCount(const UseCount& uc):
    m_use_count(uc.m_use_count)
{
    ++(*m_use_count);
}
UseCount::~UseCount()
{
    if (--(*m_use_count) == 0)
    {
        delete m_use_count;
    }
}

bool UseCount::only() const
{
    return *m_use_count == 1;
}
bool UseCount::makeonly()
{
    if (only())
    {
        return false;
    }
    (*m_use_count) --;
    m_use_count = new int(1);
    return true;
}
bool UseCount::reattach(const UseCount& uc)
{
    ++(*uc.m_use_count);
    if (--(*m_use_count) == 0)
    {
        delete m_use_count;
        m_use_count = uc.m_use_count;
        return true;
    }
    m_use_count = uc.m_use_count;
    return false;
}