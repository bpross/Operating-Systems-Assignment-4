#include <stdlib.h>
#include <stdio.h>
#include "mfs/key_table.h"

int main(int argc, char**argv){
	
	
    printf("Beginning Key Table Test\n");

    printf("Testing set_uid\n");

    uid_t cur_uid = getuid();
    key_entry_ref e = malloc(sizeof(key_entry_ref));
    set_uid(e,cur_uid);
    printf("Verification: Cur_UID: %d\t Entry UID: %d\n",cur_uid,get_uid(e));

    printf("Testing set_key\n");
    char key[128] = "asdfasdfasdfasd";
    set_key(e,key);
    printf("Verification: Key: %s\t Entry Key: %s\n",key,get_key(e));

    free(e); //we are no longer going to use this

    printf("Testing new_table\n");
    key_table_ref kt = new_table();

    printf("Testing table_full\n");
    int tablefull = table_full(kt);
    printf("Tablefull: %d\n",tablefull);

    printf("Testing print_table\n");
    print_table(kt);

    printf("Testing add_to_table\n");
    int check = add_to_table(kt,cur_uid,key);
    printf("Check: %d\n",check);
    print_table(kt);
    // Going to fill up the table
    /*
    printf("Testing add_to_table\n");
    check = add_to_table(kt,cur_uid+1,key+1);
    printf("Check: %d\n",check);

    printf("Testing add_to_table\n");
    check = add_to_table(kt,cur_uid+2,key+2);
    printf("Check: %d\n",check);

    printf("Testing add_to_table\n");
    check = add_to_table(kt,cur_uid+3,key+3);
    printf("Check: %d\n",check);

    printf("Testing add_to_table\n");
    check = add_to_table(kt,cur_uid+4,key+4);
    printf("Check: %d\n",check);

    printf("Testing add_to_table\n");
    check = add_to_table(kt,cur_uid+5,key+5);
    printf("Check: %d\n",check);

    printf("Testing add_to_table\n");
    check = add_to_table(kt,cur_uid+6,key+6);
    printf("Check: %d\n",check);

    printf("Testing add_to_table\n");
    check = add_to_table(kt,cur_uid+7,key+7);
    printf("Check: %d\n",check);
    print_table(kt);

    // This should return -1, because the table is full
    printf("Testing add_to_table\n");
    check = add_to_table(kt,cur_uid,key+7);
    printf("Check: %d\n",check);
    print_table(kt);

    // Testing get_key_by_uid
    // This one should work
    printf("Testing get_key_by_uid\n");
    int test_key = get_key_by_uid(kt,cur_uid);   
    printf("Key for user %d: %d\n",cur_uid,test_key);

    // This one should NOT work
    test_key = get_key_by_uid(kt,1);
    printf("Key for user %d: %d\n",cur_uid,test_key);

    // Testing free
    printf("Testing free table\n");
    free_table(kt);
    */
	return 0;
}

