/*
 * This file was created for CMPS111 Sprint 2012 at UCSC
 * Authors: Benjamin Ross, Quentin Rivers, Matthew Musselman
 * This is the header file of the key_table structure. This
 * structure is used to store the keys set by users in the FS.
 * MAX_KEY is the maximum number of keys that can be stored
 */
#include <unistd.h>

#ifndef __MFS_KEY_TABLE_H__
#define __MFS_KEY_TABLE_H__

#define MAX_KEY 8

typedef unsigned long u32;
typedef unsigned char u8;

//typedef struct key_entry *key_entry_ref;
typedef u32 **key_table_ref;


/* new_table */
key_table_ref init_table(key_table_ref kt);

// /* free_table */
// void free_table(key_table_ref kt);

/* set_uid */
int set_uid (key_table_ref kt, int u_index, uid_t uid);

/* get_uid */
uid_t get_uid(key_table_ref kt, int u_index);

/* set_key */
int set_key (key_table_ref kt, int u_index, u32 k0, u32 k1);

/* get_key */
u8* get_key(key_table_ref kt, u8* key, int u_index);

/* is_empty */
int is_empty (key_table_ref kt, int i);

/* table_full */
int table_full (key_table_ref kt);

/* print_table */
void print_table (key_table_ref kt);

int has_key(key_table_ref kt, uid_t userid);

/* remove_from_table */
int remove_from_table(key_table_ref kt, uid_t userid);
            
/* add_to_table */
int add_to_table(key_table_ref kt, uid_t userid, u32 k0, u32 k1);

unsigned char* get_key_by_uid(key_table_ref kt, uid_t id);
#endif
