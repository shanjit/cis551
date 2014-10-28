
/*  A Bison parser, made from grammar.y
 by  GNU Bison version 1.25
  */

#define YYBISON 1  /* Identify Bison output.  */

#define	NEWLINE	258
#define	IN	259
#define	OUT	260
#define	APPEND	261
#define	HERE	262
#define	AMPERSAND	263
#define	DUPIN	264
#define	DUPOUT	265
#define	SEMI_COLON	266
#define	VERTICALBAR	267
#define	WORD	268
#define	NUMBER	269

#line 1 "grammar.y"

#include "io.h"
int Pid, Status, BackGround;
extern struct command_line *Line, *make_pipe();
extern int Interactive;
#define YYDEBUG 0

#line 9 "grammar.y"
typedef union {
	int num;
	char *str;
} YYSTYPE;
#include <stdio.h>

#ifndef __cplusplus
#ifndef __STDC__
#define const
#endif
#endif



#define	YYFINAL		52
#define	YYFLAG		-32768
#define	YYNTBASE	16

#define YYTRANSLATE(x) ((unsigned)(x) <= 269 ? yytranslate[x] : 27)

static const char yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,    15,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     1,     2,     3,     4,     5,
     6,     7,     8,     9,    10,    11,    12,    13,    14
};

#if YYDEBUG != 0
static const short yyprhs[] = {     0,
     0,     1,     4,     7,    10,    14,    16,    18,    19,    24,
    25,    30,    32,    34,    37,    40,    44,    47,    51,    54,
    58,    61,    65,    68,    72,    75,    79,    82,    84,    86,
    88,    90,    92
};

static const short yyrhs[] = {    -1,
    16,     3,     0,    17,    26,     0,    17,     3,     0,    16,
    17,     3,     0,    18,     0,    21,     0,     0,    21,    12,
    19,    21,     0,     0,    18,    12,    20,    21,     0,    13,
     0,    22,     0,    21,    13,     0,    21,    22,     0,    23,
     6,    25,     0,     6,    25,     0,    23,     4,    25,     0,
     4,    25,     0,    23,     5,    25,     0,     5,    25,     0,
    23,     7,    13,     0,     7,    13,     0,    23,    10,    24,
     0,    10,    24,     0,    23,     9,    24,     0,     9,    24,
     0,    14,     0,    23,     0,    15,     0,    13,     0,     8,
     0,    11,     0
};

#endif

#if YYDEBUG != 0
static const short yyrline[] = { 0,
    22,    23,    24,    25,    26,    29,    30,    32,    33,    33,
    34,    35,    36,    37,    38,    41,    46,    51,    55,    59,
    63,    67,    71,    75,    88,   100,   112,   126,   134,   135,
   138,   141,   142
};
#endif


#if YYDEBUG != 0 || defined (YYERROR_VERBOSE)

static const char * const yytname[] = {   "$","error","$undefined.","NEWLINE",
"IN","OUT","APPEND","HERE","AMPERSAND","DUPIN","DUPOUT","SEMI_COLON","VERTICALBAR",
"WORD","NUMBER","'-'","lines","command","pipeline","@1","@2","simple_command",
"input_output","filedes","filespec","filename","terminator", NULL
};
#endif

static const short yyr1[] = {     0,
    16,    16,    16,    16,    16,    17,    17,    19,    18,    20,
    18,    21,    21,    21,    21,    22,    22,    22,    22,    22,
    22,    22,    22,    22,    22,    22,    22,    23,    24,    24,
    25,    26,    26
};

static const short yyr2[] = {     0,
     0,     2,     2,     2,     3,     1,     1,     0,     4,     0,
     4,     1,     1,     2,     2,     3,     2,     3,     2,     3,
     2,     3,     2,     3,     2,     3,     2,     1,     1,     1,
     1,     1,     1
};

static const short yydefact[] = {     1,
     0,     0,     0,     0,     0,     0,    12,    28,     0,     0,
     6,     7,    13,     0,    31,    19,    21,    17,    23,    30,
    29,    27,    25,     2,     0,     4,    32,    33,     3,    10,
     8,    14,    15,     0,     0,     0,     0,     0,     0,     5,
     0,     0,    18,    20,    16,    22,    26,    24,    11,     9,
     0,     0
};

static const short yydefgoto[] = {     9,
    10,    11,    42,    41,    12,    13,    14,    22,    16,    29
};

static const short yypact[] = {    50,
    -8,    -8,    -8,    -3,   -11,   -11,-32768,-32768,     8,    16,
     4,    39,-32768,    72,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,    17,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,    -8,    -8,    -8,    10,   -11,   -11,-32768,
    50,    50,-32768,-32768,-32768,-32768,-32768,-32768,    61,    61,
    28,-32768
};

static const short yypgoto[] = {-32768,
    20,-32768,-32768,-32768,   -16,   -12,     1,     3,    -1,-32768
};


#define	YYLAST		82


static const short yytable[] = {    33,
    17,    18,     8,    20,    15,    21,    21,    51,    23,    19,
    24,     1,     2,     3,     4,    30,     5,     6,    26,    40,
     7,     8,    46,    27,    49,    50,    28,    52,    25,     0,
     0,     0,    43,    44,    45,     0,    33,    33,    21,    21,
    47,    48,     1,     2,     3,     4,     0,     5,     6,     0,
    31,    32,     8,     1,     2,     3,     4,     0,     5,     6,
     0,     0,     7,     8,     1,     2,     3,     4,     0,     5,
     6,     0,     0,    32,     8,    34,    35,    36,    37,     0,
    38,    39
};

static const short yycheck[] = {    12,
     2,     3,    14,    15,    13,     5,     6,     0,     6,    13,
     3,     4,     5,     6,     7,    12,     9,    10,     3,     3,
    13,    14,    13,     8,    41,    42,    11,     0,     9,    -1,
    -1,    -1,    34,    35,    36,    -1,    49,    50,    38,    39,
    38,    39,     4,     5,     6,     7,    -1,     9,    10,    -1,
    12,    13,    14,     4,     5,     6,     7,    -1,     9,    10,
    -1,    -1,    13,    14,     4,     5,     6,     7,    -1,     9,
    10,    -1,    -1,    13,    14,     4,     5,     6,     7,    -1,
     9,    10
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
#line 3 "/pkg/osrev/5.7/gcc-2.7.2/share/bison.simple"

/* Skeleton output parser for bison,
   Copyright (C) 1984, 1989, 1990 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

#ifndef alloca
#ifdef __GNUC__
#define alloca __builtin_alloca
#else /* not GNU C.  */
#if (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc) || defined (__sgi)
#include <alloca.h>
#else /* not sparc */
#if defined (MSDOS) && !defined (__TURBOC__)
#include <malloc.h>
#else /* not MSDOS, or __TURBOC__ */
#if defined(_AIX)
#include <malloc.h>
 #pragma alloca
#else /* not MSDOS, __TURBOC__, or _AIX */
#ifdef __hpux
#ifdef __cplusplus
extern "C" {
void *alloca (unsigned int);
};
#else /* not __cplusplus */
void *alloca ();
#endif /* not __cplusplus */
#endif /* __hpux */
#endif /* not _AIX */
#endif /* not MSDOS, or __TURBOC__ */
#endif /* not sparc.  */
#endif /* not GNU C.  */
#endif /* alloca not defined.  */

/* This is the parser code that is written into each bison parser
  when the %semantic_parser declaration is not specified in the grammar.
  It was written by Richard Stallman by simplifying the hairy parser
  used when %semantic_parser is specified.  */

/* Note: there must be only one dollar sign in this file.
   It is replaced by the list of actions, each action
   as one case of the switch.  */

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	return(0)
#define YYABORT 	return(1)
#define YYERROR		goto yyerrlab1
/* Like YYERROR except do call yyerror.
   This remains here temporarily to ease the
   transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto yyerrlab
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(token, value) \
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    { yychar = (token), yylval = (value);			\
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { yyerror ("syntax error: cannot back up"); YYERROR; }	\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

#ifndef YYPURE
#define YYLEX		yylex()
#endif

#ifdef YYPURE
#ifdef YYLSP_NEEDED
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, &yylloc, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval, &yylloc)
#endif
#else /* not YYLSP_NEEDED */
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval)
#endif
#endif /* not YYLSP_NEEDED */
#endif

/* If nonreentrant, generate the variables here */

#ifndef YYPURE

int	yychar;			/*  the lookahead symbol		*/
YYSTYPE	yylval;			/*  the semantic value of the		*/
				/*  lookahead symbol			*/

#ifdef YYLSP_NEEDED
YYLTYPE yylloc;			/*  location data for the lookahead	*/
				/*  symbol				*/
#endif

int yynerrs;			/*  number of parse errors so far       */
#endif  /* not YYPURE */

#if YYDEBUG != 0
int yydebug;			/*  nonzero means print parse trace	*/
/* Since this is uninitialized, it does not stop multiple parsers
   from coexisting.  */
#endif

/*  YYINITDEPTH indicates the initial size of the parser's stacks	*/

#ifndef	YYINITDEPTH
#define YYINITDEPTH 200
#endif

/*  YYMAXDEPTH is the maximum size the stacks can grow to
    (effective only if the built-in stack extension method is used).  */

#if YYMAXDEPTH == 0
#undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
int yyparse (void);
#endif

#if __GNUC__ > 1		/* GNU C and GNU C++ define this.  */
#define __yy_memcpy(TO,FROM,COUNT)	__builtin_memcpy(TO,FROM,COUNT)
#else				/* not GNU C or C++ */
#ifndef __cplusplus

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (to, from, count)
     char *to;
     char *from;
     int count;
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#else /* __cplusplus */

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (char *to, char *from, int count)
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#endif
#endif

#line 196 "/pkg/osrev/5.7/gcc-2.7.2/share/bison.simple"

/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
#ifdef __cplusplus
#define YYPARSE_PARAM_ARG void *YYPARSE_PARAM
#define YYPARSE_PARAM_DECL
#else /* not __cplusplus */
#define YYPARSE_PARAM_ARG YYPARSE_PARAM
#define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
#endif /* not __cplusplus */
#else /* not YYPARSE_PARAM */
#define YYPARSE_PARAM_ARG
#define YYPARSE_PARAM_DECL
#endif /* not YYPARSE_PARAM */

int
yyparse(YYPARSE_PARAM_ARG)
     YYPARSE_PARAM_DECL
{
  register int yystate;
  register int yyn;
  register short *yyssp;
  register YYSTYPE *yyvsp;
  int yyerrstatus;	/*  number of tokens to shift before error messages enabled */
  int yychar1 = 0;		/*  lookahead token as an internal (translated) token number */

  short	yyssa[YYINITDEPTH];	/*  the state stack			*/
  YYSTYPE yyvsa[YYINITDEPTH];	/*  the semantic value stack		*/

  short *yyss = yyssa;		/*  refer to the stacks thru separate pointers */
  YYSTYPE *yyvs = yyvsa;	/*  to allow yyoverflow to reallocate them elsewhere */

#ifdef YYLSP_NEEDED
  YYLTYPE yylsa[YYINITDEPTH];	/*  the location stack			*/
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;

#define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
#define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  int yystacksize = YYINITDEPTH;

#ifdef YYPURE
  int yychar;
  YYSTYPE yylval;
  int yynerrs;
#ifdef YYLSP_NEEDED
  YYLTYPE yylloc;
#endif
#endif

  YYSTYPE yyval;		/*  the variable used to return		*/
				/*  semantic values from the action	*/
				/*  routines				*/

  int yylen;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Starting parse\n");
#endif

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss - 1;
  yyvsp = yyvs;
#ifdef YYLSP_NEEDED
  yylsp = yyls;
#endif

/* Push a new state, which is found in  yystate  .  */
/* In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
yynewstate:

  *++yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Give user a chance to reallocate the stack */
      /* Use copies of these so that the &'s don't force the real ones into memory. */
      YYSTYPE *yyvs1 = yyvs;
      short *yyss1 = yyss;
#ifdef YYLSP_NEEDED
      YYLTYPE *yyls1 = yyls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      int size = yyssp - yyss + 1;

#ifdef yyoverflow
      /* Each stack pointer address is followed by the size of
	 the data in use in that stack, in bytes.  */
#ifdef YYLSP_NEEDED
      /* This used to be a conditional around just the two extra args,
	 but that might be undefined if yyoverflow is a macro.  */
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yyls1, size * sizeof (*yylsp),
		 &yystacksize);
#else
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yystacksize);
#endif

      yyss = yyss1; yyvs = yyvs1;
#ifdef YYLSP_NEEDED
      yyls = yyls1;
#endif
#else /* no yyoverflow */
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	{
	  yyerror("parser stack overflow");
	  return 2;
	}
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;
      yyss = (short *) alloca (yystacksize * sizeof (*yyssp));
      __yy_memcpy ((char *)yyss, (char *)yyss1, size * sizeof (*yyssp));
      yyvs = (YYSTYPE *) alloca (yystacksize * sizeof (*yyvsp));
      __yy_memcpy ((char *)yyvs, (char *)yyvs1, size * sizeof (*yyvsp));
#ifdef YYLSP_NEEDED
      yyls = (YYLTYPE *) alloca (yystacksize * sizeof (*yylsp));
      __yy_memcpy ((char *)yyls, (char *)yyls1, size * sizeof (*yylsp));
#endif
#endif /* no yyoverflow */

      yyssp = yyss + size - 1;
      yyvsp = yyvs + size - 1;
#ifdef YYLSP_NEEDED
      yylsp = yyls + size - 1;
#endif

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Stack size increased to %d\n", yystacksize);
#endif

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Entering state %d\n", yystate);
#endif

  goto yybackup;
 yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Reading a token: ");
#endif
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Now at end of input.\n");
#endif
    }
  else
    {
      yychar1 = YYTRANSLATE(yychar);

#if YYDEBUG != 0
      if (yydebug)
	{
	  fprintf (stderr, "Next token is %d (%s", yychar, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise meaning
	     of a token, for further debugging info.  */
#ifdef YYPRINT
	  YYPRINT (stderr, yychar, yylval);
#endif
	  fprintf (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting token %d (%s), ", yychar, yytname[yychar1]);
#endif

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  /* count tokens shifted since error; after three, turn off error status.  */
  if (yyerrstatus) yyerrstatus--;

  yystate = yyn;
  goto yynewstate;

/* Do the default action for the current state.  */
yydefault:

  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;

/* Do a reduction.  yyn is the number of a rule to reduce with.  */
yyreduce:
  yylen = yyr2[yyn];
  if (yylen > 0)
    yyval = yyvsp[1-yylen]; /* implement default value of the action */

#if YYDEBUG != 0
  if (yydebug)
    {
      int i;

      fprintf (stderr, "Reducing via rule %d (line %d), ",
	       yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (i = yyprhs[yyn]; yyrhs[i] > 0; i++)
	fprintf (stderr, "%s ", yytname[yyrhs[i]]);
      fprintf (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif


  switch (yyn) {

case 3:
#line 24 "grammar.y"
{ Pid = cmdline( Line ); ;
    break;}
case 4:
#line 25 "grammar.y"
{ Pid = cmdline( Line ); ;
    break;}
case 5:
#line 26 "grammar.y"
{ Pid = cmdline( Line ); ;
    break;}
case 8:
#line 32 "grammar.y"
{ Line = make_pipe( Line ); ;
    break;}
case 10:
#line 33 "grammar.y"
{ Line = make_pipe( Line ); ;
    break;}
case 12:
#line 35 "grammar.y"
{ do_word( Line, yyvsp[0].str); ;
    break;}
case 14:
#line 37 "grammar.y"
{ do_word( Line, yyvsp[0].str); ;
    break;}
case 16:
#line 42 "grammar.y"
{
		  	do_file( yyvsp[-2].num, IO_APPEND | IO_WRITE | IO_DO_OPEN, 
				yyvsp[0].str, Line );
		;
    break;}
case 17:
#line 47 "grammar.y"
{
			do_file( 1, IO_APPEND | IO_WRITE | IO_DO_OPEN, 
				yyvsp[0].str, Line );
		;
    break;}
case 18:
#line 52 "grammar.y"
{
			do_file( yyvsp[-2].num, IO_READ | IO_DO_OPEN, yyvsp[0].str, Line );
		;
    break;}
case 19:
#line 56 "grammar.y"
{
			do_file( 0, IO_READ | IO_DO_OPEN, yyvsp[0].str, Line );
		;
    break;}
case 20:
#line 60 "grammar.y"
{
			do_file( yyvsp[-2].num, IO_WRITE | IO_DO_OPEN, yyvsp[0].str, Line );
		;
    break;}
case 21:
#line 64 "grammar.y"
{
			do_file( 1, IO_WRITE | IO_DO_OPEN, yyvsp[0].str, Line );
		;
    break;}
case 22:
#line 68 "grammar.y"
{
			do_file( yyvsp[-2].num, IO_DO_HERE, yyvsp[0].str, Line );
		;
    break;}
case 23:
#line 72 "grammar.y"
{	
			do_file( 0, IO_DO_HERE, yyvsp[0].str, Line );
		;
    break;}
case 24:
#line 76 "grammar.y"
{
			switch( yyvsp[0].num )
			{
			case -1:
				Line->IoCmd[yyvsp[-2].num].command = IO_DO_CLOSE;
				break;

			default:
				Line->IoCmd[yyvsp[-2].num].command = IO_DO_DUP | IO_WRITE;
				Line->IoCmd[yyvsp[-2].num].fd_source = yyvsp[0].num;
			}
		;
    break;}
case 25:
#line 89 "grammar.y"
{
			switch( yyvsp[0].num )
			{
			case -1:
				Line->IoCmd[1].command = IO_DO_CLOSE;
				break;
			default:
				Line->IoCmd[1].command = IO_DO_DUP | IO_WRITE;
				Line->IoCmd[1].fd_source = yyvsp[0].num;
			}
		;
    break;}
case 26:
#line 101 "grammar.y"
{
			switch( yyvsp[0].num )
			{
			case -1:
				Line->IoCmd[yyvsp[-2].num].command = IO_DO_CLOSE;
				break;
			default:
				Line->IoCmd[yyvsp[-2].num].command = IO_DO_DUP | IO_READ;
				Line->IoCmd[yyvsp[-2].num].fd_source = yyvsp[0].num;
			}
		;
    break;}
case 27:
#line 113 "grammar.y"
{
			switch( yyvsp[0].num )
			{
			case -1:
				Line->IoCmd[0].command = IO_DO_CLOSE;
				break;
			default:
				Line->IoCmd[0].command = IO_DO_DUP | IO_READ;
				Line->IoCmd[0].fd_source = yyvsp[0].num;
			}
	
		;
    break;}
case 28:
#line 127 "grammar.y"
{ 
			if( yyval.num < MAXFILE )
				yyval.num = yyvsp[0].num;
			else
				yyerror( "Bad file number" );
		;
    break;}
case 29:
#line 134 "grammar.y"
{ yyval.num = yyvsp[0].num; ;
    break;}
case 30:
#line 135 "grammar.y"
{ yyval.num = -1; ;
    break;}
case 31:
#line 138 "grammar.y"
{ yyval.str = yyvsp[0].str; ;
    break;}
case 32:
#line 141 "grammar.y"
{ BackGround = TRUE; ;
    break;}
}
   /* the action file gets copied in in place of this dollarsign */
#line 498 "/pkg/osrev/5.7/gcc-2.7.2/share/bison.simple"

  yyvsp -= yylen;
  yyssp -= yylen;
#ifdef YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;

#ifdef YYLSP_NEEDED
  yylsp++;
  if (yylen == 0)
    {
      yylsp->first_line = yylloc.first_line;
      yylsp->first_column = yylloc.first_column;
      yylsp->last_line = (yylsp-1)->last_line;
      yylsp->last_column = (yylsp-1)->last_column;
      yylsp->text = 0;
    }
  else
    {
      yylsp->last_line = (yylsp+yylen-1)->last_line;
      yylsp->last_column = (yylsp+yylen-1)->last_column;
    }
#endif

  /* Now "shift" the result of the reduction.
     Determine what state that goes to,
     based on the state we popped back to
     and the rule number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;

yyerrlab:   /* here on detecting error */

  if (! yyerrstatus)
    /* If not already recovering from an error, report this error.  */
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  int size = 0;
	  char *msg;
	  int x, count;

	  count = 0;
	  /* Start X at -yyn if nec to avoid negative indexes in yycheck.  */
	  for (x = (yyn < 0 ? -yyn : 0);
	       x < (sizeof(yytname) / sizeof(char *)); x++)
	    if (yycheck[x + yyn] == x)
	      size += strlen(yytname[x]) + 15, count++;
	  msg = (char *) malloc(size + 15);
	  if (msg != 0)
	    {
	      strcpy(msg, "parse error");

	      if (count < 5)
		{
		  count = 0;
		  for (x = (yyn < 0 ? -yyn : 0);
		       x < (sizeof(yytname) / sizeof(char *)); x++)
		    if (yycheck[x + yyn] == x)
		      {
			strcat(msg, count == 0 ? ", expecting `" : " or `");
			strcat(msg, yytname[x]);
			strcat(msg, "'");
			count++;
		      }
		}
	      yyerror(msg);
	      free(msg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exceeded");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror("parse error");
    }

  goto yyerrlab1;
yyerrlab1:   /* here on error raised explicitly by an action */

  if (yyerrstatus == 3)
    {
      /* if just tried and failed to reuse lookahead token after an error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
	YYABORT;

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Discarding token %d (%s).\n", yychar, yytname[yychar1]);
#endif

      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token
     after shifting the error token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;

yyerrdefault:  /* current state does not do anything special for the error token. */

#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */
  yyn = yydefact[yystate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
  if (yyn) goto yydefault;
#endif

yyerrpop:   /* pop the current state because it cannot handle the error token */

  if (yyssp == yyss) YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#ifdef YYLSP_NEEDED
  yylsp--;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "Error: state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

yyerrhandle:

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting error token, ");
#endif

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  yystate = yyn;
  goto yynewstate;
}
#line 144 "grammar.y"


yyerror( str )
char *str;
{
	fprintf( stderr, "%s: %s near line number %d\n",
		ProgName,
		str,
		LineNumber );
}

