
/*Max packet size is 255, but max buffer size if 512*/


// Control sequence
// 200 - Authentication done. 
// 201 - My next message is going to be the new password. 
// 202 - My next message is going to be adding a new user with username and password.
// 204 - Not authenticated
// 203 - this is the output of a command i sent earlier. 


#include "defs.h"

int show_menu()
{
	int option;
	printf("Choose among the following options:\n");
	printf("1. Update your password:\n");
	printf("2. Add new User:\n");
	printf("3. Execute Command on drone:\n");

	printf("Please Enter Option: ");
	scanf("%d", &option);
	return option;
}

int main(int argc, char *argv[])
{
	char username[NAMELEN];
	char password[PASSLEN];
	char servip[20];
	char data_buf[BUFLEN];
	unsigned int send_mtype;
	// interface socket
	int ssock; 

	if(argc!=4)
	{
		printf("Incorrect Usage!! \n");
		printf("Usage : <ip> <username> <password> \n");
		return 0;
	}

	else if(argc==4)
	{
		strncpy(servip, argv[1], 20);
		strncpy(username, argv[2], NAMELEN);
		strncpy(password, argv[3], PASSLEN);
		printf("Connecting to:%s with username:%s and pass:%s \n", servip, username, password);
	}


	struct sockaddr_in servaddr;

	memset((char *) &servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	inet_pton(AF_INET, servip, &servaddr.sin_addr);		
	servaddr.sin_addr.s_addr = inet_addr(argv[1]);
	servaddr.sin_port = htons(atoi("10551"));

	// --- client initialization --- //
	ssock = socket(AF_INET, SOCK_STREAM, 0);

	int optval = 1;
	setsockopt(ssock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);

	send_mtype = 100;
	/* Connect to the server */
	if (connect(ssock, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
		perror("connect failed");
		close(ssock);
		exit(1);
	}


	inet_ntop(AF_INET, &servaddr.sin_addr, servip, 20);
	fflush(stdout);

	char raw_packet[BUFLEN];
	app_packet *packet = (app_packet *)raw_packet;
	int issent;
	app_packet *read_packet;


	while(1)
	{
		memset(raw_packet, 0, BUFLEN);
		packet->control_seq = send_mtype;
		
		/* Initially request for authentication from the server */
		if (send_mtype==100)
		{
			sprintf(packet->payload, "%s:%s", argv[2], argv[3]);
		}
		/* If the user has been authenticated successfully then depending on 
		   service chosen by user, get input from user */	
		else if(send_mtype==101)
		{
			char new_pass[PASSLEN];
			printf("Enter new password: ");
			scanf("%s", new_pass);
			sprintf(packet->payload, "%s", new_pass);
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
			/* Unused case for phase 1*/
		}

		/* Send data entered by user to server */
		issent = send(ssock, packet, sizeof(app_packet), 0);

	        /* Receive response from server */	
		if( recv(ssock, data_buf, sizeof(data_buf), 0) == -1 ) { /* wait for a client message to arrive */
			fprintf(stderr, "recv() did not get client data\n");
			close(ssock);
			exit(7);
		}

		read_packet = (app_packet *)data_buf;

		/* Depending on response from server, display cases to user */
		switch(read_packet->control_seq)
		{	
			case 200:
				{
					int option = show_menu();

					if (option==1)
					{	// Change password in next message
						send_mtype = 101;
					}
					else if(option==2)
					{
						// Add user 
						send_mtype = 102;
					}

					else if (option == 3)
					{	
						/* Unused case for phase 1*/
						send_mtype = 102;
						/*send_mtype = 200; DECISION YET TO BE MADE*/
					}

					else 
					{
						printf("Incorrect Option entered. Program exiting. \n\n");
						close(ssock);
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
					}
					else if(option==2)
					{
						// Add user 
						send_mtype = 102;
					}

					else if (option == 3)
					{	
						// drone command control
						send_mtype = 102;
						/*send_mtype = 200; DECISION YET TO BE MADE*/
					}

					else 
					{
						printf("Incorrect Option entered. Program exiting. \n\n");
						close(ssock);
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
					}
					else if(option==2)
					{
						// Add user 
						send_mtype = 102;
					}

					else if (option == 3)
					{	
						// drone command control
						send_mtype = 102;
						/*send_mtype = 200; DECISION YET TO BE MADE*/
					}

					else 
					{
						printf("Incorrect Option entered. Program exiting. \n\n");
						close(ssock);
						exit(1);
					}

					break;
				}

			case 204:
				printf("Incorrect Authentication! Exiting.\n");
				close(ssock);
				exit(1);
				break;

			default:
				printf("stuff happened\n");
		}
	}
	return 0;
}
