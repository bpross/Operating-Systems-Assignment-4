GCCOPT     = -Wall -Wextra

TESTPROT_F = test1

setkey:
	gcc -o setkey setkey.c
	chmod +x setkey
	mv setkey /usr/bin

testtable:
	gcc -o test key_table_test.c mfs/key_table.c mfs/key_table.h

protectfile:
	- rm protectfile protectfile.o
	gcc ${GCCOPT} -c protectfile.c rijndael.c
	gcc ${GCCOPT} -o protectfile protectfile.o rijndael.o
    
testprot:
	./protectfile e 1bc4 ${TESTPROT_F}
