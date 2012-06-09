/*
 * This file was created for CMPS111 Sprint 2012 at UCSC
 * Authors: Benjamin Ross, Quentin Rivers, Matthew Musselman
 * This is the implementation file of key_table.h
 * Please refer to key_table.h for descriptions of the methods
 */
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "fs.h"
#include "key_table.h"
#include <stddef.h>
#include <string.h>
#include <minix/com.h>
#include <minix/u64.h>
#include "buf.h"
#include "inode.h"
#include "super.h"
#include <minix/vfsif.h>
/* Struct Definitions */
struct key_entry{
    uid_t userid;
    unsigned int key;
};

struct key_table {
    struct key_entry **keys;
    int entries;
};


/* new_table */
PUBLIC key_table_ref new_table(void)
{
    key_table_ref kt;
    kt = calloc(1, sizeof(key_table_ref));
    kt->entries = 0;
    kt->keys = calloc(MAX_KEY, sizeof(key_entry_ref));
    int i;
    for(i=0;i<MAX_KEY;i++)
        kt->keys[i] = malloc(sizeof(key_entry_ref));
    return(kt);
}

/* free_table */
PUBLIC void free_table(key_table_ref kt)
{
    int i;
    for(i=0;i<kt->entries;i++)
    {
        free(kt->keys[i]);
    }
    free(kt->keys);
    free(kt);
}

/* set_uid */
PUBLIC key_entry_ref set_uid (key_entry_ref e, uid_t uid)
{
    e->userid = uid;
    return e;
}

/* get_uid */
PUBLIC uid_t get_uid(key_entry_ref e)
{
    return e->userid;
}

/* set_key */
PUBLIC key_entry_ref set_key (key_entry_ref e, int key)
{

    e->key = key;
    return e;
}

/* get_key */
PUBLIC int get_key(key_entry_ref e)
{
    return e->key;
}

/* table_full */
PUBLIC int table_full (key_table_ref kt)
{
    if (kt->entries < MAX_KEY)
        return 0;
    else
        return 1;
}

/* print_table */
PUBLIC void print_table (key_table_ref kt)
{
    int i;
    for(i=0;i<kt->entries;++i)
    {
        printf("%d:\tuid: %d\tkey: %d\n",i,kt->keys[i]->userid,kt->keys[i]->key);
    }
}

/* add_to_table */
PUBLIC int add_to_table(key_table_ref kt, uid_t userid, int key)
{
    if(table_full(kt))
        /* Table is full. Cannot add to full table */
        return -1;
    else 
    {
        int u_key = get_key_by_uid(kt,userid);
        if ( u_key != NULL )
        {
            int i;
            for(i = 0;i<kt->entries;i++)
            {
                if(get_uid(kt->keys[i]) == userid)
                    kt->keys[i] = set_key(kt->keys[i],key);
            } 
            printf("Key Updated\n");
        }
        else {
            kt->keys[kt->entries] = set_uid(kt->keys[kt->entries],userid);
            kt->keys[kt->entries] = set_key(kt->keys[kt->entries],key);
            kt->entries++;
        }
        return 1;
    }
}

PUBLIC int get_key_by_uid(key_table_ref kt, uid_t id)
{
    int i;
    printf("UID to look for: %d\n",id);
    for(i = 0;i<kt->entries;i++)
    {
        printf("Key: %d\n",get_key(kt->keys[i]));
        if(get_uid(kt->keys[i]) == id)
            return get_key(kt->keys[i]);
    }
    //not found
    return NULL;
}
