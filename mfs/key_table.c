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

#define UID( _kt, _i ) ((kt)[(_i)][0])
#define K0( _kt, _i ) ((kt)[(_i)][1])
#define K1( _kt, _i ) ((kt)[(_i)][2])

/* new_table */
void init_table(key_table_ref kt)
{
    int i;
    for(i=0;i<MAX_KEY;i++)
    {
        UID(kt,i) = -1;
        K0(kt, i) = 0;
        K1(kt, i) = 0;
    }
    
    return(kt);
}

// /* free_table */
// void free_table(key_table_ref kt)
// {
    // int i;
    // for(i=0;i<kt->entries;i++)
    // {
        // free(kt->keys[i]);
    // }
    // free(kt->keys);
    // free(kt);
// }

/* set_uid */
int set_uid (key_table_ref kt, int u_index, uid_t uid)
{
    UID(kt, u_index) = uid;
    return u_index;
}

/* get_uid */
uid_t get_uid(key_table_ref kt, int u_index)
{
    return UID(kt, u_index);
}

/* set_key */
int set_key (key_table_ref kt, int u_index, u32 k0, u32 k1)
{
    K0(kt, u_index) = k0;
    K1(kt, u_index) = k1;
    return u_index;
}

/* get_key */
/*u8* get_key(key_table_ref kt, int u_index, u8* key)
{
    u32 k0 = K0(kt, u_index);
    u32 k1 = K1(kt, u_index);
    memset(&key, 0, sizeof(key));
    bcopy (&k0, &(key[0]), sizeof (k0) * 8);
    bcopy (&k1, &(key[sizeof(k0)*8]), sizeof (k1)*8);
    return key;
}
*/

/* is_empty */
int is_empty (key_table_ref kt, int i)
{
    return (K0(kt, i) == 0 && K1(kt, i) == 0);
}

/* table_full */
int table_full (key_table_ref kt)
{
    int i;
    for (i = 0; i < MAX_KEY; i++) {
        if (is_empty (kt,i))
            return 0;
    }
    return 1;
}

/* print_table */
void print_table (key_table_ref kt)
{
    int i;
    for(i=0;i<MAX_KEY;++i)
    {
        printf("%d:\tuid: %d\tk0: %80x\tk1: %80x\n",
                i,UID(kt,i),K0(kt,i), K1(kt,i));
    }
}

int has_key(key_table_ref kt, uid_t userid)
{
    int i;
    for(i = 0; i < MAX_KEY; i++)
    {
        if(!is_empty(kt, i) && ((uid_t) UID(kt, i)) == userid)
            return 1;
    }
    
    return 0; /* Didn't find it */
}

/* remove_from_table */
int remove_from_table(key_table_ref kt, uid_t userid)
{
    int i;
    for(i = 0;i<MAX_KEY;i++)
    {
        if(UID(kt,i) != -1 && get_uid(kt,i) == userid)
        {
            UID(kt,i) = -1;
            K0(kt,i) = 0;
            K1(kt,i) = 0;
            printf ("Key deleted\n");
            return 1;
        }
    }
    /* if we get here, the table doesn't contain an entry for the user */
    return -1;
}
            
/* add_to_table */
int add_to_table(key_table_ref kt, uid_t userid, u32 k0, u32 k1)
{
    if(table_full(kt))
        /* Table is full. Cannot add to full table */
        return -1;
    else 
    {
        int i;
        for(i = 0; i < MAX_KEY; i++)
        {
            if(get_uid(kt, i) == userid || is_empty(kt, i)) {
                UID(kt, i) = userid;
                K0(kt, i) = k0;
                K1(kt, i) = k1;
            }
        }
        return 1;
    }
}

u8* get_key_by_uid(key_table_ref kt, uid_t userid, u8* key)
{
    int i;
    for(i = 0; i < MAX_KEY; i++)
    {
        if(!is_empty(kt, i) && get_uid(kt, i) == userid)
            return get_key(kt, i, key);
    }
    //not found
    return NULL;
}
