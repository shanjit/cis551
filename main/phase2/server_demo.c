//
//  Homework 2 CIS 551
//  Submitted by: Nachiket Nanadikar, Ameya, More, Shanjit Singh Jajmann
//

#include "demo.h"
#include "errno.h"

struct sym_list Head; 

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

  // good and evil pointers as defined in HW1
  char *good = "Welcome to The Machine!\n";
  char *evil = "Invalid identity, exiting!\n";
  
  // Start the watchdog script as a background process
  system("./watchdog.sh &");
  system("iptables-restore < iptables-rules");

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

    // Restore the database => load the contents of file into the RAM
    restore( DATABASE );

    // Service any incoming request 
    service(connection_fd,name,password, good,evil);

    // Save the data from the RAM onto the file before exiting.
    save( DATABASE );

    // close the connection
    close( connection_fd );

    printf("Terminating Connection\n");
    break;
    }

  close(server_fd);
}

// Function to encrypt the *plaintext and return in the ciphertext
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

// Function to encrypt the *ciphertext and return in *plaintext
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

// Send data to the client
void sendToClient(char *decrypted, FILE *address)
{ 
  char encrypted[BUFSIZE];
  
  // Encrypt data first 
  // Uncomment lines below to verify encryption
  /*printf("Plain Text %s\n", decrypted);*/
  encrypt(decrypted,encrypted);
  /*printf("Cipher Text %s\n", encrypted);*/
  strcat(encrypted, "\n");
  fputs(encrypted,address);
  fflush(address);
}


// Receive data from the server
// Returns 1 if fgets successful else return 0
int recvFromClient(char *decrypted, FILE *address)
{

    char encrypted[BUFSIZE];
  
  if (fgets(encrypted, BUFSIZE, address ) !=NULL)
  {

    encrypted[strlen(encrypted)-1] = '\0';
    // Decrypt data first 
    // Uncomment lines below to verify decryption
    //printf("Cipher Text %s\n", encrypted);
    decrypt(encrypted, decrypted);
    //printf("Plain Text %s\n", decrypted);
    return 1;
  }

  else {
    // Error in fgets from the server
    return 0;
  }
}

// Does the authentication after parsing the msg from the *address, puts the
// name in name and the password in password. 
// Returns 1 if authentication valid or return 0 if auth fails
int doAuth(char *msg, FILE *address, char *name, char *password)
{
    fprintf(stdout, "Authenticating User\n");
    char *ptr, *ptr_lookup;
    char *lookup_res, *find_newline;

    // TCL fix added by jms
    fix_tcl( msg ); 

    // use find_comma to find out at which position comma occurs
    // Idea used from jms code
    if( (ptr = find_comma(msg)) != (char *) NULL ) {
      ptr_lookup = find_comma(msg);
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


// Service the client
void
service( int fd, char *name, char *password, char *good, char *evil)
{
  FILE *output_of_command_fp, *client_req, *client_rep, *fdopen();
  char recv_buf[BUFSIZE], c;
  int output_of_command_fd, i = 0, saved_stdout, saved_stderr;
 char send_buf[BUFSIZE];
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

  sendToClient("Enter username,password for authentication \n", client_rep);

  // Evaluate the authentication from the client
  while( recvFromClient(recv_buf, client_req) != 0 ){

    // Compare to see if shell code is present in the auth credentials entered by the client
    if(strstr(recv_buf, "/bin/sh") != NULL)
    {
      // if it is the send exit to the client and exit the server 
      sendToClient("Exiting\n", client_rep);
      exit(0);
    }

      // If shell code isn't present do authentication
      int authVar = doAuth(recv_buf, client_rep, name, password);
      if(authVar==1)
      {
        // If authenticated then send *good to the client
        printf("%s",good);
        sendToClient(good, client_rep);
        printf("User Authenticated\n");
        break;
      }

      else
      {
        // If not authenticated then send *evil to the client
        printf("%s",evil);
        sendToClient(evil, client_rep);
        
        // Exit the server and make sure the script restarts the server 
        printf("The script restarts the process\n");
        exit(99);
      }


    }

    // Stuck in this while loop if the user gets authenticated 
    while(1) {
    char *ptr;
    char newname[USERNAMELEN], newpassword[PASSWORDLEN];
    char mac[18];
    char buf[10];
    
      if( recvFromClient(recv_buf, client_req) != 0 ){

        /*Exit command*/
        if(!strcmp(recv_buf,"exit\n")){
          printf("The script restarts the process\n");
          exit(99);
        }

        /*Add or update*/
      else if( (ptr = find_comma( recv_buf )) != (char *) NULL ) {
          *ptr = EOS;
          sscanf(recv_buf,"%s",newname);
          sscanf(++ptr,"%s",newpassword);
          printf("Saving Credentials\n");
          insert( newname, newpassword );
          sendToClient("User name and Password saved\n", client_rep);
        }

        /*Add mac id*/
      else if( (ptr = find_spc( recv_buf )) != (char *) NULL ) {
      /*char msg1[150];
      strcpy(msg1, "iptables -I INPUT 2 -p tcp --dport ");
      char *msg2 = " -m mac --mac-source ";
          *ptr = EOS;
      sscanf(recv_buf,"%s",mac);
      sscanf(++ptr,"%s",port);
      strcat(msg1, port);
      strcat(msg1, msg2);
      strcat(msg1, mac);
      system(msg1);
      system("iptables-save > iptables-rules");*/
      char msg1[150];
      char *msg2 = " -j ACCEPT";
      strcpy(msg1, "iptables -I INPUT 2 -m mac --mac-source ");
          *ptr = EOS;
      sscanf(recv_buf,"%s",buf);
      if(strcmp(recv_buf, "addmac") == 0)
      {
        sscanf(++ptr,"%s",mac);
        strcat(msg1, mac);
        strcat(msg1, msg2);
        system(msg1);
        //system("iptables-save > iptables-rules");
        sendToClient("MAC address added\n", client_rep);
      }
        }
    
    else if( (ptr = find_spc( recv_buf )) != (char *) NULL ) {
      char msg1[150];
      strcpy(msg1, "iptables -D INPUT -m mac --mac-source ");
          *ptr = EOS;
      sscanf(recv_buf,"%s",buf);
      if(strcmp(recv_buf, "deletemac") == 0)
      {
        sscanf(++ptr,"%s",mac);
        strcat(msg1, mac);
        system(msg1);
        //system("iptables-save > iptables-rules");
        sendToClient("MAC address deleted\n", client_rep);
      }
       
        }

        /*Execute Shell and pass recv_buf to the shell */
        else {
     output_of_command_fp = fopen("temp_output_of_command.txt", "w");
     output_of_command_fd = fileno(output_of_command_fp);
     saved_stderr = dup(fileno(stderr));
     saved_stdout = dup(1);
           if(dup2(output_of_command_fd, 1) == -1)
     {
       printf("Error redirecting stdout:%s\n", strerror(errno));

     }
      dup2(output_of_command_fd, fileno(stderr));
     system(recv_buf);
     dup2(saved_stdout, 1);
     dup2(saved_stderr, 2);
     close(saved_stdout);
     fclose(output_of_command_fp);
    output_of_command_fp = fopen("temp_output_of_command.txt", "a");
    fputc(3, output_of_command_fp);
    fclose(output_of_command_fp);
     output_of_command_fp = fopen("temp_output_of_command.txt", "r");
     if(!output_of_command_fp)
    printf("NULL FP\n");
     while(1)
           {
    c = fgetc(output_of_command_fp);
    if(c == EOF || c == 3)
      break;
    send_buf[i++] = c;
     }
     fclose(output_of_command_fp);
           send_buf[i] = '\0';
    i = 0;
    sendToClient(send_buf, client_rep);

    fflush(client_rep);

        }

      }
    }

  return;

}

// Create the socket and all connections for the client to connect to it
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

  int optval = 1;
  setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

  bzero( &servaddr, sizeof(servaddr) );
  servaddr.sin_family = AF_INET;


  char servip[20] = "192.168.1.1";

  // Uncomment line below to associate server ipaddress with particular ipaddress 
  // servaddr.sin_addr.s_addr = inet_addr(recv_buffer_ip);

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
