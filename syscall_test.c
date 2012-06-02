/*This is work done for CMPS111 Spring 2012
 * This work was done by bpross, apoit, and qsun
 */


#include <stdio.h>
#include <_encrypt.h>

int main()
{
    int k0 = 1337;
    int k1 = 31337;
    int ret_value;

    ret_value = setkey(k0,k1);
    printf("Return Value on SetKey: %d\n", ret_value);

    return 0;
}
