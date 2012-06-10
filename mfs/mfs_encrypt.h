#include <unistd.h>
#include "fs.h"
#include "inode.h"
#include "key_table.h"

#ifndef __MFS_ENCRYPT_H__
#define __MFS_ENCRYPT_H__

#define KEYBITS 128

void encrypt_buf(uid_t uid, ino_t fid,char *buf, int chunk,u64 position);

#endif
