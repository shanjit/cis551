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

// interface socket
int ssock; 


struct app_packet
{
	u_char control_seq;
	char payload[256];
};


int match (char *s1, char *s2)
{
	while( *s1 != '\0' && *s2 != 0 && *s1 == *s2 ){
			s1++; s2++;
	}
	return( *s1 - *s2 );
}
/*

void welcome(char *str) {printf(str); }
void goodbye(char *str) {void exit(); printf(str); exit(1); }

*/

int main(int argc, char *argv[])
{
	char data_buf[512];

	u_char send_mtype;

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

    struct sockaddr_in name;
    socklen_t namelen = sizeof(name);
    /*getsockname stores the address that is bound to a specified socket sock in the buffer pointed to by name*/
    int err = getsockname(sock, (struct sockaddr*) &name, &namelen); 
    char buffer[100];
    const char* p = inet_ntop(AF_INET, &name.sin_addr, buffer, 100);
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

while(1)
{
	// RECEIVING STUFF
	namelen = sizeof(cliaddr); /* accept connection request */
	if( (csock = accept(ssock, (struct sockaddr *) &cliaddr, &namelen)) == -1) {
		fprintf(stderr, "accept() failed to accept client connection request.\n");
		exit(6);
	}

	// PUT RECEIVED DATA INTO data_buf
	if( recv(csock, data_buf, sizeof(data_buf), 0) == -1 ) { /* wait for a client message to arrive */
		fprintf(stderr, "recv() did not get client data\n");
		exit(7);
	}


	// PROCESS DATA_BUF and decide what to do in next message
	struct app_packet *read_packet;
	read_packet = (struct app_packet *)data_buf;

	printf("%d\n", read_packet->control_seq);

	switch(read_packet->control_seq)
	{
		case 100:
				char name[NAMELEN];
				char password[PASSLEN];
				sscanf(read_packet->payload, "%16[^:]:%s", name,password);
				printf("%s\n", name);
				printf("%s\n", password);
				send_mtype = 0b00000010;
				printf("%d\n",send_mtype);

				char *good = "Welcome to The Machine!\n";
				char *evil = "Invalid identity, exiting!\n";


				if( match(name,pw) == 0 )
				welcome( good );
				

				else
				goodbye(evil );
				// find the username and password to authenticate with 





			break
		case 101:





			break;
		case 102:


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
	char username[NAMELEN];
	char password[PASSLEN];
	sscanf(read_packet->payload, "%16[^:]:%s", username, password);
	printf("%s\n", username);
	printf("%s\n", password);


	// SEND DATA BACK TO CLIENT
	if( send( csock, data_buf, sizeof(data_buf), 0) < 0) { /* echo the client message back to the client */
		fprintf(stderr, "send() failed to send data back to client.\n");
		exit(8);
	}


} 




	close(csock);
	close(ssock);


}