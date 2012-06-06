testsyscall:
	gcc -o test syscall_test.c

testtable:
	gcc -o test key_table_test.c mfs/key_table.c mfs/key_table.h
