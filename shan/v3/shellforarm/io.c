
#include "io.h"
#include <sys/signal.h>

/* global variables go here  */

char *DefaultPS1="$ ";
char *DefaultPS2="> ";

char *PathName = "/usr/jms/private.d/io.d/io";
char *ProgName;
int LineNumber;
int Interactive;
int ErrFlag;
struct command_line *Line, *line_alloc();

/*
 * main program -
 * essentially just picks up arguments and kicks off yyparse().
 */

main( argc, argv, env )
int argc;
char *argv[], *env[];
{

	ProgName = argv[0];
	LineNumber = 0;
	
	loadenv( env );
	
	Line = line_alloc();

	if( isatty(0) )
	{
		printf( "%s", prompt1() );	
		Interactive = TRUE;
	}
	else
		Interactive = FALSE;

	signal( SIGCLD, SIG_IGN );

	yyparse();
	
	exit( 0 );
	
}

/*
 * produces the character string used as the primary prompt.
 */
char *vval();

char *
prompt1()
{
	static char *p = (char *) NULL;
	
	if( p != (char *) NULL )
		return( p );
		
	p = vval( "PS1" );
	if( p != (char *) NULL )
		return( p );
		
	p = DefaultPS1;
	
	return( p );
}

/*
 * produces the character string used as the secondary
 * prompt.
 */

char *
prompt2()
{
	char *getenv();
	static char *p = (char *) NULL;
	
	if( p != (char *) NULL )
		return( p );
		
	p = vval( "PS2" );
	if( p != (char *) NULL )
		return( p );
		
	p = DefaultPS2;
	
	return( p );
	
}


