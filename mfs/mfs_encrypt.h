#include "fs.h"
#include "inode.h"
#include "super.h"
#include "key_table.h"
#include <stddef.h>
#include "buf.h"
#include <string.h>
#include <stdlib.h>
#include <minix/com.h>
#include <minix/u64.h>
#include <minix/vfsif.h>
#include <assert.h>
#include <stdio.h>


#ifndef __MFS_ENCRYPT_H__
#define __MFS_ENCRYPT_H__

#define KEYBITS 128

void encrypt_buf(uid_t uid, ino_t fid,char *buf, int chunk,int position);

#endif
