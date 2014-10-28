
/* %W% */

/*
 *  gmatch - match Shell expression patterns
 *
 *   David Korn
 *   AT&T Bell Laboratories
 *   Room 5D-112
 *   Murray Hill, N. J. 07974
 *   Tel. x7975
 *
 *  Derived from Bourne Shell
 */

/*
 * gmatch compares the string s with the shell pattern p and returns 1 if for match,
 * 0 otherwise.
 * The eighth bit is used to 'quote' a character
 */

#define STRIP	0177

gmatch(s, p)
register char *p;
char *s;
{
 register int 	scc,c;
 while(scc = *s++)
	{
	 if(scc)
		{
		 if((scc &= STRIP)==0)
			 scc=0200;
		}
	 switch(c = *p++)
		{
		 case '[':
			{
			 char ok = 0;
			 int lc = -1;
			 int notflag=0;
			 if(*p == '!' )
				{
				 notflag=1;
				 p++;
				}
			 while(c = *p++)
				{
				 if(c==']' && lc>=0)
					 return(ok?gmatch(s,p):0);
				 else if(c=='-' && lc>=0 && *p!=']')
					 /*character range */
					{
					 c = *p++;
					 if(notflag)
						{
						 if(lc>scc || scc>c)
							 ok++;
						 else
							 return(0);
						}
					 else
						 if(lc<scc && scc<=c)
							 ok++;
					}
				 else
					{
					 c &=STRIP;
				 	 if(notflag)
						{
						 if(scc!=c)
							 ok++;
						 else
							 return(0);
						}
					 else
						{
						 if(scc==c)
							 ok++;
						}
					 lc = c;
					}
				}
			 return(0);
			}
		 default:
			 if((c&STRIP) != scc)
				 return(0);
		 case '?':
			 break;
		 case '*':
		/* several asteriks are the same as one */
			 while(*p=='*' )
				 p++;
			 if(*p==0)
				 return(1);
			 --s;
			 c = (*p)&STRIP;
			 while(*s)
				{
				 if(c != ((*s)&STRIP) && *p!='?' && *p!='[')
					 s++;
				 else if(gmatch(s++,p))
					 return(1);
				}
			 return(0);

		 case 0:
			 return(scc==0);
		}
	}
 while(*p == '*')
	 p++;
 return(*p==0);
}


