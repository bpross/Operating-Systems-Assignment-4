/*
 * This file was created for CMPS111 Sprint 2012 at UCSC
 * Authors: Benjamin Ross, Quentin Rivers, Matthew Musselman
 * The purpose of this file is to provide a system call that
 * allows the user to set their encryption key in the FS.
 * This key will be used to en/decrypt files in the read()
 * write() system calls, only if the sticky bit is set.
 *
 * Usage: setkey(k0,k1)
 */
#include <lib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

PUBLIC void setkey(unsigned int k0, unsigned int k1)
{
    /*Setup the variables to be passed to the FS */
    message m;
    m.m1_i1 = k0;
    m.m1_i2 = k1;

    /* Get the return value of the syscall and check for errors */
    int check;
    check = _syscall(VFS_PROC_NR, SETKEY, &m);
    printf("Check in library: %d\n",check);
    if(check == 0)
        fprintf(stderr,"SETKEY ERROR: Key Table is Full\n");
    else if(check == -1)
        fprintf(stderr,"You key has been deleted\n");
    else if(check == -2)
        fprintf(stderr,"You do not have a key to delete\n");
    else if(check == 2)
        fprintf(stderr,"Your key has been updated\n");
    else
        fprintf(stderr,"Your Key has been set\n");
}
