/*
 * This file was created for CMPS111 Sprint 2012 at UCSC
 * Authors: Benjamin Ross, Quentin Rivers, Matthew Musselman
 * This is the implementation file of key_table.h
 * Please refer to key_table.h for descriptions of the methods
 */
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
//#include "fs.h"
#include "key_table.h"

/* Struct Definitions */
struct key_entry{
    uid_t userid;
    unsigned char *key;
};

struct key_table {
    struct key_entry **keys;
    int entries;
};


/* new_table */
key_table_ref new_table(void)
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
void free_table(key_table_ref kt)
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
key_entry_ref set_uid (key_entry_ref e, uid_t uid)
{
    e->userid = uid;
    return e;
}

/* get_uid */
uid_t get_uid(key_entry_ref e)
{
    return e->userid;
}

/* set_key */
key_entry_ref set_key (key_entry_ref e, unsigned char* key)
{
    e->key = key;
    return e;
}

/* get_key */
unsigned char * get_key(key_entry_ref e)
{
    return e->key;
}

/* table_full */
int table_full (key_table_ref kt)
{
    if (kt->entries < MAX_KEY)
        return 0;
    else
        return 1;
}

/* print_table */
void print_table (key_table_ref kt)
{
    int i;
    for(i=0;i<kt->entries;++i)
    {
        printf("%d:\tuid: %d\tkey: %s\n",i,kt->keys[i]->userid,kt->keys[i]->key);
    }
}

/* del_from_table */
int add_to_table(key_table_ref kt, uid_t userid)
{
    for(i = 0;i<MAX_KEY;i++)
    {
        if(kt->keys[i] != NULL && get_uid(kt->keys[i]) == userid)
        {
            kt->keys[i] = NULL;
            printf ("Key deleted\n");
            return 1;
        }
    }
    /* if we get here, the table doesn't contain an entry for the user */
    return -1;
}
            
/* add_to_table */
int add_to_table(key_table_ref kt, uid_t userid, unsigned char* key)
{
    if(table_full(kt))
        /* Table is full. Cannot add to full table */
        return -1;
    else 
    {
        unsigned char * u_key = get_key_by_uid(kt,userid);
        if ( u_key != NULL )
        {
            int i;
            for(i = 0;i<MAX_KEY;i++)
            {
                if(kt->keys[i] != NULL && get_uid(kt->keys[i]) == userid)
                    kt->keys[i] = set_key(kt->keys[i],key);
            } 
            printf("Key Updated\n");
        }
        else {
            for(i = 0;i<MAX_KEY;i++)
            {
                if(kt->keys[i] == NULL)
                {
                    kt->keys[i] = set_uid(kt->keys[kt->entries],userid);
                    kt->keys[i] = set_key(kt->keys[kt->entries],key);
                    kt->entries++;
                    break;
                }
            }
        }
        return 1;
    }
}

unsigned char* get_key_by_uid(key_table_ref kt, uid_t id)
{
    int i;
    for(i = 0;i<kt->entries;i++)
    {
        if(kt->keys[i] != NULL && get_uid(kt->keys[i]) == id)
            return get_key(kt->keys[i]);
    }
    //not found
    return NULL;
}
