#ifndef __TEST_NET_SESSION_H__
#define __TEST_NET_SESSION_H__
class BufferHandler;
class TestNetSession
{
public:
    TestNetSession();
    ~TestNetSession();

    BufferHandler* GetBufferHandler() { return m_pBufferHandler; }

    static void HandleRecData(void* args);
    
    

private:
    BufferHandler* m_pBufferHandler;
};
#endif

