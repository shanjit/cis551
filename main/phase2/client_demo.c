/*This is the client file for phase 1 of homework 2 for CIS 551 Fall 2014

Contributors:
Shanjit Singh Jajmann
Nachiket Nanadikar
Ameya Moore
*/


#include <sys/ioctl.h>
#include "demo.h"
#include <unistd.h>

// Encryption the plaintext string and change the ciphertext string 
void encrypt(char *plaintext, char *ciphertext)
{
  int i = 0;
  while(plaintext[i] !='\0')
  {
    ciphertext[i] = plaintext[i] + 7;
    i++;
  }
  ciphertext[i] = '\0';
}

// Decrypt the plaintext string and change the ciphertext string 
void decrypt(char *ciphertext, char *plaintext)
{
  int i = 0;
  while(ciphertext[i] !='\0')
  {
    plaintext[i] = ciphertext[i] - 7;
    i++;
  }
  plaintext[i] = '\0';
}

// Takes the decrypted text, encrypts it and then sends it to the server
// Return 1 if success else return 0
int sendToServer(char *decrypted, FILE *address)
{
  char encrypted[BUFSIZE];
  
  //Encrypt data first and then send. 
  // Uncomment the lines below to see the encryption in action
  /*printf("Plain Text %s\n", decrypted);*/
  encrypt(decrypted, encrypted);
  /*printf("Cipher Text %s\n", encrypted);*/
  strcat(encrypted, "\n");
  if(fputs(encrypted,address)==EOF)
  {
    return 0;
  }
  fflush(address);
  return 1;
  }


// fgets the encrypted data from the server and then decrypts it before 
// putting it in the decrpyted string
// Returns 1 if success else 0
int recvFromServer(char *decrypted, FILE *address)
{

  char encrypted[BUFSIZE];

  if (fgets( encrypted, BUFSIZE, address ) !=NULL)
  {
    encrypted[strlen(encrypted)-1] = '\0'; 
    // Decrypt data first
    // Uncomment the lines below to see the decryption in action
    /*printf("Cipher Text %s\n", encrypted);*/
    decrypt(encrypted, decrypted);
    /*printf("Plain Text %s\n", decrypted);*/
    fflush(stdout);
    return 1;
  }

  else {
    // Error in fgets
    return 0;
  }
}



main( int argc, char *argv[] )
{
  // Define the receive buffer
  char recv_buf[BUFSIZE];

  // Define the send buffer
  char send_buf[BUFSIZE];

  // The replies sent by the server. These strings are just used for comparison
  // good and evil from HW1
  char *good = "Welcome to The Machine!\n";
  char *evil = "Invalid identity, exiting!\n";

  size_t max_buf = BUFSIZE;
 
  // Socket Stuff goes here
  int sockfd;
  struct sockaddr_in servaddr;

  // Create a file pointer to keep track of the file being accessed. 
  // Two separate pointers are made for the server_req and server_rep
  // See https://www.cs.bu.edu/teaching/c/file-io/intro/ for usage. 
  FILE *server_req, *server_rep;

  // See if the ip address entered
  if( argc != 2 )
    {
      fprintf(stderr, "Usage: %s <IP address>\n", argv[0] );
      exit ( 1 );
    }

  /* Open up a TCP/IP socket */
  if( (sockfd = socket(AF_INET, SOCK_STREAM, 0 )) < 0 )
    {
      perror( "socket open" );
      exit( ERR_SOCKET );
    }

  /* initialize and prepare for connections to a server */
  bzero( &servaddr, sizeof(servaddr ));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons( CIS551_PORT );

  /* convert address, e.g., 127.0.0.1, to the right format */
  if( inet_pton( AF_INET, argv[1], &servaddr.sin_addr ) <= 0 )
    {
      perror( "inet_pton for address" );
      exit( 99 );
    }

  /* connect to the server */
  if( connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0 )
    {
      fprintf(stderr, "Connection Refused, Server is down\n");
      exit( 100 );
    }

  // server_req is anything you request from the server
  server_req = fdopen( sockfd, "w" );
  if( server_req == (FILE *) NULL )
    {
      perror( "fdopen of stream for server requests" );
      exit( 2 );
    }

  // server_rep is the reply of the server 
  server_rep = fdopen( sockfd, "r" );
  if( server_rep == (FILE *) NULL )
    {
      perror( "fdopen of stream for server replies" );
      exit( 3 );
    }

    // While loop to maintain communication
    printf("Connection established\n");
    while (1) {
      // after the connection gets established I am going to wait for the server to send me the request to authenticate my username and password.
      // The server is going to be the one to initiate the conversation by asking for the username and password from the client
       if (recvFromServer(recv_buf, server_rep) != 0) {

        fprintf(stdout,"Server: %s", recv_buf);
        if(strcmp(recv_buf,good)==0){
          //
          // If the user gets authenticated
          // This is the User interface showed to the client.
          //
          //
        printf("Add or Update user by: user,password\n");
		    printf("Add MAC by: addmac mac\n");
	      printf("Delete MAC by: deletemac mac\n");
        break; 
        
        }


        else if(strcmp("Exiting\n", recv_buf)==0)
        {
          // If the server sends Exiting then exit.
          // Only run during the authentication process
          printf("Shell Code found! Exiting!\n");
          exit(0);
        }
         else if(strcmp(recv_buf,evil)==0) {
          fclose( server_req );
          fclose( server_rep );
          close( sockfd);
          exit( 0 );
        }

        if (fgets( send_buf, BUFSIZE, stdin ) != NULL) {
          // Get input from the standard input and put it into send_buf,
          // put this as server request and send it to the server.
          sendToServer(send_buf, server_req);
          
        }
      }
    }

  // The user comes here if he gets authenticated
  // make a prompt with '>' 
  // send _buf is the buffer which has anything you type in to the keyboard (stdin)
  for( putchar('>');
     (fgets( send_buf, BUFSIZE, stdin ) != NULL );
      putchar('>'))
  {

      if(strcmp(send_buf,"exit\n")==0)
      { 
        // if you enter exit then you exit(0)
        sendToServer("exit\n", server_req);
        exit(0);
      }

      //Upon successful completion, fputs() shall return a non-negative number. Otherwise, it shall return EOF, set an error indicator for the stream, [CX] [Option Start]  and set errno to indicate the error. [Option End]
      if( sendToServer(send_buf, server_req) == 0)
      {
        perror( "Send to server failed." );
      }

      // Sent the server_req to the server
      fflush( server_req );

    // Ameya's code to handle shell code output from the server
    int finished_reading = 0, len;
      while(1)
      {
        finished_reading;
              len =0;
        //         sleep(2);
        ioctl(sockfd, FIONREAD, &len);
        if (len > 0) 
        {
          //printf("%d\n", len);
          len = read(sockfd, recv_buf, len);
          char decrypted[BUFSIZE];
          /*fprintf(stdout, recv_buf);*/
          // Make sure you do decryption for everything, because the server always sends via encryption
          recv_buf[strlen(recv_buf)-1] = '\0'; 
          decrypt(recv_buf,decrypted);
          fprintf(stdout, decrypted);
          /*fflush(stdout);*/
          memset(recv_buf,0,strlen(recv_buf));
          finished_reading = 1;
        }
        if((len == 0) && (finished_reading == 1))
    		break;
      }

      
  }

  /* shut things down */
  fclose( server_req );
  fclose( server_rep );
  close( sockfd);

  exit( 0 );
}
