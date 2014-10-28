
extern char *ProgName;
extern int LineNumber;
extern int Interactive;
extern char *DefaultPS1;
extern char *DefaultPS2;
extern char *prompt1(), *prompt2();

void
  set_io(),
  do_io();

#include <stdlib.h>
#include <stdio.h>
#include "iocmd.h"

#define MAXLINE 1024

#define TRUE 1
#define FALSE 0

#define EOS '\0'

#ifdef BSD
# include <strings.h>
# include <sys/file.h>
#else
# include <string.h>
# include <fcntl.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>

#define find_equals(_str) strchr(_str,'=')
#define find_dollar(_str) strchr(_str,'$')
#define find_slash(_str) strchr(_str,'/')




