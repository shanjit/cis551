//
// Anything sent to the server is server_req
// Anything got from the server is the server_rep

/*This is the client file for phase 1 of homework 2 for CIS 551 Fall 2014

Contributors:
Shanjit Singh Jajmann
Nachiket Nanadikar
Ameya Moore
*/

// Things to do:
// Wrong shell command should through an error.
// 


#include <sys/ioctl.h>
#include "demo.h"
#include <unistd.h>

void encrypt(char *msg, int key)
{
  char *p = msg;

  while (*p)
  {
     if ('a' <= *p && *p <= 'z') {
            *p = 'a' + (*p - 'a' + key) % 26;
        }
        p++;
  }

}

void decrypt(char *msg, int key)
{
  char *p = msg;

  while (*p)
  {
     if ('a' <= *p && *p <= 'z') {
            *p = 'a' + (*p - 'a' - key) % 26;
        }
        p++;
  }

}



int sendToServer(char *msg, FILE *address)
{
  //Encrypt data first and then send. 
  // encrypt(msg, KEY);

  if(fputs(msg,address)==EOF)
  {

    return 0;
  }
  fflush(address);
  return 1;
  }


int recvFromServer(char *msg, FILE *address)
{
  if (fgets( msg, BUFSIZE, address ) !=NULL)
  {


    // Decrypt data first
    // decrypt(msg, KEY);

    fflush(stdout);

    // CHECK FOR SHELL CODE HERE!


    return 1;
  }

  else {
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

  /* attempt to make the actual connection */
  if( connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0 )
    {
      fprintf(stderr, "Connection Refused, Server is down\n");
      exit( 100 );
    }

  /* setup the interfaces between the new socket and stdio system */
  // server_requet is anything you request from the server
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

  /* The main interactive loop, getting input from the user and
   * passing to the server, and presenting replies from the server to
   * the user, as appropriate. Lots of opportunity to generalize
   * this primitive user interface...
   */
    printf("Connection established\n");
    while (1) {
      // after the connection gets established I am going to wait for the server to send me the request to authenticate my username and password.
      // The server is going to be the one to initiate the conversation by asking for the username and password from the client
      /*if(fgets(recv_buf,BUFSIZE,server_rep)!=NULL) {*/
        if (recvFromServer(recv_buf, server_rep) != 0) {

        fprintf(stdout,"Server: %s", recv_buf);
        if(strcmp(recv_buf,good)==0){
          //
          //
          //
          // This is the User interface showed to the client.
          // Add modifications of MAC id here
          //
          //
          printf("Add or Update user by: user:password\n");
          break; // break out from while (1)
        
        } else if(strcmp(recv_buf,evil)==0) {
          // request is denied => Close the server_req and the server_req and the socket
          // close the server_req, server_rep file descriptors if auth failed.
          fclose( server_req );
          fclose( server_rep );
          close( sockfd);
          // exit the program
          exit( 0 );
        }

        if (fgets( send_buf, BUFSIZE, stdin ) != NULL) {
          // Get input from the standard input and put it into send_buf,
          // put this as server request and send it to the server.
          sendToServer(send_buf, server_req);
          
        }
      }
    }

  // Run this when authenticated.
  // make a prompt with '>' 
  for( putchar('>');
     (fgets( send_buf, BUFSIZE, stdin ) != NULL );
      putchar('>'))
  {
      //Upon successful completion, fputs() shall return a non-negative number. Otherwise, it shall return EOF, set an error indicator for the stream, [CX] [Option Start]  and set errno to indicate the error. [Option End]
      if( sendToServer(send_buf, server_req) == 0)
      {
        perror( "write failure to associative memory at server" );
      }

      // Sent the server_req to the server
      /*fflush( server_req );   buffering everywhere.... */



    // Ameya's Code - Not sure of working.
    // The code below breaks a lot of functionality right now. 
    // Needs  to verify and check. 
/*    int finished_reading = 0, len;
      while(1)
      {
        finished_reading;
              len =0;
        //         sleep(2);
        ioctl(sockfd, FIONREAD, &len);
        if (len > 0) 
        {
          printf("%d\n", len);
          len = read(sockfd, recv_buf, len);
          fprintf(stdout, recv_buf);
          fflush(stdout);
          memset(recv_buf,'\0',strlen(recv_buf));
          finished_reading = 1;
        }
        if((len == 0) && (finished_reading == 1))
    break;
      }*/

      // Shan's Code commented out by Ameya.
      // Get the server reply and put into recv_buf
      /*if( fgets( recv_buf, BUFSIZE, server_rep ) == NULL )*/
  if( recvFromServer( recv_buf, server_rep ) == 0 )
      {
        perror( "read failure from associative memory at server");
      }
      // print whatever the server sends to the stdout
      fprintf(stdout,"Server: %s",recv_buf);

      // If the server sends exiting, you better exit as well.
      if(strcmp(recv_buf,"Exiting\n")==0)
        break;
  }

  /* shut things down */
  fclose( server_req );
  fclose( server_rep );
  close( sockfd);

  exit( 0 );
}
