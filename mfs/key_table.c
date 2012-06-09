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

#define UID( _kt, _i ) ((kt)[(_i)][0])
#define K0( _kt, _i ) ((kt)[(_i)][1])
#define K1( _kt, _i ) ((kt)[(_i)][2])

u32 **key_table;

/* new_table */
key_table_ref init_table(key_table_ref kt)
{
    
    int i;
    for(i=0;i<MAX_KEY;i++)
    {
        K0(kt, i) = 0;
        K1(kt, i) = 0;
    }
    
    kt = key_table;
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
u8* get_key(key_table_ref kt, u8* key, int u_index)
{
    u32 k0 = K0(kt, u_index);
    u32 k1 = K1(kt, u_index);
    memset(&key, 0, 16);
    bcopy (&k0, &(key[0]), sizeof (k0));
    bcopy (&k1, &(key[sizeof(k0)]), sizeof (k1));
    return key;
}

/* is_empty */
int is_empty (key_table_ref kt, int i)
{
    return (K0(kt, i) == NULL && K1(kt, i) == NULL);
}

/* table_full */
int table_full (key_table_ref kt)
{
    int i;
    for (i = 0; i < MAX_KEY; i++) {
        if (is_empty (i))
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
                    i,UID(kt,i),K0(kt,i), K!(kt,i));
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
    for(i = 0;i<MAX_KEY;i++)
    {
        if(kt->keys[i] != NULL && get_uid(kt->keys[i]) == userid)
        {
            kt->keys[i] = NULL;
            kt->entries--;
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
