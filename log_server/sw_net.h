#ifndef SW_NET_H
#define SW_NET_H

typedef int (*NET_ACCEPT_FUN)();
typedef int (*NET_RECV_FUNC)();

struct sw_net_cb
{
    NET_ACCEPT_FUN accept_func;
    NET_RECV_FUNC  rec_data_fun;

    sw_net_cb() :accept_func(0), rec_data_fun(0) {}
};
#endif // !SW_NET_H

