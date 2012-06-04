#include <lib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

PUBLIC void setkey(unsigned int k0, unsigned int k1)
{
    message m;
    m.m1_i1 = k0;
    m.m1_i2 = k1;
    int retvalue;
    retvalue = _syscall(MFS, SETKEY, &m);
    printf("Return value: %d\n",retvalue);
    if(retvalue < 0){
        printf("Error!!\n");
        printf("Error with syscall: %s\n", strerror( errno ));
    }
}
