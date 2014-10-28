
/*
 * var.c
 * - routines used to access and manipulate the variables
 * (parameters) used by the IO command interpreter.
 */

#include "io.h"
#include "var.h"

char 
  	**varlist(),
	*strsave(),
	*sappend(),
	*vval(),
	*getvname(),
	*endvname(),
	*params();

int 
	vmark(),
	vhash(),
	vinsert();

void
  	assign(),
	collect_vars();
	

struct var *vlookup();

	

/*
 * Vtab - global hash table used to find variables.
 */

struct var *Vtab[VHASHSIZE];

/*
 * vhash - simple hashing function for lookup of names.
 */

int
vhash( name )
char *name;
{
	register int val;

	for( val = 0; *name != EOS; ++name )
		val += (*name & 0377);

	return( val & (VHASHSIZE-1) );
}


struct var *
vlookup( name )
char *name;
{
	register struct var *v;

	for( v = Vtab[ vhash(name) ];
		v != (struct var *) NULL;
		v = v->v_next )
	{
		if( strcmp( name, v->v_name ) == 0 )
			break;
	}

	return( v );	/* NULL if no match occurred */
}

/*
 * vmark - 
 * set the type of a variable.
 * returns the current setting of v->v_flag.
 */

int 
vmark( name, flag )
char *name;
int flag;
{
	struct var *v;

	if( (v=vlookup(name)) != (struct var *) NULL )
	{
		v->v_flag |= flag;
		return( v->v_flag );
	}
	else
		return( -1 );	/* failure */
}

/*
 * vinsert
 * insert a new <name, value> pair into the variable database.
 * if name exists, we replace its value.
 * if not, we construct a new structure and append it to
 * the head of the appropriate hash table entry.
 */

int
vinsert( name, value )
char *name, *value;
{
	struct var *v;
	
	if( (v=vlookup(name) ) != (struct var *) NULL )
	{
		if( v->v_flag & V_READONLY )
		{
			return( V_READONLY );
		}

		safe_free( v->v_value );
		v->v_value = strsave( value );
		return( v->v_flag );
	}
	else
	{
		int index;

		v = (struct var *) emalloc( sizeof( struct var ));

		v->v_name = strsave( name );
		v->v_value = strsave( value );
		v->v_flag = V_LOCAL;
 		index = vhash( name );
		v->v_next = Vtab[ index ];
		Vtab[index] = v;
		return( V_LOCAL );
	}
}

/*
 * loadenv -
 * called by main() to load the environment variables into
 * the hash-pointered data structure.
 */

void
loadenv( envp )
char **envp;
{

	while( *envp != (char *) NULL )
	{	
	  	assign( *envp, V_EXPORTED );
		++envp;
	}

	return;
}

void
assign( assignment, flag )
char *assignment;
int flag;
{
	char *tmp, *val, *strsave();

	tmp = strsave( assignment );
	val = find_equals( tmp );
	*val = EOS;
	++val;
	vinsert( tmp, val );
	vmark( tmp, flag );

	return;
}

/*
 * params() - 
 * expands a passed word that the parser has picked up by performing
 * any parameter substitution it can. It is not recursive - it only 
 * passes once through word.
 */

char *
params( word )
char *word;
{
	char *name, 
		*next_var, 
		*val,
		*ret_val, 
		*p;

	ret_val = strsave( "" );

	while( (next_var=find_dollar(word)) != (char *) NULL )
	{
		name = getvname( next_var );
		word = endvname( next_var );
		val = vval( name );
		if( val != (char *) NULL )
		{
			p = sappend( ret_val, val );
			safe_free( ret_val );
			ret_val = p;
		}
	}
	
	p = sappend( ret_val, word );
	safe_free( ret_val );
	
	return( p );
}


/*
 * sappend()
 * returns a pointer to new storage containing
 * s2 appended to s1. Does makecmd() do this ?
 */

char *
sappend( s1, s2 )
char *s1, *s2;
{
	char *p;

	p = (char *) emalloc( (unsigned) strlen(s1) + strlen(s2) + 1 );
	strcpy( p, s1 );
	strcat( p, s2 );

	return( p );
}

/*
 * vval()
 * returns the character string value of a <name, value> pair.
 */

char *
vval( name )
char *name;
{
	struct var *v;

	v = vlookup( name );
	if( v != (struct var *) NULL )
		return( v->v_value );
	else
		return( (char *) NULL );
}

/*
 * getvname -
 * extracts a variable name from a string beginning
 * with a DOLLAR SIGN.
 */

char *
getvname( str )
char *str;
{
	int len;
	char *p, *q, *r;

	++str;
	if( *str == '{' )
		++str;
	
	for( p = str, len = 0;
		valid( (int) *p, len ) == TRUE;
		len += 1 )
	{
		++p;
	}

	p = str;

	r = q = (char *) emalloc( (unsigned) (len+1) );

	while( len-- > 0 )
		*q++ = *p++;

	*q = EOS;

	return( r );
}
 
/*
 * endvname - 
 * finds the end of a variable name in a string
 * beginning with a DOLLAR SIGN.
 */

char *
endvname( str )
char *str;
{
	int len, curly_flag;

	++str;

	if( *str == '{' )
	{
		curly_flag = TRUE;
		++str;
	}
	else
		curly_flag = FALSE;
	
	for( len = 0;
		valid( (int ) *str, len ) == TRUE;
		len += 1 )
	{	
		++str;
	}

	if( curly_flag && *str == '}' )
		++str;

	return( str );
}


/*
 * valid() - 
 * used to test whether parts of a vriable name are valid.
 * position matters.
 */

#include <ctype.h>

int
valid( c, pos )
int c, pos;
{
	if( isascii( c ) )
	{
		if( pos == 0 && (isalpha( c ) || c == '_' ) )
			return( TRUE );
		else if( isalnum( c ) || c == '_' )
			return( TRUE );
	}

	return( FALSE );
}


/*
 * readonly()
 */


char **
readonly( list )
char **list;
{
	return( varlist( list, V_READONLY ) );
}

/*
 * export - marks variables
 * in a fashion similar to readonly above.
 * (they use the same code!)
 */

char **
export( list )
char **list;
{
	return( varlist( list, V_EXPORTED ) );
}

/*
 * varlist()
 *	-work routine for readonly() and export().
 *	-this is a tricky function, and I'm not sure quite
 * what the right way to write it is. Nevertheless, It deals
 * with NULL terminated lists of character pointers, which it
 * both takes as arguments and returns. If it gets no arguments,
 * it returns a list of variables (as  <name=value> pairs)
 * from the global hash table structure, in lexicographic order (sorted)
 * which are marked by "flag". If it gets any arguments, it returns
 * (char **) NULL.
 */

char **
varlist( list, flag )
char **list;
int flag;
{
	int count;
	char **ret_val;

	if( list != (char **) NULL )
	{
		while( *list != (char *) NULL )
		{
			vmark( *list++, flag );	
		}
		return( (char **) NULL );
	}
	else	/* the work !! */
	{
		count = count_vars( flag );
		ret_val = (char **)
			emalloc( (sizeof(char *) ) * (count + 1 ) );
		collect_vars( ret_val, flag ); 
		sort_list( ret_val, count );
		return( ret_val );
	}

}

void
collect_vars( ptr, flag )
char **ptr;
int flag;
{
  	register struct var *v;
	int i;
	
	for( i = 0; i < VHASHSIZE; i += 1 )
	{
		for( v = Vtab[i]; v != (struct var *) NULL; v = v->v_next )
		{
			if( ( v->v_flag & flag ) == flag )
			{
				*ptr = (char *) emalloc( strlen( v->v_name ) +
						strlen( v->v_value ) + 2 );
				strcpy( *ptr, v->v_name );
				strcat( *ptr, "=" );
				strcat( *ptr, v->v_value );
				++ptr;
			}
		}
	}

	*ptr = (char *) NULL;
}

int
count_vars( type )
int type;
{
	register struct var *v;
	int i, count;

	for( count = 0, i = 0; i < VHASHSIZE; i += 1 )
	{
		for( v = Vtab[i]; v != (struct var *) NULL; v = v->v_next )
			if( (v->v_flag & type) == type )
				count += 1;
	}

	return( count );

}



	
