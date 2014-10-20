/*The server creates a connection using the ssock socket and the client connects to a csock socket using the accept command*/


// Control sequence
// 100 - initial auth. payload had username and password
// 101 - update password. payload is the new password
// 102 - add user. payload new username and password.
// 103 - command execute. 




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

#define BUFLEN 512

#define NAMELEN 16
#define PASSLEN 16

#define MAX_USERS 8
#define MAX_UNAME 32
#define MAX_PASSWD 32

#define SUCCESS 0
#define USER_ALREADY_EXISTS 1
#define FILE_DOESNT_EXIST 2
#define USER_DOESNT_EXIST 3

// interface socket
int ssock; 

char curr_user[MAX_UNAME];

struct app_packet
{
	u_char control_seq;
	char payload[256];
};


int match1(char *s1, char *s2)
{
	while( *s1 != '\0' && *s2 != 0 && *s1 == *s2 ){
			s1++; s2++;
	}
	return( *s1 - *s2 );
}

int match(char *user_name, char *passwd)
{
	FILE *database;
	char user_in_file[MAX_UNAME];
	char passwd_in_file[MAX_PASSWD];
	database = fopen("./database.txt", "r");
	while(fscanf(database, "%s\t%s\n", user_in_file, passwd_in_file) != EOF)
	{
		if(strcmp(user_in_file, user_name) == 0)
		{
			if(strcmp(passwd_in_file, passwd) == 0)
				return 0;
			else
				return 1;
		}
	}
	fclose(database);
	return 1;
}


void welcome(char *str) 
	{
		printf(str); 
	}


void goodbye(char *str) {void exit(); printf(str); 
	/*exit(1); */
}


int is_user_present(char *user_name, FILE *database)
{
	char user_in_file[MAX_UNAME];
	char passwd_in_file[MAX_PASSWD];
	while(fscanf(database, "%s\t%s\n", user_in_file, passwd_in_file) != EOF)
	{
		if(strcmp(user_in_file, user_name) == 0)
		{
			printf("user is present\n");
			return 1;
		}
	}
	return 0;
}
		
int add_user(char *user_name, char *passwd)
{
	FILE *database;
	database = fopen("database.txt", "r");
	if(is_user_present(user_name, database))
	{
		return USER_ALREADY_EXISTS;
	}
	fclose(database);
	database = fopen("database.txt", "a");
	fprintf(database, "%s\t%s\n", user_name, passwd);
	fclose(database);
	return SUCCESS;
}



int update_passwd(char *user_name, char *passwd)
{
	FILE *database;
	int i = 0;
	char users_in_file[MAX_USERS][MAX_UNAME];
	char passwds_in_file[MAX_USERS][MAX_PASSWD];
	database = fopen("database.txt", "r");
	if(!database)
	{
		return FILE_DOESNT_EXIST;
	}

	if(!is_user_present(user_name, database))
	{
		printf("user not found\n");
		return USER_DOESNT_EXIST;
	}
	rewind(database);
	while(fscanf(database, "%s\t%s\n", users_in_file[i], passwds_in_file[i]) != EOF)
	{
		printf("%s\n",users_in_file[i]);
		i++;
	}
	fclose(database);
	database = fopen("database.txt", "w");
	i--;

	while(i>=0)
	{
		if(strcmp(users_in_file[i], user_name) == 0)
			fprintf(database, "%s\t%s\n", users_in_file[i], passwd);
		else
			fprintf(database, "%s\t%s\n", users_in_file[i], passwds_in_file[i]);
		i--;
	}
	fclose(database);
	return SUCCESS;
}


int create_database()
{
	FILE *database;
	database = fopen("./database.txt", "w");
	if(!database)
		return -1;
	fclose(database);
	return SUCCESS;
}




int main(int argc, char *argv[])
{
	char data_buf[512];
	char name[NAMELEN];
	char pw[PASSLEN];
	char *good = "Welcome to The Machine!\n";
	char *evil = "Invalid identity, exiting!\n";
	
	u_char send_mtype;

	// make sure the database file exists
	/*create_database();*/

	//-------------------------------------------------------------

	/*Find the default address used for communication, this address needs to be used by the client*/
		// --- find out default IP used for communication ---- //
	const char* google_dns_server = "8.8.8.8";
    int dns_port = 53;

    /*define sockkaddr: structure used to specify local and remote endpoint addresses */

    struct sockaddr_in serv; 
    /*open a socket*/
    int sock = socket (AF_INET, SOCK_STREAM, 0); /*SOCK_STREAM is for TCP and SOCK_DGRAM is for UDP*/
    if(sock < 0)
    {
        perror("Socket error");
    }

	/*Start filling the sockaddr structures*/
    memset(&serv, 0, sizeof(serv));
    serv.sin_family = AF_INET; /*refers to the address family, use AF_INET6 for ipv6*/
    serv.sin_addr.s_addr = inet_addr(google_dns_server); /*ip address*/
    serv.sin_port = htons(dns_port); /*port number*/

    /* No binding here because the port would already be in use*/

    serv.sin_addr.s_addr = inet_addr(google_dns_server); /*ip address*/
    serv.sin_port = htons(dns_port); /*port number*/

    if(connect(sock , (const struct sockaddr*) &serv , sizeof(serv)) < 0)
    {
    	perror("Connect Error");
    	close(sock);
    	return -1;
    }

    struct sockaddr_in servname;
    socklen_t namelen = sizeof(servname);
    /*getsockname stores the address that is bound to a specified socket sock in the buffer pointed to by name*/
    int err = getsockname(sock, (struct sockaddr*) &servname, &namelen); 
    char buffer[100];
    const char* p = inet_ntop(AF_INET, &servname.sin_addr, buffer, 100);
    if(p == NULL)
    {
        //Some error
        printf("Error Occured.");
    }
    close(sock);

    //---------------------------------------------------------------

    /*this is the address on which the client needs to connect*/
    printf("%s\n", buffer);

    struct sockaddr_in servaddr, cliaddr;
    socklen_t len = sizeof(servaddr);
	char servip[20];
	int csock;

	/*create a socket*/
	ssock = socket(AF_INET, SOCK_STREAM, 0);
	
	// --- clear out memory and assign IP parameters --- //
	memset((char *) &servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(buffer);
	servaddr.sin_port = htons(10551);

	// --- bind socket --- //
	if (bind(ssock, (struct sockaddr *) &servaddr, sizeof(servaddr)))
	{
		perror("Error Binding");
	}
	
	len = sizeof(servaddr);
	err = getsockname(ssock, (struct sockaddr *) &servaddr, &len);
	
	if( listen(ssock, 1) != 0 ) { /* listen for a connection */
		fprintf(stderr, "listen() failed\n");
		exit(5);
	}

	// RECEIVING STUFF
	namelen = sizeof(cliaddr); /* accept connection request */

	if( (csock = accept(ssock, (struct sockaddr *) &cliaddr, &namelen)) == -1) {
		fprintf(stderr, "accept() failed to accept client connection request.\n");
		exit(6);
	}


while(1)
{
	

	printf("hey!!\n");


	// PUT RECEIVED DATA INTO data_buf
	if( recv(csock, data_buf, sizeof(data_buf), 0) == -1 ) { /* wait for a client message to arrive */
		fprintf(stderr, "recv() did not get client data\n");
		exit(7);
	}
	
	// PROCESS DATA_BUF and decide what to do in next message
	struct app_packet *read_packet;
	read_packet = (struct app_packet *)data_buf;
	
	printf("%s\n", read_packet);

	printf("%d\n", read_packet->control_seq);

	switch(read_packet->control_seq)
	{
		case 100:
				sscanf(read_packet->payload, "%16[^:]:%s", name,pw);
				printf("%s\n", name);
				printf("%s\n", pw);
			
				if( match(name,pw) == 0 )
				{
				welcome(good);
				strcpy(curr_user, name);
				send_mtype = 200; //telling the user he is good.
				}
				else
				{
				goodbye(evil);
				send_mtype = 204; // telling the user is out.
				}
				

			break;
		case 101:
			update_passwd(curr_user, read_packet->payload);
			printf("%s\n", curr_user);
			send_mtype = 200;
			
			break;
		case 102:
			sscanf(read_packet->payload, "%16[^:]:%s", name,pw);
			printf("%s\n", name);
			printf("%s\n", pw);
			add_user(name, pw);
			send_mtype = 200;
			break;



		case 103:


			break;


		case 104:

			break;

		default:

			printf("stuff happened\n");
	}
	
	
	/*printf("%s\n", read_packet->payload);*/
	// see the sscanf example here 
/*	char username[NAMELEN];
	char password[PASSLEN];
	sscanf(read_packet->payload, "%16[^:]:%s", username, password);
	printf("%s\n", username);
	printf("%s\n", password);
*/


 	// -- Send the message back to the client

	char raw_packet[BUFLEN];
	struct app_packet *packet = (struct app_packet *)raw_packet;
	memset(raw_packet, 0, BUFLEN);
	packet->control_seq = send_mtype;
	
	//make a payload with ssprintf
	
	// make payload when printing command outputs, else no payload ;)
	if (packet->control_seq == 106)
	{
		sprintf(packet->payload, "%s:%s", argv[2], argv[3]);
	}

	/*strcpy(packet->payload, "Hey!");
	*/
	
	// SEND DATA BACK TO CLIENT
	if( send( csock, packet, sizeof(struct app_packet), 0) < 0) { /* echo the client message back to the client */
		fprintf(stderr, "send() failed to send data back to client.\n");
		exit(8);
	}
	printf("message sent!\n");



} 




	close(csock);
	close(ssock);


}
