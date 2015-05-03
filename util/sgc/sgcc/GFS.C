#include "genesis.h"
#include "gfs.h"
#include "psgsound.h"
#include <string.h>

ulong gfs_filecount;
gfs_dir_entry *gfs_directory;

void str_to_fname(fname, str)
char *fname;
char *str;
{
	register uint i;

	for(i = 0; (i != 8) && (*str != '.'); i++, fname++, str++){
		*fname = *str;
	}

	for( ; i != 8; i++, fname++){
		*fname = ' ';
	}

	for( ; i != 12; i++, fname++, str++){
		*fname = *str;
	}

	for( ; i != 12; i++, fname++){
		*fname = ' ';
	}
}

uint flashy_pal[16]={
0xEEE,
0x444,
0x888,
0xEEE,
0x004,
0x008,
0x00E,
0x040,
0x080,
0x0E0,
0x400,
0x800,
0xE00,
0x044,
0x088,
0x0EE
};

void init_GFS()
{
	gfs_filecount = *((ulong *)_GFS_START_OFFSET);
	gfs_directory = (gfs_dir_entry *)(_GFS_START_OFFSET+4UL);
}

gfs_dir_entry* find_GFS_dir_entry(fname)
char *fname;
{
	char filename[12];
	register uint max, min, i;
	register int tmp;
	char done;

	str_to_fname(filename, fname);

	min = 0;
	max = gfs_filecount;
	done = FALSE;
	while(!done){
		i = (max+min) >> 1;

		if(i == min){
			done = TRUE;
		}

		tmp = strncmp(gfs_directory[i].filename,filename,12);

		if(tmp < 0){
			min = i;
		}else if(tmp > 0){
			max = i;
		}else{
			done = TRUE;
		}
	}

	if(tmp == 0){
		return &gfs_directory[i];
	}else{
		return NULL;
	}
}

void get_GFS_fileaddr(faddr ,fname)
ulong *faddr;
char *fname;
{
	*faddr = find_GFS_dir_entry(fname)->offs+_GFS_START_OFFSET;
}

void get_GFS_filesize(fsize ,fname)
ulong *fsize;
char *fname;
{
	*fsize = find_GFS_dir_entry(fname)->len;
}

void get_GFS_file(faddr, fsize ,fname)
ulong *faddr;
ulong *fsize;
char *fname;
{
	gfs_dir_entry *entry;

	entry = find_GFS_dir_entry(fname);
	*faddr = entry->offs+_GFS_START_OFFSET;
	*fsize = entry->len;
}
