GCCOPT     = -Wall -Wextra
TESTPROT_F = test1
MAKE = make

all:
	${MAKE} installvfs
	${MAKE} installmfs
	${MAKE} installen
	${MAKE} compile

installvfs:
	echo -e "Coping VFS files\n\n"
	cp ./vfs/protect.c /usr/src/servers/vfs/protect.c
	cp ./vfs/proto.h /usr/src/servers/vfs/proto.h
	cp ./vfs/request.c /usr/src/servers/vfs/request.c
	cp ./vfs/table.c /usr/src/servers/vfs/table.c
	cp ./vfs/vfsif.h /usr/src/include/minix/vfsif.h
	cp ./vfs/read.c /usr/src/servers/vfs/read.c
	cp ./vfs/glo.h /usr/src/servers/vfs/glo.h
	echo -e "DONE COPYING VFS FILES\n\n"

installmfs:
	echo -e "Copying MFS files \n\n"
	cp ./mfs/callnr.h /usr/src/include/minix/callnr.h
	cp ./mfs/table.c /usr/src/servers/mfs/table.c
	cp ./mfs/proto.h /usr/src/servers/mfs/proto.h
	cp ./mfs/Makefile /usr/src/servers/mfs/Makefile
	cp ./mfs/protect.c /usr/src/servers/mfs/protect.c
	cp ./mfs/misc.c /usr/src/servers/mfs/misc.c
	cp ./mfs/main.c /usr/src/servers/mfs/main.c
	cp ./mfs/glo.h /usr/src/servers/mfs/glo.h
	cp ./mfs/key_table.h /usr/src/servers/mfs/key_table.h
	cp ./mfs/read.c /usr/src/servers/mfs/read.c
	cp ./_encrypt.h /usr/include/_encrypt.h
	echo -e "DONE COPYING MFS FILES\n\n"

installen:
	echo -e "Copying Encrytion Files"
	cp ./mfs/rijndael.c /usr/src/servers/mfs/rijndael.c
	cp ./mfs/rijndael.h /usr/src/servers/mfs/rijndael.h
	cp ./mfs/mfs_encrypt.h /usr/src/servers/mfs/mfs_encrypt.h
	cp ./mfs/encrypt.c /usr/src/servers/mfs/encrypt.c
	echo -e "Done Copying Encrytion Files"

compile:
	cd /usr/src/tools/
	make hdboot
	reboot

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
