#include <stdlib.h>
#include <stdio.h>
#include "key_table.h"
int main(int argc, char**argv){
	
	
    printf("Beginning Key Table Test\n");

    printf("Testing set_uid\n");

    uid_t cur_uid = getuid();
    key_entry_ref e = malloc(sizeof(key_entry_ref));
    set_uid(e,cur_uid);
    printf("Verification: Cur_UID: %d\t Entry UID: %d\n",cur_uid,get_uid(e));

    printf("Testing set_key\n");
    int key = 1337;
    set_key(e,key);
    printf("Verification: Key: %d\t Entry Key: %d\n",key,get_key(e));

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

printf("Testing add_to_table\n");
    check = add_to_table(kt,cur_uid,key);
    printf("Check: %d\n",check);
    print_table(kt);
printf("Testing add_to_table\n");
    check = add_to_table(kt,cur_uid,key);
    printf("Check: %d\n",check);
    print_table(kt);
printf("Testing add_to_table\n");
    check = add_to_table(kt,cur_uid,key);
    printf("Check: %d\n",check);
    print_table(kt);
printf("Testing add_to_table\n");
    check = add_to_table(kt,cur_uid,key);
    printf("Check: %d\n",check);
    print_table(kt);
printf("Testing add_to_table\n");
    check = add_to_table(kt,cur_uid,key);
    printf("Check: %d\n",check);
    print_table(kt);
printf("Testing add_to_table\n");
    check = add_to_table(kt,cur_uid,key);
    printf("Check: %d\n",check);
    print_table(kt);
printf("Testing add_to_table\n");
    check = add_to_table(kt,cur_uid,key);
    printf("Check: %d\n",check);
    print_table(kt);

	return 0;
}

