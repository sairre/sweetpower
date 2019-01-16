#ifndef _SW_ROLE_H
#define _SW_ROLE_H
#include "TestModule.h"
class SW_Role
{
public:
    SW_Role();
    virtual ~SW_Role();

    bool LoadRole();

    bool SaveRole();

    int get_player_id() { return m_player_id; }
    void set_player_id(int val) { m_player_id = val; }

    // TODO并非线程安全的
    bool get_is_connected() { return m_is_connected; }
    void set_is_connected(bool val) { m_is_connected = val; }
private:
    TestModule m_testModule;


private:
    bool m_is_connected;
    int m_player_id;
};

#endif

