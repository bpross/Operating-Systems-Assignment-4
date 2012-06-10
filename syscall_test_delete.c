/*This is work done for CMPS111 Spring 2012
 * This work was done by Benjamin Ross, Quentin Rivers and Matthew Musselamn
 * The purpose of this file is to test the implementation of the setkey
 * systemcall. It follows the test cases outlined in Section 4 of 
 * design.pdf
 */


#include <stdio.h>
#include <_encrypt.h>

int main()
{
    int k0 = 0;
    int k1 = 0;
    int ret_value;

    setkey(k0,k1);

    return 0;
}
