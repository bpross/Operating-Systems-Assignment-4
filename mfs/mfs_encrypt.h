#include <unistd.h>
#include "fs.h"
#include "inode.h"

#ifndef __MFS_ENCRYPT_H__
#define __MFS_ENCRYPT_H__

#define KEYBITS 128

void encrypt_buf(uid_t uid, inode_t fid,char *buf, int chunk);

#endif
