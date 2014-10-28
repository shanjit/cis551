%{
#include "io.h"
#include "y.tab.h"

/* this stuff is all for quoted strings.	*/
#define QSIZE 4096	/* big */
char Qbuf[QSIZE];
#define inquotes() ((InDouble==TRUE) || (InSingle==TRUE) || (InBack==TRUE))
int InDouble = FALSE;
char *DoublePtr = (char *) NULL;
int InSingle = FALSE;
int InBack = FALSE;
char *BackPtr = (char *) NULL;
char 
	*cmd_subst(), 
	*params(),
	*strsave(),
	*Tmp;

/* end quoted string stuff */

%}
%%
["]	{
	
	if( inquotes() )
	{
		if( InDouble==TRUE )
		{
			InDouble = FALSE;
			Tmp = params( DoublePtr );
			strcat( Qbuf, Tmp );
			safe_free( Tmp );
			DoublePtr = (char *) NULL;
		}
		if( inquotes() )
			strcat( Qbuf, yytext );
	}
	else
		InDouble = TRUE;
}		
[']	{
	
	if( inquotes() )
	{
		if( InSingle==TRUE )
			InSingle = FALSE;
		if( inquotes() )
			strcat( Qbuf, yytext );
	}
	else
		InSingle = TRUE;
}		
[`]	{
	/* still have to deal with backquotes inside
	 * double quotes.....
	 */
	if( inquotes() )
	{
		if( InBack==TRUE )
		{
			InBack = FALSE;	
			Tmp = cmd_subst( BackPtr );
			strcat( Qbuf, Tmp );
			safe_free( Tmp );
			BackPtr = (char *) NULL;
		}
		if( inquotes() )
			strcat( Qbuf, yytext );
	}
	else
	{
		InBack = TRUE;
		BackPtr = &Qbuf[ strlen(Qbuf) ];
	}
}		
[ \t]+	{ 
	if( inquotes() )
		strcat( Qbuf, yytext);
	else
	{
		if( Qbuf[0] != EOS )
		{	
			yylval.str = strsave( Qbuf );
			Qbuf[0] = EOS;
			return( WORD );
		}
	}
}
">>" { 
	if( !inquotes() )
	{
		if( Qbuf[0] != EOS )
		{	
			yylval.str = strsave( Qbuf );
			Qbuf[0] = EOS;
			unput( '>' ); unput( '>' );
			return( WORD );
		}
		else
			return( APPEND );
	}
	else
		strcat( Qbuf, yytext );
}
"<<" { 
	if( !inquotes() )
	{
		if( Qbuf[0] != EOS )
		{	
			yylval.str = strsave( Qbuf );
			Qbuf[0] = EOS;
			unput( '<' ); unput( '<' );
			return( WORD );
		}
		else
			return( HERE );
	}
	else
		strcat( Qbuf, yytext );
}
"<" { 
	if( !inquotes() )
	{
		if( Qbuf[0] != EOS )
		{	
			yylval.str = strsave( Qbuf );
			Qbuf[0] = EOS;
			unput( '<' );
			return( WORD );
		}
		else
			return( IN );
	}
	else
		strcat( Qbuf, yytext );
}
"<&" { 
	if( !inquotes() )
	{
		if( Qbuf[0] != EOS )
		{	
			yylval.str = strsave( Qbuf );
			Qbuf[0] = EOS;
			unput( '&' ); unput( '<' );
			return( WORD );
		}
		else
			return( DUPIN );
	}
	else
		strcat( Qbuf, yytext );
}
">" { 	if( !inquotes() )
	{
		if( Qbuf[0] != EOS )
		{	
			yylval.str = strsave( Qbuf );
			Qbuf[0] = EOS;
			unput( '>' );
			return( WORD );
		}
		else
			return( OUT );
	}
	else
		strcat( Qbuf, yytext );
}
">&" { 	
	if( !inquotes() )
	{
		if( Qbuf[0] != EOS )
		{	
			yylval.str = strsave( Qbuf );
			Qbuf[0] = EOS;
			unput( '&' ); unput( '>' );
			return( WORD );
		}
		else
			return( DUPOUT );
	}
	else
		strcat( Qbuf, yytext );
}
"&"	{ 	
	if( !inquotes() )
	{
		if( Qbuf[0] != EOS )
		{	
			yylval.str = strsave( Qbuf );
			Qbuf[0] = EOS;
			unput( '&' );
			return( WORD );
		}
		else
			return( AMPERSAND );
	}

	else
		strcat( Qbuf, yytext );
}
"|"	{ 	
	if( !inquotes() )
	{
		if( Qbuf[0] != EOS )
		{	
			yylval.str = strsave( Qbuf );
			Qbuf[0] = EOS;
			unput( '|' );
			return( WORD );
		}
		else
			return( VERTICALBAR );
	}
	else
		strcat( Qbuf, yytext );
}
[0-9]+ { 
	if( !inquotes() )
	{
		yylval.num = atoi( yytext );
		return( NUMBER ); 
	}
	else
		strcat( Qbuf, yytext );
}
[^><& \t\n"'`|]* {
	if( !inquotes() )
	{	
		Tmp = strsave( yytext );
		yylval.str = params( Tmp );
		safe_free( Tmp );
		return( WORD );
	}
	else
		strcat( Qbuf, yytext );
}
\n { 
	LineNumber += 1;
	if( !inquotes() )
	{
		if( Qbuf[0] != EOS )
		{
			yylval.str = strsave( Qbuf );
			Qbuf[0] = EOS;
			unput( '\n' );
			return( WORD );
		}
		else
		{
			if( Interactive == TRUE )
			{
				printf( "%s", prompt1() );
			}
			return( NEWLINE ); 
		}
	}
	else
	{
		if( Interactive == TRUE )
		{
			printf( "%s", prompt2() );
		}
		strcat( Qbuf, yytext );
	}
}
. { 
	if( !inquotes() )
		return( yytext[0] ); 
	else
		strcat( Qbuf, yytext );
}

%%
#ifndef yywrap
int yywrap(void) { return 1; }
#endif
