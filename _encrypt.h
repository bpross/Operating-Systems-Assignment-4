#include <lib.h>
#include <unistd.h>

PUBLIC void setkey(unsigned int k0, unsigned int k1)
{
    message m;
    m.m1_i1 = k0;
    m.m1_i2 = k1;
    int retvalue;
    retvalue = _syscall(MFS_PROC_NR, SETKEY, &m);
    printf("Return value: %d\n",retvalue);
}
