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
    int key;
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
    return(kt);
}

/* set_uid */
void set_uid (key_entry_ref e, uid_t uid)
{
    e->userid = uid;
}
uid_t get_uid(key_entry_ref e)
{
    return e->userid;
}
/* set_key */
void set_key (key_entry_ref e, int key)
{
    e->key = key;
}

int get_key(key_entry_ref e)
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
    key_entry_ref temp = kt->keys[0];
    for(i=0;i<kt->entries;++i)
    {
        if (kt->keys[i] != NULL)
            printf("uid: %d\tkey: %d\n",kt->keys[i]->userid,kt->keys[i]->key);
        kt->keys++; 
    }
    kt->keys[0] = temp;
}

/* add_to_table */
int add_to_table(key_table_ref kt, uid_t userid, int key)
{
    if(table_full(kt))
        /* Table is full. Cannot add to full table */
        return -1;
    else 
    {
        key_table_ref temp = kt;
        int i;
        for(i=0;i<kt->entries;++i);
        printf("i: %d\n",i);
        if(kt->keys[i] == NULL)
            kt->keys[i] = malloc(sizeof(key_entry_ref));
        set_uid(kt->keys[i],userid);
        set_key(kt->keys[i],key);
        kt->entries++;
        return 1;
    }
}
