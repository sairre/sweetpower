#include "selector.h"
int main(void)
{
    selector my_selector;
    my_selector.start_listen();
    my_selector.start();
    return 0;
}