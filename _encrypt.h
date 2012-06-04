#include <lib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

PUBLIC void setkey(unsigned int k0, unsigned int k1)
{
    message m;
    m.m1_i1 = k0;
    m.m1_i2 = k1;
    char cur_path[1024];
    getcwd(cur_path,1024);
    printf("CWD: %s\n",cur_path);
    _loadname(cur_path,&m);
    int retvalue;
    retvalue = _syscall(VFS_PROC_NR, SETKEY, &m);
    printf("Return value: %d\n",retvalue);
    if(retvalue < 0){
        printf("Error!!\n");
        printf("Error with syscall: %s\n", strerror( errno ));
    }
}
