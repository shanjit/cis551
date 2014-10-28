/*
 * this file contains
 * routines to implement
 * the "builtin" functions of the 
 * "io" command interpreter.
 */

#include "io.h"
#include "var.h"

int
	b_readonly(),
	b_export(),
  	b_exit(),
	b_cd();

struct b_cmd {
	char *b_name;	
	int b_number;
	int (*b_func)();
};

#define READONLY 0
#define EXPORT 1
#define EXIT 2
#define CD 3

struct b_cmd Builtins[] = {
	{ "readonly", READONLY, b_readonly },
	{ "export", EXPORT, b_export },
	{ "exit", EXIT, b_exit },
	{ "cd", CD, b_cd },
	{ NULL, -1, NULL },
};


int
builtin( name )
char *name;
{
	int i;

	for( i = 0; Builtins[i].b_name != (char *) NULL; i += 1 )
		if( strcmp( name, Builtins[i].b_name ) == 0 )
			return( TRUE );

	return( FALSE );
}

void
do_builtin( list )
char *list[];
{
	int i;

	for( i = 0; Builtins[i].b_name != (char *) NULL; i += 1 )
		if( strcmp( list[0], Builtins[i].b_name ) )
			(Builtins[i].b_func) ( &list[1] );
	
	return;	
}


/*
 * these interface routines which follow assure that
 * calling that is done by Builtins.b_func behaves
 * properly.
 */

int
b_cd( list )
char *list[];
{
	int ret;
	extern char *vval(), *ProgName;

	if( list[0] == (char *) NULL )
		ret = chdir( vval( "HOME" ) );
	else
		ret = chdir( list[0] );

	if( ret < 0 )
		perror( ProgName );

	return( ret );
}

int
b_exit( list )
char *list[];
{
	if( list[0] == (char *) NULL )
		exit( 0 );
	else
		exit( atoi( list[0] ) );
}

int
b_readonly( list )
char *list[];
{
	char **lp, **readonly();

	lp = readonly( list );

	if( lp != (char **) NULL )
	{
		put_list( lp );
	}

	return( 0 );
}

int
b_export( list )
char *list[];
{
	char **lp, **export();

	lp = export( list );

	if( lp != (char **) NULL )
	{
		put_list( lp );
	}

	return( 0 );

}
