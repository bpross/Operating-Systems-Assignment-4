#include "fs.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include "rijndael.h"
#include "mfs_encrypt.h"
int ctr;

void encrypt_buf(uid_t uid, ino_t fid,char *buf, int chunk){

    unsigned long rk[RKLENGTH(KEYBITS)];
    unsigned char key[KEYLENGTH(KEYBITS)];
    int k0 = -1;
    int k1 = -1;
    int i;
    for(i=0;i<entries;i++){
        if(UID(kt,i) == uid)
        {
            k0 = K0(kt,i);
            k1 = K1(kt,i);
        }
    }
    if (k0 == -1 && k1 == -1){
        return;
    }
    
    bzero(&key,sizeof(key));
    bcopy(&k0,&(key[0]),sizeof(k0));
    bcopy(&k1,&(key[sizeof(k0)]),sizeof(k1));

    unsigned char ciphertext[16];
    unsigned char ctrvalue[16];
    int nrounds;
    int offset = 0;

    nrounds = rijndaelSetupEncrypt(rk,key,KEYBITS);

    bcopy(&fid, &(ctrvalue[8]), sizeof(fid));
    if (chunk > 16){
        for(ctr = 0; ctr < chunk / 16; ctr++)
        {
            bcopy(&ctr, &(ctrvalue[0]), sizeof(ctr));

            rijndaelEncrypt(rk,nrounds, ctrvalue, ciphertext);

            for(i=0; i < 16; i++){
                *(buf+i+offset) ^= ciphertext[i];
            }
            offset += 16;
        }
    }
    else
    {
        bcopy(&ctr, &(ctrvalue[0]), sizeof(ctr));
        rijndaelEncrypt(rk, nrounds, ctrvalue, ciphertext);

        for(i=0; i < chunk; i++){
                *(buf+i+offset) ^= ciphertext[i];
        }
    }
}
