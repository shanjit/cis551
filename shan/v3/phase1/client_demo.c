//
// Anything sent to the server is server_request
// Anything got from the server is the server_reply



/*This is the client file for phase 1 of homework 2 for CIS 551 Fall 2014

Contributors:
Shanjit Singh Jajmann
Nachiket Nanadikar
Ameya Moore
*/

/*
 * demo client
 * It provides an associative memory for strings
 * strings are named with an assignment, e.g.,
 * a=mary had a little lamb
 * They are retrieved by used of their name prepended by a '$' character
 * presented to the prompt ('>'), e.g.,
 * >$a
 * mary had a little lamb
 * >
 *
 * The client passes the strings input to a server, the IP address of
 * which is specified on the command line, e.g..
 * $ client_demo 127.0.0.1
 * The server stores the assigned values into an internal
 * data structure, which it makes persistent using a file.
 *
 * There's a certain amount of overhead in using stdio and
 * sockets, particularly buffer management with fflush() and so forth.
 */

#include "demo.h"

main( int argc, char *argv[] )
{
  // Define the receive buffer
  char recv_buf[BUFSIZE];

  // Define the send buffer
  char send_buf[BUFSIZE];

  // The replies sent by the server. These strings are just used for comparison
  char *replyGranted = "Access Granted\n";
  char *replyDenied  = "Access Denied. Terminating connection\n";

?  // The buffer size for 
  size_t max_buf = BUFSIZE;
 
  // Socket Stuff goes here
  int sockfd;
  struct sockaddr_in servaddr;

  // Create a file pointer to keep track of the file being accessed. 
  // Two separate pointers are made for the server_request and server_reply
  // See https://www.cs.bu.edu/teaching/c/file-io/intro/ for usage. 
  FILE *server_request, *server_reply;

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
      perror( "connect to associative memory at server" );
      exit( 100 );
    }

  /* setup the interfaces between the new socket and stdio system */
  // server_requet is anything you request from the server
  server_request = fdopen( sockfd, "w" );
  if( server_request == (FILE *) NULL )
    {
      perror( "fdopen of stream for server requests" );
      exit( 2 );
    }

  // server_reply is the reply of the server 
  server_reply = fdopen( sockfd, "r" );
  if( server_reply == (FILE *) NULL )
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
      if(fgets(recv_buf,BUFSIZE,server_reply)!=NULL) {
        fprintf(stdout,"Server: %s", recv_buf);
        if(strcmp(recv_buf,replyGranted)==0){
          //
          //
          //
          // This is the User interface showed to the client.
          // Add modifications of MAC id here
          //
          //
          printf("Add or update user by: user,password\n");
          printf("Delete user by: delete$user\n");
          break; // break out from while (1)
        
        } else if(strcmp(recv_buf,replyDenied)==0) {
          // request is denied => Close the server_request and the server_request and the socket
          // close the server_request, server_reply file descriptors if auth failed.
          fclose( server_request );
          fclose( server_reply );
          close( sockfd);
          // exit the program
          exit( 0 );
        }

        if (fgets( send_buf, BUFSIZE, stdin ) != NULL) {
          // Get input from the standard input and put it into send_buf,
          // put this as server request and send it to the server.
          fputs(send_buf,server_request);
          fflush(server_request);

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
      if( fputs( send_buf, server_request ) == EOF )
      {
        perror( "write failure to associative memory at server" );
      }

      // Sent the server_request to the server
      fflush( server_request );  /* buffering everywhere.... */

      // Get the server reply and put into recv_buf
      if( fgets( recv_buf, BUFSIZE, server_reply ) == NULL )
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
  fclose( server_request );
  fclose( server_reply );
  close( sockfd);

  exit( 0 );
}
