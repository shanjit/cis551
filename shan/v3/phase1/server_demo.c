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
 * values with an assignment statement or access
 * values with a $ preface. When the value is
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
  char buf[BUFSIZE];

  char name[USERNAMELEN], value[PASSWORDLEN];
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
	   service(connection_fd,name,value, good,evil);

     // Save the data from the RAM onto the file before exiting.
	   save( DATABASE );

     // close the connection
	   close( connection_fd );

     printf("Connection Terminated\n");
     break;
    }

  close(server_fd);
}

void
service( int fd, char *name, char *value, char *good, char *evil)
{
  FILE *client_req, *client_rep, *fdopen();
  char buf[BUFSIZE];

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


  // The server is the first one to actually start talking to the client.
  fputs("Enter username:password for authentication \n",client_rep);
  fflush( client_rep );

  // Evaluate the first response from the client.
  while( fgets( buf, BUFSIZE, client_req ) != NULL ){
      char *ptr, *ptr_lookup;
      char *lookup_result, *find_newline;

      fix_tcl( buf ); 

      // use find_colon to find out at which position comma occurs
      if( (ptr = find_colon(buf)) != (char *) NULL ) {

        ptr_lookup = find_colon(buf);
        *ptr = EOS;
        sscanf(buf,"%s",name);
        sscanf(++ptr,"%s",value);
        printf("Name: %s\n", name);
        printf("Password: %s\n", value);


       /* removes trailing newline if found */
       if( (find_newline = strrchr( ptr_lookup, NEWLINE )) != NULL )
         *find_newline = EOS;

        // lookup takes the username as the argument and returns the password as its argument.
         if( (lookup_result = lookup(name)) != NULL ) {
           /*Correct Name entered*/
           // lookup_result returns the password for the name passed as argument to lookup
           if ( strcmp(lookup_result,value) == 0) {
            /*Correct password entered*/
            /*Access Granted*/
            fputs(good,client_rep);
            fflush(client_rep);
            break;
           } else {
            /*Wrong password*/
            printf("Wrong User Password\n");
            fputs( evil, client_rep );
            fflush( client_rep );
            return;
           }
         }
         else{
          /*Wrong name entered*/
          printf("Wrong User Name entered\n");
          fputs( evil, client_rep );
          fflush( client_rep );
          return;
         }

      }
    }


    printf("Ready to service authenticated user\n");
    while(1) {
      // Service the user after authenticating him.
      char *ptr;

      // now when you get something from the client
      if( fgets( buf, BUFSIZE, client_req ) != NULL ){

        /*Exit command*/
        if(!strcmp(buf,"exit\n")){
          fputs("Exiting\n",client_rep);
          fflush(client_rep);
          return;
        }
        /*Add or update*/
        if( (ptr = find_colon( buf )) != (char *) NULL ) {
          #ifdef EBUG
            fprintf( stderr, "ASSIGN: %s\n", buf );
            dump( buf );
          #endif
	        *ptr = EOS;
	        sscanf(buf,"%s",name);
          sscanf(++ptr,"%s",value);

          printf("User Name to be saved: %s\n", name);
          printf("Associated Password: %s\n", value);

          insert( name, value );
          fputs( "User name and Password saved\n", client_rep );
	        fflush( client_rep );
          #ifdef EBUG
            fprintf( stderr, "REPLY: <>\n" );
          #endif
        }
        /*Delete User*/
        else if ((ptr = find_dollar(buf))!=NULL)
        {
          *ptr = EOS;
          if(strcmp("delete",buf)==0){
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
  /* servaddr.sin_addr.s_addr = inet_addr(buffer_ip);
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

fix_tcl( char *buf )
{
  char *ptr;

#define CARRIAGE_RETURN '\r'
  if( (ptr = strrchr( buf, CARRIAGE_RETURN )) != NULL )
    *ptr = EOS;
  return;

}

dump( char *buf )
{
  fprintf( stderr, "strlen(buf)=%d, buf=<%s>\n", strlen(buf), buf );
  {
    int i;

    for( i=0; buf[i] != EOS; i++ )
      fprintf( stderr, "%d:%c:%x\n", i, buf[i], buf[i] );
  }
}
