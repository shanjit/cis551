/* Header Files */
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>


#define NAMELEN 16
#define PASSLEN 16

#define BUFLEN 512

#define MAX_USERS 8

#define PORTNUM 10551

#define SUCCESS 0
#define USER_ALREADY_EXISTS 1
#define FILE_DOESNT_EXIST 2
#define USER_DOESNT_EXIST 3

typedef struct app_packet
{
	unsigned int control_seq;
	char payload[256];
}app_packet;