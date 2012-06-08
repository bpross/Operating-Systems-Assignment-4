#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "fs.h"
#include "rijndael.h"
#include "mfs_encrypt.h"

void encrypt_buf(uid_t uid, inode_t fid,char *buf, int chunk){

    unsigned long rk[RKLENGTH(KEYBITS)];
    unsigned char key[KEYLENGTH(KEYBITS)];
    int u_key = get_key_by_uid(kt,uid);
    bcopy(&u_key, &(key[0]), sizeof(key));
    unsigned char ciphertext[16];
    unsigned char ctrvalue[16];
    int nrounds;
    int offset = 0;
    int i,ctr;

    nrounds = rijndaelSetupEncrypt(rk,key,KEYBITS);

    bcopy(&fid, &(ctrvalue[8]), sizeof(fid));

    for(ctr = 0; ctr < chunk % 16; ctr++)
    {
        bcopy(&ctr, &(ctrvalue[0]), sizeof(ctr));

        rijndaelEncrypt(rk,nrounds, ctrvalue, ciphertext);

        for(i=0; i < 16 | i >= chunk; i++){
            buf[i+offset] ^= ciphertext[i];
            printf("Encrypted: %c\n",*(buf+i));
        }
        offset += 16;
    }
}
