/*
 * This file was created for CMPS111 Sprint 2012 at UCSC
 * Authors: Benjamin Ross, Quentin Rivers, Matthew Musselman
 * This is the header file of the key_table structure. This
 * structure is used to store the keys set by users in the FS.
 * MAX_KEY is the maximum number of keys that can be stored
 */
#include <unistd.h>

#ifndef __MFS_KEY_TABLE_H__
#define __MFS_KEY_TABLE_H__

#define MAX_KEY 8

#define UID( _kt, _i ) ((kt)[(_i)][0])
#define K0( _kt, _i ) ((kt)[(_i)][1])
#define K1( _kt, _i ) ((kt)[(_i)][2])

#endif
