/* various global data structures and
 * utility functions used by the "io"
 * command (later to be used in the shell)
 */

#include "io.h"
char 
	*emalloc();
/*
 * panic()
 * prints an error specified in "msg"
 * and exits.
 */

void
panic( msg )
char *msg;
{
	fprintf(stderr, msg );
	exit( 1 );
}

/* strsave()
 * utility routine to save a copy of a string into emalloc()'ed
 * storage. Ripped off from K&R.
 *
 * Note: changed to use emalloc() - saves a lot of error checking.
 * if the string can't be saved, we're probably sunk.
 */

char *
strsave( str )
char *str;
{
	char *p, *emalloc();

	p = emalloc( strlen( str ) + 1 );
	strcpy( p, str );

	return( p );
}


/*
 * emalloc saves the code for checking the return value of malloc()
 * in order to determine whether to exit upon failure.
 * Definitely improves readability.
 */

char *
emalloc( size )
unsigned size;
{
	char *p;
	extern void *malloc();

	p = malloc( (unsigned) size );
	if( p == (char *) NULL )
	{
		fprintf( stderr, "io: No memory, exiting.\n" );
		exit( 1 );
	}

	return( p );
}


/*
 * safe_free()
 * This front end to free()
 * ensures that at least one variant of garbage pointers are not
 * passed to free().
 *
 */

void
safe_free( ptr )
char *ptr;
{

  	if( ptr != (char *) NULL )
		free( ptr );
#ifdef DEBUG
	else
		fprintf( stderr, "%s: attempt to free NULL pointer.\n",
			ProgName );
#endif
	return;
}


/*
 * used by cmd_subst()
 */

char *cargs[] = {
  	"io",
	(char *) NULL
};

/*
 * cmd_subst()
 * performs command substitution on the passed string.
 */

char *
cmd_subst( cmd )
char *cmd;
{
#define MOBY 4096
  	int pfd[2], pid, n;
	char *buf, **export();
	extern char *PathName;

	pipe( pfd );

	pid = fork();

	switch( pid )
	{
	case -1:
		panic( "Fork failed.\n" );
		break;
	case 0:
		dup2( pfd[0], 0 );
		close( pfd[0] );
		dup2( pfd[1], 1 );
		close( pfd[1] );
		execve( PathName, cargs, export( (char **) NULL) );
		panic( "Exec failed.\n" );

	default:
		write( pfd[1], cmd, strlen( cmd ) );
		close( pfd[1] );
		/* is this right? */
		wait( &n );
		buf = (char *) emalloc( MOBY );
		n = read( pfd[0], buf, MOBY );
		buf[n] = EOS;
		close( pfd[0] );
		return( buf );
	}
}

/*
 * utility function to output a list of pointers to characters
 */

void
put_list(list )
char **list;
{
	for( ; *list != (char *) NULL; ++list )
		printf("%s\n", *list );

	return;
}


