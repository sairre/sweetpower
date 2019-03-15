#ifndef INET_MGR_H
#define INET_MGR_H
class INetMgr
{
public:
    INetMgr();
    ~INetMgr();

    virtual int CreateListener(int type, int port) = 0;
};
#endif

