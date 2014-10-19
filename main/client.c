
/*Max packet size is 255, but max buffer size if 512*/


// Control sequence
// 200 - Authentication done. 
// 201 - My next message is going to be the new password. 
// 202 - My next message is going to be adding a new user with username and password.
// 204 - Not authenticated
// 203 - this is the output of a command i sent earlier. 


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


#define NAMELEN 16
#define PASSLEN 16

char username[NAMELEN];
char password[PASSLEN];
char ip[16]; // change this depending on ipv6 or ipv4

#define BUFLEN 255
// interface socket
int ssock; 

struct app_packet
{
	u_char control_seq;
	char payload[256];
};




int main(int argc, char *argv[])
{
	char servip[20];

	if(argc!=4)
	{
		printf("Incorrect Usage!! \n");
		printf("Usage : <ip> <username> <password> \n");
		return 0;
	}
	
	else if(argc==4)
	{
		strncpy(servip, argv[1], 16);
		strncpy(username, argv[2], NAMELEN);
		strncpy(password, argv[3], PASSLEN);
		printf("Connecting to:%s with username:%s and pass:%s \n", ip, username, password);
	}


	// --------------------------------------------------------------

	/*Find the default address used for communication, this address needs to be used by the client*/
		// --- find out default IP used for communication ---- //
	const char* google_dns_server = "8.8.8.8";
    int dns_port = 53;

    /*define sockkaddr: structure used to specify local and remote endpoint addresses
	struct sockaddr_in{
	  short sin_family;
	  unsigned short sin_port;
	  IN_ADDR sin_addr;
	  char sin_zero[8];
	};
	*/

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
    // if (bind(sock, (struct sockaddr*) &serv, sizeof(serv)) < 0)
    // {
    // 	idealy should also handle ADDR_IN_USE error
    // 	perror("Bind Error");
    // 	close(sock);
    // 	return -1;
    // }

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


	char cliip[20];
	struct sockaddr_in cliaddr, servaddr;
	socklen_t len = sizeof(cliaddr);


	// --- server information --- //
	// --- clear out memory and assign IP parameters --- //
	/* Who to talk to */
	memset((char *) &servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	inet_pton(AF_INET, servip, &servaddr.sin_addr);		
	servaddr.sin_port = htons(atoi("10551"));

	// --- client initialization --- //
	ssock = socket(AF_INET, SOCK_STREAM, 0);

	// --- clear out memory and assign IP parameters --- //
	memset((char *) &cliaddr, 0, sizeof(cliaddr));
	cliaddr.sin_family = AF_INET;
	cliaddr.sin_addr.s_addr = inet_addr(buffer);
	cliaddr.sin_port = htons(INADDR_ANY);

	// --- bind socket --- //
	bind(ssock, (struct sockaddr *) &cliaddr, sizeof(cliaddr));
	err = getsockname(sock, (struct sockaddr *) &cliaddr, &len);
	
	// --- print out initialization status --- //
	inet_ntop(AF_INET, &servaddr.sin_addr, servip, 20);
	inet_ntop(AF_INET, &cliaddr.sin_addr, cliip, 20);

	printf("Sending message to: %s %d. listening on %s %d\n", servip, ntohs(servaddr.sin_port), cliip, ntohs(cliaddr.sin_port));


	if (connect(ssock, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
		perror("connect failed");
		close(ssock);
		return -1;
	}

	char raw_packet[BUFLEN];
	struct app_packet *packet = (struct app_packet *)raw_packet;
	memset(raw_packet, 0, BUFLEN);
	packet->control_seq = 100;
	
	//make a payload with ssprintf
	sprintf(packet->payload, "%s:%s", argv[2], argv[3]);
	/*strcpy(packet->payload, "Hey!");
	*/
	
	send(ssock, packet, sizeof(struct app_packet), 0);
	printf("message sent!\n");


return 0;





}