#include "StructData.h"
// 不传长度，会在调用具体回调前进行判断，过滤长度不符合的消息
class SW_Role;
typedef int (* PROCESS_HANDLER)(SW_Role * role, SERVER_BASE_DATA * data);