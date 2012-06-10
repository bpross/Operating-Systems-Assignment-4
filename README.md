cmps111_asg4_bmq
================

What Works:
We got protectfile working. It is able to encrypt and decrypt a file given a hex key.
The options work correctly as well.
We got encryption into the kernel and we got it working. It is able to encrypt on 
write and decrypt. However, we are not able to get it to work with protectfile.
We know this has something to do with the way we are decrypting/encrypting the 
buffer. We did not have enough time to diagnose the problem.

Extra Credit:
We were able to get the basic logic down for the extra credit. We did not have
enough time to figure out how to get a file to write back to itself through
the kernel, because we ran out of time. Given enough time we probably could
have gotten this to work. The protect.c file lives in extra_credit/

To Install:
cd into our dir
make

To Install Setkey:
make setkey
This will install setkey into your /usr/bin

To Test Protectfile:
make protectfile
Test as you will
