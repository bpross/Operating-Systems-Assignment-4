GCCOPT     = -Wall -Wextra

TESTPROT_F = test1

testsyscall:
	gcc -o test syscall_test.c

testsyscalldelete:
	gcc -o test syscall_test_delete.c

testtable:
	gcc -o test key_table_test.c mfs/key_table.c mfs/key_table.h

protectfile:
	- rm protectfile protectfile.o
	gcc ${GCCOPT} -c protectfile.c rijndael.c
	gcc ${GCCOPT} -o protectfile protectfile.o rijndael.o
    
testprot:
	./protectfile e 1bc4 ${TESTPROT_F}
