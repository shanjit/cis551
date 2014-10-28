typedef union {
	int num;
	char *str;
} YYSTYPE;
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


extern YYSTYPE yylval;
