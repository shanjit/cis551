#define BSD
#ifndef BSD
/*
 * these routines kludge compatibility
 * between Sys V and 4.2 and later BSDs.
 */

#ifdef u3b2
# include <sys/fs/s5dir.h>
#else
# include <sys/dir.h>
#endif

typedef struct _dirdesc {
  	int dd_fd;
  	long dd_loc;
	long dd_size;
	long dd_bbase;
	long dd_entno;
	long dd_bsize;
	char *dd_buf;
} DIR;

extern MYDIR *opendir();
extern struct direct *readdir();
extern long telldir();
extern void seekdir();
#define rewinddir(dirp) seekdir( (dirp), (long) 0)
extern void closedir();

#endif
