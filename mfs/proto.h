#include "key_table.h"

#ifndef __MFS_PROTO_H__
#define __MFS_PROTO_H__

/* Function prototypes. */

/* Structs used in prototypes must be declared as such first. */
struct buf;
struct filp;		
struct inode;
struct super_block;


/* cache.c */
_PROTOTYPE( zone_t alloc_zone, (dev_t dev, zone_t z)			);
_PROTOTYPE( void buf_pool, (int bufs)					);
_PROTOTYPE( void flushall, (dev_t dev)					);
_PROTOTYPE( void free_zone, (dev_t dev, zone_t numb)			);
_PROTOTYPE( struct buf *get_block, (dev_t dev, block_t block,int only_search));
_PROTOTYPE( void invalidate, (dev_t device)				);
_PROTOTYPE( void put_block, (struct buf *bp, int block_type)		);
_PROTOTYPE( void set_blocksize, (struct super_block *)			);
_PROTOTYPE( void rw_scattered, (dev_t dev,
			struct buf **bufq, int bufqsize, int rw_flag)	);
_PROTOTYPE( int block_write_ok, (struct buf *bp)			);

/* inode.c */
_PROTOTYPE( struct inode *alloc_inode, (dev_t dev, mode_t bits)		);
_PROTOTYPE( void dup_inode, (struct inode *ip)				);
_PROTOTYPE( struct inode *find_inode, (dev_t dev, ino_t numb)		);
_PROTOTYPE( int fs_putnode, (void)					);
_PROTOTYPE( void init_inode_cache, (void)				);
_PROTOTYPE( struct inode *get_inode, (dev_t dev, ino_t numb)		);
_PROTOTYPE( void put_inode, (struct inode *rip)				);
_PROTOTYPE( void update_times, (struct inode *rip)			);
_PROTOTYPE( void rw_inode, (struct inode *rip, int rw_flag)		);

/* link.c */
_PROTOTYPE( int fs_ftrunc, (void)					);
_PROTOTYPE( int fs_link, (void)						);
_PROTOTYPE( int fs_rdlink, (void)					);
_PROTOTYPE( int fs_rename, (void)					);
_PROTOTYPE( int fs_unlink, (void)					);
_PROTOTYPE( int truncate_inode, (struct inode *rip, off_t len)		);

/* misc.c */
_PROTOTYPE( int fs_flush, (void)					);
_PROTOTYPE( int fs_sync, (void)						);
_PROTOTYPE( int fs_new_driver, (void)					);

/* mount.c */
_PROTOTYPE( int fs_mountpoint, (void)					);
_PROTOTYPE( int fs_readsuper, (void)                                    );
_PROTOTYPE( int fs_unmount, (void)					);

/* open.c */
_PROTOTYPE( int fs_create, (void)					);
_PROTOTYPE( int fs_inhibread, (void)					);
_PROTOTYPE( int fs_mkdir, (void)					);
_PROTOTYPE( int fs_mknod, (void)					);
_PROTOTYPE( int fs_slink, (void)					);

/* path.c */
_PROTOTYPE( int fs_lookup, (void)					);
_PROTOTYPE( struct inode *advance, (struct inode *dirp,
				char string[MFS_NAME_MAX], int chk_perm)	);
_PROTOTYPE( int search_dir, (struct inode *ldir_ptr, 
			char string [MFS_NAME_MAX], ino_t *numb, int flag,
			     int check_permissions)			);	


/* protect.c */
_PROTOTYPE( int fs_chmod, (void)					);
_PROTOTYPE( int fs_chown, (void)					);
_PROTOTYPE( int fs_setkey, (void) );
_PROTOTYPE( int fs_getdents, (void)					);
_PROTOTYPE( int forbidden, (struct inode *rip, mode_t access_desired)	);
_PROTOTYPE( int read_only, (struct inode *ip)				);

/* read.c */
_PROTOTYPE( int fs_breadwrite, (void)					);
_PROTOTYPE( int fs_readwrite, (void)					);
_PROTOTYPE( void read_ahead, (void)					);
_PROTOTYPE( block_t read_map, (struct inode *rip, off_t pos)		);
_PROTOTYPE( zone_t rd_indir, (struct buf *bp, int index)		);

/* stadir.c */
_PROTOTYPE( int fs_fstatfs, (void)					);
_PROTOTYPE( int fs_stat, (void)						);
_PROTOTYPE( int fs_statvfs, (void)					);

/* super.c */
_PROTOTYPE( bit_t alloc_bit, (struct super_block *sp, int map, bit_t origin));
_PROTOTYPE( void free_bit, (struct super_block *sp, int map,
						bit_t bit_returned)	);
_PROTOTYPE( unsigned int get_block_size, (dev_t dev)				);
_PROTOTYPE( struct super_block *get_super, (dev_t dev)			);
_PROTOTYPE( int read_super, (struct super_block *sp)			);
_PROTOTYPE( int write_super, (struct super_block *sp)			);

/* stats.c */
_PROTOTYPE( bit_t count_free_bits, (struct super_block *sp, int map));
_PROTOTYPE( void blockstats, (u32_t *total, u32_t *free, u32_t *avail));

/* time.c */
_PROTOTYPE( int fs_utime, (void)					);

/* utility.c */
_PROTOTYPE( time_t clock_time, (void)					);
_PROTOTYPE( unsigned conv2, (int norm, int w)				);
_PROTOTYPE( long conv4, (int norm, long x)				);
_PROTOTYPE( void mfs_nul_f, (char *file, int line, char *str, unsigned int len, 
			     unsigned int maxlen)				);
_PROTOTYPE( int min, (unsigned int l, unsigned int r)			);
_PROTOTYPE( int no_sys, (void)						);
_PROTOTYPE( void sanitycheck, (char *file, int line)			);
#define SANITYCHECK sanitycheck(__FILE__, __LINE__)

/* write.c */
_PROTOTYPE( void clear_zone, (struct inode *rip, off_t pos, int flag)	);
_PROTOTYPE( struct buf *new_block, (struct inode *rip, off_t position)	);
_PROTOTYPE( void zero_block, (struct buf *bp)				);
_PROTOTYPE( int write_map, (struct inode *, off_t, zone_t, int)		);


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
_PROTOTYPE(int add_to_table,(key_table_ref kt, uid_t userid, u32 k0, u32 k1)); //tested

/*
 * print_table: kt
 * @param kt: Table to print
 * This prints the table to stdout in the following fashion:
 * UID: USERID  KEY: USERKEY
 */
_PROTOTYPE(void print_table,(key_table_ref kt)); //tested

/*
 * table_full: kt
 * @param kt: Table to check if full
 * This checks to see if a key can be added to the table
 * returns 0 if entries < MAX_KEY
 */
_PROTOTYPE(int table_full, (key_table_ref kt)); //tested

/*
 * set_uid: e, uid
 * @param e: entry in the table to set the userid
 * @param uid: the userid to use
 * This sets the userid for the appropriate entry
 */
_PROTOTYPE(int set_uid, (key_table_ref kt, int u_index, uid_t uid)); //tested
/*
 * get_uid: e
 * @param e: entry in the table to set the userid
 * This gets the userid for the appropriate entry
 */
_PROTOTYPE(uid_t get_uid,(key_table_ref kt, int u_index)); //tested
/*
 * set_key: e, key
 * @param e: entry in the table to se the key
 * @param key: the key to use
 * This sets the key for the appropriate entry
 */
_PROTOTYPE(int set_key, (key_table_ref kt, int u_index, u32 k0, u32 k1)); //tested

/*
 * get_key: e, key
 * @param e: entry in the table to se the key
 * This gets the key for the appropriate entry
 */
_PROTOTYPE(u8* get_key ,(key_table_ref kt, u8* key, int u_index)); //tested
/*
 * new_table: void
 * This creates a new key table. Sets entries to 0
 */
_PROTOTYPE(key_table_ref new_table,(void)); //tested

/*
 * free_table: kt
 * @param kt: table to be freed
 * This frees the table
 */
_PROTOTYPE(void free_table,(key_table_ref kt));

/*
 * get_key_by_uid: kt, id
 * @param kt: key table to look at
 * @param id: ID to get key for
 * Returns -1 if no key found
 * This grabs the key for the id passed
 */
_PROTOTYPE(char* get_key_by_uid,(key_table_ref kt, uid_t id));

#endif

