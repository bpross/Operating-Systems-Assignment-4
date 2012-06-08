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

typedef struct key_table *key_table_ref;
typedef struct key_entry *key_entry_ref;

/*
 * add_to_table: kt, userid, key
 * @param kt: Key table to add key to
 * @param userid: userid to add to the table
 * @param key: key to map to the user
 * This method is used to add a user/key to the table
 * IT DOES NOT CHECK IF THE USER HAS ANOTHER KEY
 * (this will be added if we have time, but the spec does
 * not call for this)
 */
int add_to_table(key_table_ref kt, uid_t userid, unsigned char* key); //tested

/*
 * print_table: kt
 * @param kt: Table to print
 * This prints the table to stdout in the following fashion:
 * UID: USERID  KEY: USERKEY
 */
void print_table(key_table_ref kt); //tested

/*
 * table_full: kt
 * @param kt: Table to check if full
 * This checks to see if a key can be added to the table
 * returns 0 if entries < MAX_KEY
 */
int table_full (key_table_ref kt); //tested

/*
 * set_uid: e, uid
 * @param e: entry in the table to set the userid
 * @param uid: the userid to use
 * This sets the userid for the appropriate entry
 */
key_entry_ref set_uid (key_entry_ref e, uid_t uid); //tested
/*
 * get_uid: e
 * @param e: entry in the table to set the userid
 * This gets the userid for the appropriate entry
 */
uid_t get_uid(key_entry_ref e); //tested
/*
 * set_key: e, key
 * @param e: entry in the table to se the key
 * @param key: the key to use
 * This sets the key for the appropriate entry
 */
key_entry_ref set_key (key_entry_ref e,char* key); //tested

/*
 * get_key: e, key
 * @param e: entry in the table to se the key
 * This gets the key for the appropriate entry
 */
char * get_key (key_entry_ref e); //tested
/*
 * new_table: void
 * This creates a new key table. Sets entries to 0
 */
key_table_ref new_table(void); //tested

/*
 * free_table: kt
 * @param kt: table to be freed
 * This frees the table
 */
void free_table(key_table_ref kt);

/*
 * get_key_by_uid: kt, id
 * @param kt: key table to look at
 * @param id: ID to get key for
 * Returns -1 if no key found
 * This grabs the key for the id passed
 */
char* get_key_by_uid(key_table_ref kt, uid_t id);
#endif
