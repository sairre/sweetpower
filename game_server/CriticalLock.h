#ifndef __CRITICAL_LOCK_H
#define __CRITICAL_LOCK_H
#include "PlatForm.h"
class CriticalLock
{
public:
    CriticalLock(CRITICAL_SECTION& cs);
    ~CriticalLock();

    void operator=(const CriticalLock & cs_lock)
    {
        ptr_cs = cs_lock.ptr_cs;
    }

private:
    CriticalLock();

private:

    CRITICAL_SECTION * ptr_cs;


};

#endif

