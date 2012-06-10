/*
 * This file was created for CMPS111 Sprint 2012 at UCSC
 * Authors: Benjamin Ross, Quentin Rivers, Matthew Musselman
 */
#include <stdio.h>
#include <string.h>
#include <_encrypt.h>

int main(int argc, char** argv)
{
    if (argc < 3){
        fprintf(stderr,"Usage: %s <key1> <key2>\n",argv[0]);
        return 1;
    }

    int k0;
    int k1;

    /* Get variables from the commandline */
    k0 = strtol(argv[1],NULL,0);
    k1 = strtol(argv[2],NULL,0);
    setkey(k0,k1);
    return 0;
}
