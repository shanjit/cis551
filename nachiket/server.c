/*The server creates a connection using the ssock socket and the client connects to a csock socket using the accept command*/


// Control sequence
// 100 - initial auth. payload had username and password
// 101 - update password. payload is the new password
// 102 - add user. payload new username and password.
// 103 - command execute. 



#include "defs.h"

int match(char *user_name, char *passwd)
{
	FILE *database;
	char user_in_file[NAMELEN];
	char passwd_in_file[PASSLEN];
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
	char user_in_file[NAMELEN];
	char passwd_in_file[PASSLEN];
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
	char users_in_file[MAX_USERS][NAMELEN];
	char passwds_in_file[MAX_USERS][PASSLEN];
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
	// interface socket
	int ssock; 
	char data_buf[BUFLEN];
	char name[NAMELEN];
	char pw[PASSLEN];
	char curr_user[NAMELEN];
	char *good = "Welcome to The Machine!\n";
	char *evil = "Invalid identity, exiting!\n";
	
	unsigned int send_mtype;

/*	// make sure the database file exists
	/*create_database();*/

	//-------------------------------------------------------------
/*
	/*Find the default address used for communication, this address needs to be used by the client*/
		// --- find out default IP used for communication ---- //

    //---------------------------------------------------------------

	char buffer[20] = "192.168.42.249";
    struct sockaddr_in servaddr, cliaddr;
    socklen_t len = sizeof(servaddr);
	char servip[20];
	int csock;

	/*create a socket*/
	if ((ssock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("cannot create socket");
		exit(1);
	}

	int optval = 1;
	setsockopt(ssock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);

	signal(SIGPIPE, SIG_IGN);
	
	// --- clear out memory and assign IP parameters --- //
	memset((char *) &servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(buffer);
	servaddr.sin_port = htons(PORTNUM);

	inet_ntop(AF_INET, &servaddr.sin_addr, servip, 20);
	printf("Starting server on: %s %d. \n", servip, ntohs(servaddr.sin_port));
	fflush(stdout);

	// --- bind socket --- //
	if (bind(ssock, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
	{
		perror("Error Binding");
		close(ssock);
		exit(5);
	}

	
	len = sizeof(servaddr);
	int err = getsockname(ssock, (struct sockaddr *) &servaddr, &len);
	
	if( listen(ssock, 5) < 0 ) { /* listen for a connection */
		printf("listen() failed\n");
		close(ssock);
		exit(5);
	}

	// RECEIVING STUFF
	int namelen = sizeof(cliaddr); /* accept connection request */
	char cliip[20];


	while(1)
	{

		printf("waiting\n");

		if((csock = accept(ssock, (struct sockaddr *) &cliaddr, &namelen)) < 0) {
		printf("accept() failed to accept client connection request.\n");
		close(ssock);
		exit(6);
		}
		printf("Connection accepted");

		inet_ntop(AF_INET, &cliaddr.sin_addr, cliip, 20);
		printf("Receiving message from: %s %d.", cliip, ntohs(cliaddr.sin_port));
		// fflush(stdout);
		// PUT RECEIVED DATA INTO data_buf
		int rb;
		do
		{
			rb = recv(csock, data_buf, sizeof(data_buf), 0); /* wait for a client message to arrive */
			if(rb < 0)
			{
			printf("recv() did not get client data\n");
			close(csock);
			close(ssock);
			exit(7);
			}
			printf("recvd");
		
		// PROCESS DATA_BUF and decide what to do in next message
		app_packet *read_packet;
		read_packet = (app_packet *)data_buf;

		printf("%d\n", read_packet->control_seq);

		switch(read_packet->control_seq)
		{
			case 100:
				sscanf(read_packet->payload, "%16[^:]:%s", name,pw);
			/*	printf("%s\n", name);
				printf("%s\n", pw);
			*/
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
				/*printf("%s\n", curr_user);*/
				send_mtype = 201;
				
				break;
			case 102:
				sscanf(read_packet->payload, "%16[^:]:%s", name,pw);
	/*			printf("%s\n", name);
				printf("%s\n", pw);*/
				add_user(name, pw);
				send_mtype = 202;
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
		app_packet *packet = (app_packet *)raw_packet;
		memset(raw_packet, 0, BUFLEN);
		packet->control_seq = send_mtype;
		
		//make a payload with ssprintf
		
		// make payload when printing command outputs, else no payload ;)
		if (packet->control_seq == 106)
		{
			sprintf(packet->payload, "%s:%s", argv[2], argv[3]);
		}

		printf("Before send\n");
		// SEND DATA BACK TO CLIENT
		if(send(csock, packet, sizeof(app_packet), 0) < 0) { /* echo the client message back to the client */
			printf("send() failed to send data back to client.\n");
			close(csock);
			close(ssock);
			exit(8);
		}
		printf("message sent!\n");
	}
	while(rb != 0);
	}
	close(csock);
	close(ssock);
}
