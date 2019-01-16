#ifndef __STRUCT_DATA_H
#define __STRUCT_DATA_H
struct SERVER_BASE_DATA
{
    int cmd_type;
    int sub_type;

    SERVER_BASE_DATA():cmd_type(0),sub_type(0)
    {}
};

struct TEST_DATA :SERVER_BASE_DATA
{
    int ext_test;
    int para_int;
    TEST_DATA() :ext_test(0), para_int(0) {}
};
#endif