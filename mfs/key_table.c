#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "fs.h"
#include "key_table.h"

struct key_entry{
    uid_t userid;
    int key;
};

struct key_table {
    struct key_entry **keys;
    int entries;
};

key_table_ref new_table(void)
{
    key_table_ref kt;
    kt = calloc(1, sizeof(key_table_ref));
    kt->entries = 0;
    kt->keys = calloc(1, (sizeof(key_entry_ref) * MAX_KEY));
    return(kt);
}

void set_uid (key_entry_ref e, uid_t uid)
{
    e->userid = uid;
}

void set_key (key_entry_ref e, int key)
{
    e->key = key;
}

int table_full (key_table_ref kt)
{
    if (kt->entries < 8)
        return 0;
    else
        return 1;
}

void print_table (key_table_ref kt)
{
    int i;
    key_table_ref temp = kt->keys;
    for(i=0;i<kt->entries;++i)
    {
        printf("uid: %d\tkey: %d\n",kt->keys->userid,kt->keys->key);
        kt->keys++; 
    }
    kt->keys = temp;
}

int add_to_table(key_table_ref kt, uid_t userid, int key)
{
    if(table_full(kt))
        return -1;
    else 
    {
        key_table_ref temp = kt;
        int i;
        for(i=0;i<kt->entries;++i)
            kt->keys++;
        set_uid(kt->keys,userid);
        set_key(kt->keys,key);
        return 1;
    }
}
