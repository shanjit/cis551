#define BUFSIZE 65536
#define MAXLINE 200
#define MAX_SYMS 64
#define USERNAMELEN 127
#define PASSWORDLEN 127

#define PROMPT '>'
#define COLON ':'
#define DOLLAR '$'
#define EQUALS '='
#define NEWLINE '\n'
#define EOS '\0'

// for the caesar cipher
#define KEY 1

#define TRUE 1
#define FALSE 0

#define SA struct sockaddr

#define DATABASE "file"
#define CIS551_PORT 10551
#define LISTENQ 5

#define ERR_SOCKET 1
#define ERR_BIND 2
#define ERR_LISTEN 3
#define ERR_ACCEPT 4

#define find_equals(_s) strchr(_s, EQUALS)
#define find_dollar(_s) strchr(_s, DOLLAR)
#define	find_colon(_s) strchr(_s, COLON)

void *malloc();

/* declarations of functions go here */
char 
	*lookup(),
	*strsave(),
	*delete_user();

struct sym_list
	*s_lookup(),
	*new_sym();

void exit();

