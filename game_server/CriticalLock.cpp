#include "CriticalLock.h"


CriticalLock::CriticalLock(CRITICAL_SECTION& cs)
{
    ptr_cs = &cs;

    EnterCriticalSection(ptr_cs);
}

CriticalLock::CriticalLock()
{

}

CriticalLock::~CriticalLock()
{
    LeaveCriticalSection(ptr_cs);
    ptr_cs = NULL;
}
