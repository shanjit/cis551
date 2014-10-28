/* 
 * this file contains routines which deal with directories 
 * and pathnames.
 *
 */

#include "io.h"

#include <dirent.h>

#include "stack.h"

extern char *push(), *pop();

extern char
  	*strsave(),
	*emalloc();

char 
	*make_prefix(),
	*next_dir(),
	*remaining_path(),
	*make_name(),
	*get_dir_match();

/*
 * do_dir:
 * complex recursive routine which actually does most of the
 * work of file name generation/ pattern expansion.
 * the basic idea is that the current values of prefix and postfix
 * tell us where we are with respect to completion of the expansion
 * which started via a call by expand() to do_dir().
 * The terminal condition should probably be something to do
 * with "pattern", since it's really what we're worried about.
 * However, the test for "postfix" being empty ("") is also correct, and
 * allows a somewhat subtle optimization of the recursion.
 */

void
do_dir( prefix, postfix, pattern )
char *prefix, *postfix, *pattern;
{
	DIR *dir_fd;
	char *p;
	struct stack local_stack;
	/* extern int ErrFlag; ???? */

	if( *prefix == EOS )
	  	dir_fd = opendir( "." );
	else
		dir_fd = opendir( prefix );

	if( dir_fd == (DIR *) NULL )
	{
		perror( prefix );
		/* ErrFlag = TRUE; */
	}

	/*
 	 * here, we iterate on the matches found by get_dir_match(),
	 * and push them onto our local directory stack.
	 */

	local_stack.depth = 0;	/* initialize properly */
	local_stack.list = (struct list *) NULL;

	while( (p=get_dir_match( dir_fd, pattern )) != (char *) NULL )
	{
		push( p, &local_stack );
	}

	closedir( dir_fd );


	/*
	 * we either terminate here, by pushing stuff onto the final 
 	 * directory stack (a global, Final), or continue in the next section
	 * of code, to call do_dir(), with new prefixes, postfixes, and
	 * patterns.
	 */

	if( *postfix == EOS )
	{
		while( (p=pop( &local_stack ) ) != (char *) NULL )
		{
			push( make_name( prefix, p ), &Final );
			safe_free( p );
		}
		
		return;
	}

	/*
	 * if we've gotten here, there's some path remaining; we must
	 * continue the recursion. We only include directories
	 * in further searching, for obvious reasons.
	 */

	while( (p=pop( &local_stack )) != (char *) NULL )
	{
		char *newprefix, *newpostfix, *newpat;

		newprefix = make_prefix( prefix, p );
		newpostfix = remaining_path( postfix );
		newpat = next_dir( postfix );


		if( good_dir( newprefix ) == TRUE )
			do_dir( newprefix, newpostfix, newpat );

		safe_free( newprefix );
		safe_free( newpostfix );
		safe_free( newpat );
		safe_free( p );
	}

	return;
}



/*
 * get_dir_match()
 * returns the next valid directory entry found on the directory
 * pointed to by dir_fd which matches (according to whatever
 * rules the match function uses) the pattern in match_string.
 */

char *
get_dir_match( dir_fd, match_string )
DIR *dir_fd;
char *match_string;
{
  struct dirent *dp, *readdir();

	if( dir_fd == (DIR *) NULL )
	  	return( (char *) NULL );

	for( dp = readdir( dir_fd ); dp != NULL;
		dp = readdir( dir_fd ) )
	{
		if( dp->d_ino == 0 )
			continue;
#define DOT '.'
		/* DOT gets special handling as first character */
		if( (dp->d_name[0] == DOT ) && ( match_string[0] != DOT ) )
		  	continue;
		if( match( match_string, dp->d_name ) )
		{
			return( strsave(dp->d_name) );
		}
	}

	return( (char *) NULL );
}

/*
 * pathname routines 
 */


/*
 * next_dir:
 * takes a pathname of the form "a/b/c"
 * and returns "a" (actually a copy)
 * given "/a/b", returns "a".
 */


char *next_dir( name )
char *name;
{
	char *ptr, *save;

	if( *name == '/' )
		save = strsave( name+1 );
	else
		save = strsave( name );
	ptr = find_slash( save );

	if( ptr != (char *) NULL )
		*ptr = EOS;

	return( save );
}

/*
 * remaining_path:
 * takes a pathname of the form "a/b/c"
 * and returns a pathname of the form "/b/c".
 * takes a pathname of the form "/a/b/c"
 * nad returns a pathname of the form "/b/c".
 */

char *
remaining_path( name )
char *name;
{
	char *ptr;

	if( *name == '/' )
		ptr = find_slash( name+1 );
	else
		ptr = find_slash( name );

	if( ptr != (char *) NULL )
	{
		/* this adjusts //////z to /z	*/
		while( *(ptr+1) == '/' ) /* should use #define for '/' */
			++ptr;

		ptr = strsave( ptr );
	}
	else
	  	ptr = strsave(""); /* so we can strcpy(), etc. w/ impunity */

	return( ptr );
}


/*
 * make_name:
 * takes a "prefix" of the form "a/b/",
 * a name of the form "c", and returns a pathname of the form
 * "a/b/c".
 */

char *
make_name( prefix, name )
char *prefix, *name;
{
	int length;
	char *p;
  
	length = strlen( name ) + strlen( prefix ) + 1 ;
	p = emalloc( (unsigned) length );
	strcpy( p, prefix );
	strcat( p, name );

	return( p );
}

/*
 * make_prefix:
 * takes a prefix of the form "a/b/",
 * a name of the form "c", and returns a new prefix of the form
 * "a/b/c/".
 * same code as make_name(), but adds in a "/".
 */

char *
make_prefix( prefix, name )
char *prefix, *name;
{
	int length;
	char *p;
  
	length = strlen( name ) + strlen( prefix ) + 2;
	p = emalloc( (unsigned) length );
	strcpy( p, prefix );
	strcat( p, name );
	strcat( p, "/" );

	return( p );
}


	

#if 0

/*
 * C routines to implement Berkeley style directory
 * manipulation stuff.
 */

DIR *
opendir( filename )
char *filename;
{
  	char *malloc();
	DIR *dirp;

	dirp = (DIR *) malloc( (unsigned) sizeof( DIR ) );
	if( dirp != (DIR *) NULL )
	{
		struct stat sb;
		int nbytes;

		dirp->dd_fd = open( filename, 0 );
		if( dirp->dd_fd < 0 )
		{
			free( dirp );
			return( (DIR *) NULL );
		}

		fstat( dirp->dd_fd, &sb );
		dirp->dd_size = dirp->dd_bsize = sb.st_size;
		dirp->dd_buf = malloc( (unsigned) dirp->dd_bsize );
		if( dirp->dd_buf == (char *) NULL )
		{
			close( dirp->dd_fd );
			free( dirp );
			return( (DIR *) NULL );
		}

		/* 
		 * Read in the entire directory.
		 * ?should this be in readdir()? 
		 */
		nbytes = read( dirp->dd_fd, dirp->dd_buf, dirp->dd_bsize );

		if( nbytes < dirp->dd_bsize )
		{
			close( dirp->dd_fd );
			free( dirp->dd_buf );
			free( dirp );
			return( (DIR *) NULL );
		}

		/* should this be in closedir()?
		 * It's not needed after that big read...
		 */
		close( dirp->dd_fd );

		dirp->dd_loc = 0L;
		dirp->dd_entno = 0L;
	}

	return( dirp );	
}

struct direct *
readdir( dirp )
DIR *dirp;
{
	struct direct *dp;

	if( dirp->dd_loc >= dirp->dd_size )
		return( (struct direct *) NULL );
	dp = (struct direct *) 
		&(dirp->dd_buf[ dirp->dd_loc ]);
	dirp->dd_entno += 1;
	dirp->dd_loc = dirp->dd_entno * sizeof( struct direct ); /* Sys V */

	return( dp );

}

long
telldir( dirp )
DIR *dirp;
{
	return( dirp->dd_loc );
}

void
seekdir( dirp, loc )
DIR *dirp;
long loc;
{
	dirp->dd_loc = loc;
	dirp->dd_entno = loc / (sizeof (struct direct) ); /* Sys V */

	return;
}

/*
 * rewinddir() is implemented as a macro.
 */

void
closedir( dirp )
DIR *dirp;
{
	/* deallocate buffer */
	free( dirp->dd_buf );

	/* deallocate "dirp" */
	free( dirp );

	return;
}

#endif

/*
 * tests if a path name refers to a directory or not.
 *
 */

int
good_dir( name )
char *name;
{
	struct stat sb;

	if( stat( name, &sb ) < 0 )
		return( FALSE ); /* may have to do better than this !! */

	if( access( name, ( 04 | 01 ) ) < 0 )
		return( FALSE ); /* irrelevant if we can't search */

	if( (sb.st_mode & 070000) == 040000 )	/* DIRECTORY */
		return( TRUE );

	return( FALSE );
}

