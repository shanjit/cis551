#include "io.h"
#include "stack.h"

/*
 * notes:
 * pops a name from the directory stack,
 * returning NULL if the bottom of stack condition
 * is reached. The bottom of stack is indicated with a NULL
 * pointer.
 */

char *
pop( stak )
struct stack *stak;
{	
	struct list *l;
	char *p;
	void safe_free();

	l = stak->list;
	if( stak->depth > 0 )
	{
		l = stak->list;
		p = l->l_name;
		stak->list = l->l_next;
		stak->depth -= 1;
		safe_free( l );

#ifdef DEBUG
fprintf( stderr, "stack was: %x,", stak );
fprintf( stderr, "value popped was: %s\n", p );
#endif
	}
	else
		p = (char *) NULL;

	return( p );
}

/*
 * stack_depth()
 * returns the number of objects remaining on the stack.
 */

int
stack_depth( stak )
struct stack *stak;
{

	return( stak->depth );
}

/*
 * notes:
 * pushes a name onto the directory stack.
 * may exit (via emalloc()) if there is not enough memory.
 */

char *
push( name, stak )
char *name;
struct stack *stak;
{
	struct list *l;
	extern char *emalloc(), *strsave();

#ifdef DEBUG
fprintf( stderr, "Stack was: %x, ", stak );
fprintf( stderr, "Pushed: %s\n", name );
#endif
	l = (struct list *) emalloc( sizeof( struct list ) );
	l->l_name = strsave( name );
	l->l_next = stak->list;
	stak->list = l;
	stak->depth += 1;
	return( l->l_name );

}
