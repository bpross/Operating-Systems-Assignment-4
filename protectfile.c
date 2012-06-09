#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "rijndael.h"

#define KEYBITS 128
#define USR_KEY_BITS 64
#define USR_KEY_CHARS USR_KEY_BITS / 4

/* Borrowed from rijndael.c */
typedef unsigned long u32;
typedef unsigned char u8;

/***********************************************************************
 *
 * hexvalue
 *
 * This routine takes a single character as input, and returns the
 * hexadecimal equivalent.  If the character passed isn't a hex value,
 * the program exits.
 *
 * Copied from encrypt.c
 *
 ***********************************************************************
 */
int hexvalue (char c)
{
  if (c >= '0' && c <= '9') {
    return (c - '0');
  } else if (c >= 'a' && c <= 'f') {
    return (10 + c - 'a');
  } else if (c >= 'A' && c <= 'F') {
    return (10 + c - 'A');
  } else {
    fprintf (stderr, "ERROR: key digit %c isn't a hex digit!\n", c);
    exit (-1);
  }
}

void usage_exit(char* progname) {
    fprintf(stderr, "Usage: %s [e|d] [key] [filename]\n", progname);
    exit (EXIT_FAILURE);
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "rijndael.h"

#define KEYBITS 128
#define USR_KEY_BITS 64
#define USR_KEY_CHARS USR_KEY_BITS / 4

/* Borrowed from rijndael.c */
typedef unsigned long u32;
typedef unsigned char u8;

/***********************************************************************
 *
 * hexvalue
 *
 * This routine takes a single character as input, and returns the
 * hexadecimal equivalent.  If the character passed isn't a hex value,
 * the program exits.
 *
 * Copied from encrypt.c
 *
 ***********************************************************************
 */
int hexvalue (char c)
{
  if (c >= '0' && c <= '9') {
    return (c - '0');
  } else if (c >= 'a' && c <= 'f') {
    return (10 + c - 'a');
  } else if (c >= 'A' && c <= 'F') {
    return (10 + c - 'A');
  } else {
    fprintf (stderr, "ERROR: key digit %c isn't a hex digit!\n", c);
    exit (-1);
  }
}

void usage_exit(char* progname) {
    fprintf(stderr, "Usage: %s [mode] [key] [filename]\n", progname);
    exit (EXIT_FAILURE);
}

void encrypt_file(char* filename, ino_t file_nr, u8* key) {
  /* The following code has been adopted from encrypt.c, its variable names
   * have been modified to fit the rest of the code.
   */
  unsigned long rk[RKLENGTH(KEYBITS)];
  int fd;
  int ctr;
  unsigned char filedata[16];
  unsigned char ciphertext[16];
  unsigned char ctrvalue[16];
  int i, nrounds, totalbytes, nbytes, nwritten;
  
  /*
   * Initialize the Rijndael algorithm.  The round key is initialized by this
   * call from the values passed in key and KEYBITS.
   */
  nrounds = rijndaelSetupEncrypt(rk, key, KEYBITS);
    
  /*
   * Open the file.
   */
  fd = open(filename, O_RDWR);
  if (fd < 0)
  {
    fprintf(stderr, "Error opening file %s\n", filename);
    exit(EXIT_FAILURE);
  }

  /* fileID goes into bytes 8-11 of the ctrvalue */
  bcopy (&file_nr, &(ctrvalue[8]), sizeof (file_nr));

  /* This loop reads 16 bytes from the file, XORs it with the encrypted
     CTR value, and then writes it back to the file at the same position.
     Note that CTR encryption is nice because the same algorithm does
     encryption and decryption.  In other words, if you run this program
     twice, it will first encrypt and then decrypt the file.
  */
  for (ctr = 0, totalbytes = 0; /* loop forever */; ctr++)
  {
    /* Read 16 bytes (128 bits, the blocksize) from the file */
    nbytes = read (fd, filedata, sizeof (filedata));
    if (nbytes <= 0) {
      break;
    }
    if (lseek (fd, totalbytes, SEEK_SET) < 0)
    {
      perror ("Unable to seek back over buffer");
      exit (-1);
    }

    /* Set up the CTR value to be encrypted */
    bcopy (&ctr, &(ctrvalue[0]), sizeof (ctr));

    /* Call the encryption routine to encrypt the CTR value */
    rijndaelEncrypt(rk, nrounds, ctrvalue, ciphertext);

    /* XOR the result into the file data */
    for (i = 0; i < nbytes; i++) {
      filedata[i] ^= ciphertext[i];
    }

    /* Write the result back to the file */
    nwritten = write(fd, filedata, nbytes);
    if (nwritten != nbytes)
    {
      fprintf (stderr,
	       "error writing the file (expected %d, got %d at ctr %d\n)",
	       nbytes, nwritten, ctr);
      break;
    }

    /* Increment the total bytes written */
    totalbytes += nbytes;
  }
  close (fd);
}

/* get_full_key
 * Returns a full 128-bit key (in the passed array key), created by
 * appending the bytes represented by usr_key to 8 '0' `bytes.
 */
u8* get_full_key(char* usr_key, u8* key) {
    int i;
    int usr_key_len;
    u8 byte;
    u32 tmp_long;
    
    /* Get the length of the user's key. */
    usr_key_len = strlen(usr_key);
    
    bzero(key, KEYLENGTH(KEYBITS));
    
    /* i's value persists after the first for loop */
    i = 0;
    /* Fill the first half of key with the bytes specified by the user.
     */
    for(; i < KEYLENGTH(KEYBITS) / 2; i++) {
        byte = 0;
        if (2 * i < usr_key_len) {
            byte = byte | (hexvalue(
                        usr_key[usr_key_len - 1 - 2 * i]));
        }
        if (2 * i + 1 < usr_key_len) {
            byte = byte | (hexvalue(
                        usr_key[usr_key_len - 1 - (2 * i + 1)]) << 4);
        }
        key[i] = byte;
    }
    
//    /* Fill the second half of key with the bytes determined by the
//     * inode number.
//     */
//    for(; i < KEYLENGTH(KEYBITS); i++) {
//        tmp_long = file_nr &
//                    (0xFF << (8 * (i - KEYLENGTH(KEYBITS) / 2)));
//        byte = tmp_long >> (8 * (i - KEYLENGTH(KEYBITS) / 2));
//        key[i] = byte;
//    }
    
    return key;
}

int main(int argc, char** argv) {
    int i;
    int error; /* Used to check the return value of function calls */

    if (argc != 4) {
        usage_exit(argv[0]);
    }
    
    /* The following if statement is to prevent unexpected behavior if
     * the first argument is not a mode character.
     */
    if (argv[1][1] != '\0') {
        usage_exit(argv[0]);
    }
    /* Get the mode */
    char mode = argv[1][0];
    if (mode != 'e' && mode != 'd') {
        usage_exit(argv[0]);
    }
    
    /* Get the user's part of the key. */
    if (strlen(argv[2]) > USR_KEY_BITS / 4) {
        fprintf(stderr, "Error: key too long.");
        usage_exit(argv[0]);
    }
    char input_key[strlen(argv[2]) + 1];
    strncpy(input_key, argv[2], strlen(argv[2]) + 1);
    
    /* Get the filename */
    char filename[strlen(argv[3]) + 1];
    strncpy(filename, argv[3], strlen(argv[3]) + 1);
    
    /* Get file info, used for the inode number and the sticky bit
     * status.
     */
    struct stat file_info;
    error = stat(filename, &file_info);
    if (error != 0) {
        fprintf(stderr, "Error accessing the file.");
        exit(EXIT_FAILURE);
    }
    ino_t file_nr = file_info.st_ino;
    int file_mode = file_info.st_mode;
}

/* get_full_key
 * Returns a full 128-bit key (in the passed array key), created by
 * appending the bytes represented by usr_key to the bytes derived from
 * file_nr.
 */
u8* get_full_key(char* usr_key, unsigned int file_nr, u8* key) {
    int i;
    int usr_key_len;
    u8 byte;
    u32 tmp_long;
    
    /* Get the length of the user's key. */
    usr_key_len = strlen(usr_key);
    
    bzero(key, KEYLENGTH(KEYBITS));
    
    /* i's value persists after the first for loop */
    i = 0;
    /* Fill the first half of key with the bytes specified by the user.
     */
    for(; i < KEYLENGTH(KEYBITS) / 2; i++) {
        byte = 0;
        if (2 * i < usr_key_len) {
            byte = byte | (hexvalue(
                        usr_key[usr_key_len - 1 - 2 * i]));
        }
        if (2 * i + 1 < usr_key_len) {
            byte = byte | (hexvalue(
                        usr_key[usr_key_len - 1 - (2 * i + 1)]) << 4);
        }
        key[i] = byte;
    }
    
    /* Fill the second half of key with the bytes determined by the
     * inode number.
     */
    for(; i < KEYLENGTH(KEYBITS); i++) {
        tmp_long = file_nr &
                    (0xFF << (8 * (i - KEYLENGTH(KEYBITS) / 2)));
        byte = tmp_long >> (8 * (i - KEYLENGTH(KEYBITS) / 2));
        key[i] = byte;
    }
    
    return key;
}

int main(int argc, char** argv) {
    int i;
    int error; /* Used to check the return value of function calls */

    if (argc != 4) {
        usage_exit(argv[0]);
    }
    
    /* The following if statement is to prevent unexpected behavior if
     * the first argument is not a mode character.
     */
    if (argv[1][1] != '\0') {
        usage_exit(argv[0]);
    }
    /* Get the mode */
    char mode = argv[1][0];
    if (mode != 'e' && mode != 'd') {
        usage_exit(argv[0]);
    }
    
    /* Get the user's part of the key. */
    if (strlen(argv[2]) > USR_KEY_BITS / 4) {
        usage_exit(argv[0]);
    }
    char input_key[strlen(argv[2]) + 1];
    strncpy(input_key, argv[2], strlen(argv[2]) + 1);
    
    /* Get the filename */
    char filename[strlen(argv[3]) + 1];
    strncpy(filename, argv[3], strlen(argv[3]) + 1);
    
    /* Get file info, used for the inode number and the sticky bit
     * status.
     */
    struct stat file_info;
    error = stat(filename, &file_info);
    if (error != 0) {
        usage_exit(argv[0]);
    }
    ino_t file_nr = file_info.st_ino;
    int file_mode = file_info.st_mode;
    int is_sticky = file_mode & S_ISVTX;
    
    /* Check if the file has already been en/decrypted */
    if ((!is_sticky) && mode == 'd') {
        fprintf(stderr, "Error: %s not encrypted.\n", filename);
        usage_exit(argv[0]);
    }
    if (is_sticky && mode == 'e') {
        fprintf(stderr, "Error: %s is already encrypted.\n", filename);
        usage_exit(argv[0]);
    }
    
    u8 key[KEYLENGTH(KEYBITS)];
    
    get_full_key(input_key, key);
    
    /* Clear the sticky bit before any encryption/decryption happens */
    file_info.st_mode = file_info.st_mode & (~S_ISVTX);
    error = chmod(filename, file_info.st_mode);
    if(error != 0) {
        fprintf(stderr, "Error accessing sticky bit.");
        exit(EXIT_FAILURE);
    }
    
    if (mode == 'e') {
        encrypt_file(filename, file_nr, key);
        
        file_info.st_mode = file_info.st_mode | S_ISVTX;
        error = chmod(filename, file_info.st_mode);
        if(error != 0) {
            fprintf(stderr, "Error accessing sticky bit.");
            exit(EXIT_FAILURE);
        }
    } else {
        encrypt_file(filename, file_nr, key);
    }
    
    
    
    return EXIT_SUCCESS;
}
