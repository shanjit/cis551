//
// Anything sent to the client is the client_rep
// Anything got from the client is the client_req
//

// Use of restore?
// find_colons/equals
//


#include "demo.h"

struct sym_list Head;	/* head of singly-linked list */

/*
 * Daemon provides an interactive associative memory
 * via a socket-based interface. Clients either set
 * passwords with an assignment statement or access
 * passwords with a $ preface. When the password is
 * accessed, we write it onto the client's socket.
 * We currently do this as an iterative server for
 * reasons of queuing and serialization. If the
 * server is made concurrent, the database will have
 * to have serialized access and copy control - this
 * is not necessary yet.
 *
 * Program notes:
 * Parsing is done
 * with find_dollar() and find_equals().
 * Storage management is carried out by the insert()
 * and lookup() routines.
 * save() and restore() routines added to
 * use disk storage to maintain memory across
 * invocations.
 * Iterative server code is copied from Stevens, "UNIX Network
 * Programming: Networking APIs: Sockets and XTI," p. 101
 *
 */

main( argc, argv, env )
	int argc;
	char *argv[], *env[];
{
  int server_fd, create_service(), connection_fd;
  void service(), save(), restore();
  socklen_t len;
  struct sockaddr_in cliaddr;
  char recv_buf[BUFSIZE];

  char name[USERNAMELEN], password[PASSWORDLEN];
  char *good = "Welcome to The Machine!\n";
  char *evil = "Invalid identity, exiting!\n";

  // returns the listen file descriptor
  server_fd = create_service();

  while( TRUE )
    {
      len = sizeof( cliaddr );
      // Wait for a connection to come in.
      connection_fd = accept( server_fd, (SA *) &cliaddr, &len );

      // After the connection is accepted. Check if the connection is ok.
      if( connection_fd < 0 ){
      fprintf(stderr, "Error: Socket Accept\n");
	     
	     exit( ERR_ACCEPT );
	    }

      /*The file "file" for the username and password is loaded 
      in RAM using RESTORE and SAVED before exiting. Thats what 
      those two functions do. */
	   restore( DATABASE );

     // Service any incoming request 
	   service(connection_fd,name,password, good,evil);

     // Save the data from the RAM onto the file before exiting.
	   save( DATABASE );

     // close the connection
	   close( connection_fd );

     printf("Connection Terminated\n");
     break;
    }

  close(server_fd);
}

sendToClient(char *msg, FILE *address)
{
  fputs(msg,address);
  fflush(address);
}

int recvFromClient(char *msg, FILE *address)
{
  if (fgets( msg, BUFSIZE, address ) !=NULL)
  {
    // CHECK FOR SHELL CODE HERE!


    return 1;
  }

  else {
    return 0;
  }
}

int doAuth(char *msg, FILE *address, char *name, char *password)
{
    fprintf(stdout, "Authenticating User\n");
        char *ptr, *ptr_lookup;
      char *lookup_res, *find_newline;

      fix_tcl( msg ); 

      // use find_colon to find out at which position colon occurs
      if( (ptr = find_colon(msg)) != (char *) NULL ) {

        ptr_lookup = find_colon(msg);
        *ptr = EOS;
        sscanf(msg,"%s",name);
        sscanf(++ptr,"%s",password);

        printf("Name: %s\n", name);
        printf("Password: %s\n", password);


       /* removes trailing newline if found */
       if( (find_newline = strrchr( ptr_lookup, NEWLINE )) != NULL )
         *find_newline = EOS;

        // lookup takes the username as the argument and returns the password as its argument.
         if( (lookup_res = lookup(name)) != NULL ) {
           if ( strcmp(lookup_res,password) == 0) {
            return 1;
           } else {
            return 0;
           }
         }
         else{
          return 0;
         }

      }
}

void
service( int fd, char *name, char *password, char *good, char *evil)
{
  FILE *client_req, *client_rep, *fdopen();
  char recv_buf[BUFSIZE];

  /* interface between socket and stdio */
  client_req = fdopen( fd, "r" );
  if( client_req == (FILE *) NULL )
    {
        fprintf(stderr, "Error: fdopen client_req\n");
      exit( 1 );
    }
  client_rep = fdopen( fd, "w" );
  if( client_rep == (FILE *) NULL )
    {
      fprintf(stderr, "Error: fdopen client_rep\n");
      exit( 1 );
    }

  printf("Connection Accepted\n");

  sendToClient("Enter username:password for authentication \n", client_rep);

  // Evaluate the first response from the client.
  while( recvFromClient(recv_buf, client_req) != 0 ){

      int authVar = doAuth(recv_buf, client_rep, name, password);
      if(authVar==1)
      {
        printf("%s",good);
        sendToClient(good, client_rep);
        printf("User Authenticated\n");
        break;
      }

      else
      {
        printf("%s",evil);
        sendToClient(evil, client_rep);
        // Exit the server and make sure the script restarts the server 
        printf("The script restarts the process\n");
        exit(99);
      }


    }

    while(1) {
      char *ptr;
      if( recvFromClient(recv_buf, client_req) != 0 ){

        /*Exit command*/
        if(!strcmp(recv_buf,"exit\n")){
          sendToClient("Exiting\n",client_rep);
          fflush(client_rep);
          printf("The script restarts the process\n");
          exit(99);
        }
        /*Add or update*/
        if( (ptr = find_colon( recv_buf )) != (char *) NULL ) {
          #ifdef EBUG
            fprintf( stderr, "ASSIGN: %s\n", recv_buf );
            dump( recv_buf );
          #endif
	        *ptr = EOS;
	        sscanf(recv_buf,"%s",name);
          sscanf(++ptr,"%s",password);

          printf("User Name to be saved: %s\n", name);
          printf("Associated Password: %s\n", password);

          insert( name, password );
          fputs( "User name and Password saved\n", client_rep );
	        fflush( client_rep );
          #ifdef EBUG
            fprintf( stderr, "REPLY: <>\n" );
          #endif
        }
        /*Delete User*/
        else if ((ptr = find_dollar(recv_buf))!=NULL)
        {
          *ptr = EOS;
          if(strcmp("delete",recv_buf)==0){
            char* returnstr;
            returnstr = delete_user(++ptr);
            fputs(returnstr,client_rep);
            fflush(client_rep);
          }
        }
        else {
          fputs( "Use proper format: <name,user> to add or update. <delete$user_name to delete>\n", client_rep );
          fflush( client_rep );
        }
      }
    }

  return;

}

int create_service()
{
  int listenfd;
  struct sockaddr_in servaddr;

  listenfd = socket(AF_INET, SOCK_STREAM, 0 );
  if( listenfd < 0 )
    {
      perror( "creating socket for listenfd" );
      exit( ERR_SOCKET );
    }

  bzero( &servaddr, sizeof(servaddr) );
  servaddr.sin_family = AF_INET;


  char servip[20] = "192.168.1.1";

  // Uncomment line below to associate server ipaddress with particular ipaddress 
  /* servaddr.sin_addr.s_addr = inet_addr(recv_buffer_ip);
  */

  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons( CIS551_PORT );

  inet_ntop(AF_INET, &servaddr.sin_addr, servip, 20);
  printf("Starting server on: %s %d. \n", servip, ntohs(servaddr.sin_port));

  if( bind( listenfd, (SA *) &servaddr, sizeof(servaddr) ) < 0 )
  {
    perror( "bind on listenfd");
    exit( ERR_BIND );
  }

  if( listen( listenfd, LISTENQ ) < 0 )
    {
      perror( "listen on listenfd" );
      exit( ERR_LISTEN );
    }

  return( listenfd );
}

fix_tcl( char *recv_buf )
{
  char *ptr;

#define CARRIAGE_RETURN '\r'
  if( (ptr = strrchr( recv_buf, CARRIAGE_RETURN )) != NULL )
    *ptr = EOS;
  return;

}

dump( char *recv_buf )
{
  fprintf( stderr, "strlen(recv_buf)=%d, recv_buf=<%s>\n", strlen(recv_buf), recv_buf );
  {
    int i;

    for( i=0; recv_buf[i] != EOS; i++ )
      fprintf( stderr, "%d:%c:%x\n", i, recv_buf[i], recv_buf[i] );
  }
}
