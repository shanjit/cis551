
#include "io.h"
#include "stack.h"

extern char *push(), *pop();
extern int stack_depth();


extern void do_dir();
extern char *make_prefix();
extern char *next_dir();
extern char *remaining_path();
extern char *make_name();


struct stack Final;

/* 
 * expand()
 * the interface to expand() has been documented
 * externally on a manual page.
 * (expand.1 shold be kept in this directory)
 */
  
char **
expand( pattern )
char *pattern;
{
	char 
		*strsave(), 
		**list,
		*prefix,
	  	*newpat,
		*postfix,
		*p;
	register int i;
	int sd;
	void sort_list();

/*
 * this stuff has to be thought out carefully
 *
 * in particular, the case with /a/b/*.d/?*.c, etc.
 * What should probably be done is to expand() the first
 * (leftmost) directory, push these results onto a stack,
 * and then pop each one for further expansion.
 *
 * Later thought: recursion seems most natural here.
 */

	if( *pattern == '/' )
		prefix = strsave("/");
	else
		prefix = strsave("");

	newpat = next_dir( pattern );	/* this may not handle /a/ and a/
					 * correctly. 
					 */
	postfix = remaining_path( pattern );

	do_dir( prefix, postfix, newpat );

	safe_free( postfix );
	safe_free( newpat );

	sd = stack_depth( &Final );

	if( sd == 0 )	/* i.e., no expansions */
	{
		list = (char **) emalloc( 2 * sizeof( char *) );
		list[0] = pattern;
		list[1] = (char *) NULL;
#ifdef DEBUG
fprintf( stderr, "expand: stack depth was zero.\n" );
#endif DEBUG
	}
	else
	{
		list = (char **) emalloc( (sd+1) * sizeof( char *) );

		i = 0;
		while( (p = pop( &Final )) != (char *) NULL )
		{
			list[i++] = p;
		}

		sort_list( list, i );

		list[i] = (char *) NULL;	/* set it up as guaranteed */

	}
	return( list );

}

/*
 *	Shell pattern matching function:
 *	returns 1 on a match, 0 otherwise.
 */

int
match( pattern, string )
{
	extern int gmatch();

	if( has_special( pattern ) == TRUE )
		return( gmatch( string, pattern ) );
	else
		return( strcmp( pattern, string ) == 0 );
}

/*
 * has_special
 * function to determine if any of the special pattern matching characters
 * are contained in the passed argument string.
 */

int
has_special( str )
char *str;
{
	while( *str != EOS )
		if( *str == '*' || *str == '?' || *str == '[' )
			return( TRUE );
		else
			++str;

	return( FALSE );
}

