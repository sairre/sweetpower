#include "selector.h"
#include "LogNetMgr.h"
int main(void)
{
    LogNetMgr::Instance()->CreateListener(0, 0);
    return 0;
}