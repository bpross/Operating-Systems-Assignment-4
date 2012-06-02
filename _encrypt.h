#include <lib.h>
#include <unistd.h>

PUBLIC void setkey(unsigned int k0, unsigned int k1)
{
    message m;
    m.m1_i1 = k0;
    m.m1_i2 = k1;

    return(_syscall(PM_PROC_NR, SEMINIT, &m))
}
