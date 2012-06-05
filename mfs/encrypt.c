#include <minix/callnr.h>
#include <signal.h>
#include <sys/svrctl.h>
#include <sys/resource.h>
#include <sys/utsname.h>
#include <minix/com.h>
#include <minix/config.h>
#include <minix/sysinfo.h>
#include <minix/type.h>
#include <minix/vm.h>
#include <machine/archtypes.h>
#include <lib.h>
#include <assert.h>
#include <stdio.h>
int do_setkey()
{
    
    printf("Encrypt.c\n");
    int k0 = m.in.m1_i1;
    int k1 = m.in.m1_i2;
    printf("MFS!!! BITCH!\n");
    printf("K0: %d\nK1: %d\n",k0,k1);

    return 0;
}
