/* 
 * iocmd.c:
 * contains commands which set up I/O redirection from buffer
 * created during parse phase.
 */

#include "io.h"
#include <sys/file.h>
char 
	*emalloc(),
	*delete_arg();

void
do_io( cmd )
struct io_cmd *cmd;
{
	int fd;
	char *create_here();

	for( fd = 0; fd < MAXFILE; fd += 1 )
	{
		if( cmd[fd].command == IO_DO_HERE )
		{
			cmd[fd].file_name = 
				create_here( cmd[fd].file_name );
			cmd[fd].command = IO_DO_OPEN | IO_READ;
		}
		if( cmd[fd].command != IO_NOTHING )
			set_io( fd, &cmd[fd] );
	}
	return;
}

void
set_io( fd, cmd )
int fd;
struct io_cmd *cmd;
{
	int t_fd;	/* temp fd */

	/* we don't do any major error checking here,
	 * as the accessibility of these files has been
	 * verified already, by do_file(), during parsing.
 	 * Anything that's made it here can be opened,
	 * and exists. 
	 * I'm not sure how the Bourne shell behaves with respect
	 * to these things.
	 */

	switch( cmd->command )
	{

	case IO_READ | IO_DO_OPEN:
		t_fd = open( cmd->file_name, O_RDONLY );
		if( t_fd != fd )
		{
			dup2( t_fd, fd );
			close( t_fd );
		}
		break;

	case IO_WRITE | IO_DO_OPEN:
		t_fd = open( cmd->file_name, O_WRONLY, 0666 );
		if( t_fd != fd )
		{
			dup2( t_fd, fd );
			close( t_fd );
		}
		break;

	case IO_APPEND | IO_WRITE | IO_DO_OPEN:
		t_fd = open( cmd->file_name, O_WRONLY | O_APPEND, 0666 );
		if( t_fd != fd )
		{
			dup2( t_fd, fd );
			close( t_fd );
		}
		break;

	case IO_DO_DUP:
		dup2( cmd->fd_source, fd );
		break;

	case IO_NOTHING:
        default:
		break;

	}

	return;
}

/*
 * dup2:
 * library call to emulate 4BSD dup2();
 * idea is that file descriptor old is dup()'d
 * into file descriptor new.
 */

#ifndef BSD
int
dup2( old, new )
int old, new;
{
	char close_map[MAXFILE];
	register int i;
	int ret_val;
	
	if( new < 0 || new >= MAXFILE )
	  	return( -1 );

	close( new );
	for( i=0; i < new; i += 1 )	/* "new" has been screened */
		close_map[i] = 0;

	ret_val = new;
		
	while( (i = dup(old)) < new)		/* is this robust? */
	{
		if( i == -1 )	/* dup() failed */
		{
			ret_val = -1;
			break;			
		}
		close_map[i] = 1;
	}

	for(i=0; i < new; i += 1 )
		if( close_map[i] == 1 )
			close( i );
		
	return( ret_val );
	
}
#endif

/*
 * clear_io:
 * used to initialize the global data structure storing
 * information on I/O redirection actions to take.
 */

void
clear_io( line )
struct command_line *line;
{
	register int i;

	for( i=0; i< MAXFILE; i += 1 )
	{
		line->IoCmd[i].command = IO_NOTHING;
		if( line->IoCmd[i].file_name != (char *) NULL )
		{
			free( line->IoCmd[i].file_name );
			line->IoCmd[i].file_name = (char *) NULL;
		}
		line->IoCmd[i].fd_source = 0;	/* initial value; not distinguished */
	}
	
	return;
}

#define TMP_NAME "/usr/tmp/ioXXXXXX"

/*
 * returns a temporary file name.
 * the storage should be free()'d 
 * when unneeded; NULL if no memory
 * note: uses mktemp(); this is mktmp() on System III,V.
 */

char *
tmpname()
{
	char *p, *emalloc();

	p = emalloc( strlen( TMP_NAME ) + 1 );
	strcpy( p, TMP_NAME );
	mktemp( p );

	return( p );
}

/*
 * reads lines for HERE documents:
 * removes tabs if "-" preceded WORD.
 * WORD is passed in "end"; the results
 * of the reading are put in to the file
 * pointed to by "fp". If "end" contains any
 * 'quoted' characters, command and parameter
 * substitution is not performed on the input,
 * otherwise it is.
 */

void	/* should I have this return something? */
read_lines( fp, end, tab_flag )
FILE *fp;
char *end;
int tab_flag;
{
	char line_buf[MAXLINE], *p;
	int end_len, special;
	extern int Interactive;

	/* eighth bit is used for quoting of an individual character;
	 * we also get the string's length as a by-product.
 	 */
	for( end_len = 0, special = FALSE; 
		end[end_len] != EOS; 
		end_len += 1 )
	{
		if( end[end_len] & 0177 )
			special = TRUE;
	}

	if( Interactive == TRUE )
		printf( "%s", prompt2() );
	for( ; fgets( line_buf, MAXLINE, stdin ) != NULL; )
	{
		if( tab_flag == TRUE )	/* god, this is gross! */
		{
			while( line_buf[0] == '\t' )
			{
				strcpy( line_buf, &line_buf[1] );
			}
		}


		if( strncmp( end, line_buf, end_len ) == 0 &&
			line_buf[end_len] == '\n' &&
			line_buf[end_len+1] == EOS )
			return;

		fputs( line_buf, fp );

		if( Interactive == TRUE )
			printf( "%s", prompt2());
	}
}


/*
 * create_here()
 * used to create the temporary files used for creating
 * HERE documents. There are several questions remaining:
 *
 *	1.) How do the temporary files get removed?
 *	2.) Should we return a file descriptor or a filename?
 * In any case, it currently returns a file name, and there is
 * no special mechanism to do deletion (yet).
 */

char *
create_here( looked_for )
char *looked_for;
{
	int rm_tabs;
	char *tf, *tmpname();
	FILE *fp;

	if( looked_for[0] == '-' )
	{
		++looked_for;
		rm_tabs = TRUE;
	}

	tf = tmpname();
	fp = fopen( tf, "w" );

	read_lines( fp, looked_for, rm_tabs );

	fclose( fp );

	return( tf );

}


/*
 * do_file
 * does a trial open of a file associated with file descriptor
 * "fd", and named by parameter "name". If the file cannot be opened,
 * the global ErrFlag is set to indicate that an error has occurred.
 * the *action* routines cooperate to ensure that ErrFlag is respected
 * by not doing anything once it is set, so that the input will be 
 * digested up until the logical line is ended.
 *
 * This may not be the right way to go about things.
 *
 * "line" is the command line we are working on, and
 * "command" is the set of commands (see iocmd.h) we are applying to
 * it.
 */

int
do_file( fd, command, name, line )
int fd, command;
char *name;
struct command_line *line;
{
	int tempfd, flag;
	extern int ErrFlag;
	extern char *ProgName;

	if( ErrFlag == TRUE )
		return( -1 );

	if( (command & IO_DO_HERE) == IO_DO_HERE )
	{
		line->IoCmd[fd].command = command;
		return( fd );
	}

	if( command & IO_WRITE )
		flag = (O_WRONLY | O_CREAT);
	else
		flag = O_RDONLY;

	tempfd = open( name, flag, 0666 );
	if( tempfd < 0 )
	{
		perror( ProgName );
		ErrFlag = TRUE;
		return( tempfd );
	}
	close( tempfd );

	line->IoCmd[fd].command = command;
	line->IoCmd[fd].file_name = name;

	return( fd );

}
