#include <ansi.h>

#ifndef __MFS_KEY_TABLE_H__
#define __MFS_KEY_TABLE_H__

#define MAX_KEY 8

typedef struct key_table *key_table_ref;
typedef struct key_entry *key_entry_ref;

int add_to_table(key_table_ref kt, uid_t userid, int key);
void print_table(key_table_ref kt);
int table_full (key_table_ref kt);      
void set_uid (key_entry_ref e, uid_t uid);
void set_key (key_entry_ref e,int key);
key_table_ref new_table(void);

#endif
