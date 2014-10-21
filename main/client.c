
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



int show_menu()
{
	int option;
	printf("Choose among the following options:\n");
	printf("1. Update your password:\n");
	printf("2. Add new User:\n");
	/*printf("3. Execute Command on drone:\n");*/
	
	printf("Please Enter Option: ");
	scanf("%d", &option);
	return option;
}

int main(int argc, char *argv[])
{
	char servip[20];
	char data_buf[512];
	u_char send_mtype;

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
		printf("Connecting to:%s with username:%s and pass:%s \n", servip, username, password);
	}


	struct sockaddr_in servaddr;
	// --- server information --- //
	// --- clear out memory and assign IP parameters --- //
	/* Who to talk to */

	memset((char *) &servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	inet_pton(AF_INET, servip, &servaddr.sin_addr);		
	servaddr.sin_addr.s_addr = inet_addr(argv[1]);
	servaddr.sin_port = htons(atoi("10551"));

	// --- client initialization --- //
	ssock = socket(AF_INET, SOCK_STREAM, 0);

	send_mtype = 100;

	if (connect(ssock, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
		perror("connect failed");
		close(ssock);
		return -1;
	}

	
	inet_ntop(AF_INET, &servaddr.sin_addr, servip, 20);
	/*printf("Connecting to : %s %d.", servip, ntohs(servaddr.sin_port));*/
	fflush(stdout);

	char raw_packet[BUFLEN];
	struct app_packet *packet = (struct app_packet *)raw_packet;
	int issent;
	struct app_packet *read_packet;


while(1)
{
	memset(raw_packet, 0, BUFLEN);
	packet->control_seq = send_mtype;


	if (send_mtype==100)
		{
			sprintf(packet->payload, "%s:%s", argv[2], argv[3]);
		}	
	else if(send_mtype==101)
	{
		char new_pass[PASSLEN];
		printf("Enter new password: ");
		scanf("%s", new_pass);
		sprintf(packet->payload, "%s", new_pass);
		/*strcpy(packet->payload, new_pass);*/
	}

	else if(send_mtype==102)
	{
		char new_pass[PASSLEN];
		char new_user[NAMELEN];
		printf("Enter new username: ");
		scanf("%s", new_user);		
		printf("Enter new password: ");
		scanf("%s", new_pass);

		sprintf(packet->payload, "%s:%s", new_user, new_pass);

	}

	else if(send_mtype = 103)
	{
		// control the drone




	}

	/*if send_mtype==something*/
	//make a payload with ssprintf
	/*sprintf(packet->payload, "%s:%s", argv[2], argv[3]);*/
	/*strcpy(packet->payload, "Hey!");
	*/
	

	issent = send(ssock, packet, sizeof(struct app_packet), 0);

/*
	printf("%d\n", issent);
	printf("message sent!\n");
*/

	// PUT RECEIVED DATA INTO data_buf
	if( recv(ssock, data_buf, sizeof(data_buf), 0) == -1 ) { /* wait for a client message to arrive */
		fprintf(stderr, "recv() did not get client data\n");
		exit(7);
	}

	// PROCESS DATA_BUF and decide what to do in next message
	
	read_packet = (struct app_packet *)data_buf;

/*	printf("%d\n", read_packet->control_seq);
*/

switch(read_packet->control_seq)
	{	
		case 200:
		{
			int option = show_menu();

			if (option==1)
			{	// Change password in next message
				send_mtype = 101;
/*
				printf("%d\n",ssock);
	*/
			}
			else if(option==2)
			{
				// Add user 
				send_mtype = 102;
			}

/*			else if (option == 3)
			{	
				// drone command control
				send_mtype = 102;
				/*send_mtype = 200; DECISION YET TO BE MADE*/
			//}

			else 
			{
				printf("Incorrect Option entered. Program exiting. \n\n");
				exit(1);
			}

			break;
		}

		case 201:
		{
			printf("Password Updated. \n");
			int option = show_menu();

			if (option==1)
			{	// Change password in next message
				send_mtype = 101;
/*
				printf("%d\n",ssock);
	*/
			}
			else if(option==2)
			{
				// Add user 
				send_mtype = 102;
			}

/*			else if (option == 3)
			{	
				// drone command control
				send_mtype = 102;
				
			}*/

			else 
			{
				printf("Incorrect Option entered. Program exiting. \n\n");
				exit(1);
			}

			break;
		}

		case 202:
		{
			printf("New User added. \n");
			int option = show_menu();

			if (option==1)
			{	// Change password in next message
				send_mtype = 101;
/*
				printf("%d\n",ssock);
	*/
			}
			else if(option==2)
			{
				// Add user 
				send_mtype = 102;
			}

/*			else if (option == 3)
			{	
				// drone command control
				send_mtype = 102;
				
			}*/

			else 
			{
				printf("Incorrect Option entered. Program exiting. \n\n");
				exit(1);
			}

			break;
		}

		case 204:
			close(ssock);
			printf("Incorrect Authentication! Exiting.\n");
			exit(1);
			break;

		default:

			printf("stuff happened\n");
	}









}

return 0;





}