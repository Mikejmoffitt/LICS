/* Rotinas para acesso ao Sega Genesis File System */
/* por Haroldo O. Pinheiro                         */

#ifndef _INCLUDE_GFS
#define _INCLUDE_GFS

#include "genesis.h"

#define _GFS_START_OFFSET 0x40000UL

typedef struct{
	char filename[12];
	uint filler;
	ulong offs;
	ulong len;
}gfs_dir_entry;

void init_GFS();
gfs_dir_entry* find_GFS_dir_entry(fname);
void get_GFS_fileaddr(faddr, fname);
void get_GFS_filesize(fsize ,fname);
void get_GFS_file(faddr, fsize ,fname);

#endif
