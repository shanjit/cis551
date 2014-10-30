//
// Anything sent to the client is the client_reply
// Anything got from the client is the client_request
//

// Use of restore?
// find_commas/equals
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

  char name[123], value[123];
  char *replyGranted = "Access Granted\n";
  char *replyDenied  = "Access Denied. Terminating connection\n";

  // returns the listen file descriptor
  server_fd = create_service();

  while( HELL_NOT_FROZEN )
    {
      len = sizeof( cliaddr );
      // Wait for a connection to come in.
      connection_fd = accept( server_fd, (SA *) &cliaddr, &len );

      // After the connection is accepted. Check if the connection is ok.
      if( connection_fd < 0 ){
	     perror( "accept on server_fd" );
	     exit( ERR_ACCEPT );
	    }

      /*The file "file" for the username and password is loaded 
      in RAM using RESTORE and SAVED before exiting. Thats what 
      those two functions do. */
	   restore( DATABASE );

     // Service any incoming request 
	   service(connection_fd,name,value, replyGranted,replyDenied);

     // Save the data from the RAM onto the file before exiting.
	   save( DATABASE );

     // close the connection
	   close( connection_fd );

     printf("Client disconnected\n\n");
     printf("Terminating server!\n");
     break;
    }

  close(server_fd);
}

void
service( int fd, char *name, char *value, char *replyGranted, char *replyDenied)
{
  FILE *client_request, *client_reply, *fdopen();
  char buf[BUFSIZE];

  /* interface between socket and stdio */
  client_request = fdopen( fd, "r" );
  if( client_request == (FILE *) NULL )
    {
      perror( "fdopen of client_request" );
      exit( 1 );
    }
  client_reply = fdopen( fd, "w" );
  if( client_reply == (FILE *) NULL )
    {
      perror( "fdopen of client_reply" );
      exit( 1 );
    }
  printf("Servicing Client\n");


  // The server is the first one to actually start talking to the client.

  fputs("Enter 'user,password' for Authentication\n",client_reply);
  fflush( client_reply );


  // Evaluate the first response from the client.
  while( fgets( buf, BUFSIZE, client_request ) != NULL ){
      char *ptr, *ptr_lookup;
      char *lookup_result, *find_newline;

      fix_tcl( buf ); /* hack to interface with tcl scripting language */

      // use find_comma to find out at which position comma occurs
      if( (ptr = find_comma(buf)) != (char *) NULL ) {

        ptr_lookup = find_comma(buf);
        *ptr = EOS;
        sscanf(buf,"%s",name);
        sscanf(++ptr,"%s",value);
        printf("Name entered: %s\n", name);
        printf("Password entered: %s\n", value);


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
            fputs(replyGranted,client_reply);
            fflush(client_reply);
            break;
           } else {
            /*Wrong password*/
            printf("Wrong User Password\n");
            fputs( replyDenied, client_reply );
            fflush( client_reply );
            return;
           }
         }
         else{
          /*Wrong name entered*/
          printf("Wrong User Name entered\n");
          fputs( replyDenied, client_reply );
          fflush( client_reply );
          return;
         }

      }
    }


    printf("Ready to service authenticated user\n");
    while(1) {
      // Service the user after authenticating him.
      char *ptr;

      // now when you get something from the client
      if( fgets( buf, BUFSIZE, client_request ) != NULL ){

        /*Exit command*/
        if(!strcmp(buf,"exit\n")){
          fputs("Exiting\n",client_reply);
          fflush(client_reply);
          return;
        }
        /*Add or update*/
        if( (ptr = find_comma( buf )) != (char *) NULL ) {
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
          fputs( "User name and Password saved\n", client_reply );
	        fflush( client_reply );
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
            fputs(returnstr,client_reply);
            fflush(client_reply);
          }
        }
        else {
          fputs( "Use proper format: <name,user> to add or update. <delete$user_name to delete>\n", client_reply );
          fflush( client_reply );
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
  servaddr.sin_addr.s_addr = htonl( INADDR_ANY );
  servaddr.sin_port = htons( CIS551_PORT );

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
