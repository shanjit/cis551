%{
#include "io.h"
int Pid, Status, BackGround;
extern struct command_line *Line, *make_pipe();
extern int Interactive;
#define YYDEBUG 0
%}
  
%union {
	int num;
	char *str;
}

%start lines
%token NEWLINE IN OUT APPEND HERE AMPERSAND DUPIN DUPOUT SEMI_COLON VERTICALBAR
%token <str> WORD
%token <num> NUMBER
%type <str> filename
%type <num> filedes filespec

%%
lines: /* nothing (is this right?) */
       	| lines NEWLINE	
	| command terminator { Pid = cmdline( Line ); }
	| command NEWLINE { Pid = cmdline( Line ); }
	| lines command NEWLINE { Pid = cmdline( Line ); }
	;

command: pipeline
	| simple_command
	;
pipeline: simple_command VERTICALBAR { Line = make_pipe( Line ); } simple_command
	| pipeline VERTICALBAR { Line = make_pipe( Line ); } simple_command
	;
simple_command: WORD	{ do_word( Line, $1); }
	| input_output
	| simple_command WORD { do_word( Line, $2); }
	| simple_command input_output
	;

input_output: filedes APPEND filename
  		{
		  	do_file( $1, IO_APPEND | IO_WRITE | IO_DO_OPEN, 
				$3, Line );
		}
	| APPEND filename  		
		{
			do_file( 1, IO_APPEND | IO_WRITE | IO_DO_OPEN, 
				$2, Line );
		}
	| filedes IN filename
		{
			do_file( $1, IO_READ | IO_DO_OPEN, $3, Line );
		}
	| IN filename
		{
			do_file( 0, IO_READ | IO_DO_OPEN, $2, Line );
		}
	| filedes OUT filename
		{
			do_file( $1, IO_WRITE | IO_DO_OPEN, $3, Line );
		}
	| OUT filename
		{
			do_file( 1, IO_WRITE | IO_DO_OPEN, $2, Line );
		}
	| filedes HERE WORD
		{
			do_file( $1, IO_DO_HERE, $3, Line );
		}
	| HERE WORD
		{	
			do_file( 0, IO_DO_HERE, $2, Line );
		}
	| filedes DUPOUT filespec
		{
			switch( $3 )
			{
			case -1:
				Line->IoCmd[$1].command = IO_DO_CLOSE;
				break;

			default:
				Line->IoCmd[$1].command = IO_DO_DUP | IO_WRITE;
				Line->IoCmd[$1].fd_source = $3;
			}
		}
	| DUPOUT filespec
		{
			switch( $2 )
			{
			case -1:
				Line->IoCmd[1].command = IO_DO_CLOSE;
				break;
			default:
				Line->IoCmd[1].command = IO_DO_DUP | IO_WRITE;
				Line->IoCmd[1].fd_source = $2;
			}
		}
	| filedes DUPIN filespec
		{
			switch( $3 )
			{
			case -1:
				Line->IoCmd[$1].command = IO_DO_CLOSE;
				break;
			default:
				Line->IoCmd[$1].command = IO_DO_DUP | IO_READ;
				Line->IoCmd[$1].fd_source = $3;
			}
		}
	| DUPIN filespec
		{
			switch( $2 )
			{
			case -1:
				Line->IoCmd[0].command = IO_DO_CLOSE;
				break;
			default:
				Line->IoCmd[0].command = IO_DO_DUP | IO_READ;
				Line->IoCmd[0].fd_source = $2;
			}
	
		}
	;
filedes: NUMBER 
		{ 
			if( $$ < MAXFILE )
				$$ = $1;
			else
				yyerror( "Bad file number" );
		}
	;
filespec: filedes	{ $$ = $1; }
	| '-'		{ $$ = -1; }
	;
	
filename: WORD	{ $$ = $1; }
	;

terminator: AMPERSAND	{ BackGround = TRUE; }
	| SEMI_COLON
	;
%%

yyerror( str )
char *str;
{
	fprintf( stderr, "%s: %s near line number %d\n",
		ProgName,
		str,
		LineNumber );
}

