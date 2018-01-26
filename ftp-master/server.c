#include "common.h"
#include <memory.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "DCTag.h"
#include <stdlib.h> // For exit() function
#include <string.h>
#include <assert.h>
/** 
 * Sets up server and handles incoming connections
 * @param port Server port
 */
void server(int port)
{
  printf("Start server at port %d", port);
  int sock = create_socket(port);
  struct sockaddr_in client_address;
  int len = sizeof(client_address);
  int connection, pid, bytes_read;
 
  while(1){
    connection = accept(sock, (struct sockaddr*) &client_address,&len);
    char buffer[BSIZE];
    Command *cmd = malloc(sizeof(Command));
    State *state = malloc(sizeof(State));
    pid = fork();
    
    memset(buffer,0,BSIZE);

    if(pid<0){
      fprintf(stderr, "Cannot create child process.");
      exit(EXIT_FAILURE);
    }

    if(pid==0){
      close(sock);
      char welcome[BSIZE] = "220 ";
      if(strlen(welcome_message)<BSIZE-4){
        strcat(welcome,welcome_message);
      }else{
        strcat(welcome, "Welcome to nice FTP service.");
      }

      /* Write welcome message */
      strcat(welcome,"\n");
      write(connection, welcome,strlen(welcome));

      /* Read commands from client */
      while (bytes_read = read(connection,buffer,BSIZE)){
        
        signal(SIGCHLD,my_wait);

        if(!(bytes_read>BSIZE)){
          /* TODO: output this to log */
          buffer[BSIZE-1] = '\0';
          printf("User %s sent command: %s\n",(state->username==0)?"unknown":state->username,buffer);
          parse_command(buffer,cmd);
          state->connection = connection;
          
          /* Ignore non-ascii char. Ignores telnet command */
          if(buffer[0]<=127 || buffer[0]>=0){
            response(cmd,state);
          }
          memset(buffer,0,BSIZE);
          memset(cmd,0,sizeof(cmd));
        }else{
          /* Read error */
          perror("server:read");
        }
      }
      printf("Client disconnected.\n");
      exit(0);
    }else{
      printf("closing... :(\n");
      close(connection);
    }
  }
  
  
  
  
  
}

/**
 * Creates socket on specified port and starts listening to this socket
 * @param port Listen on this port
 * @return int File descriptor for new socket
 */
int create_socket(int port)
{
  int sock;
  int reuse = 1;

  /* Server addess */
  struct sockaddr_in server_address = (struct sockaddr_in){  
     AF_INET,
     htons(port),
     (struct in_addr){INADDR_ANY}
  };


  if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
    fprintf(stderr, "Cannot open socket");
    exit(EXIT_FAILURE);
  }

  /* Address can be reused instantly after program exits */
  setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof reuse);

  /* Bind socket to server address */
  if(bind(sock,(struct sockaddr*) &server_address, sizeof(server_address)) < 0){
    fprintf(stderr, "Cannot bind socket to address");
    exit(EXIT_FAILURE);
  }

  listen(sock,5);
  return sock;
}

/**
 * Accept connection from client
 * @param socket Server listens this
 * @return int File descriptor to accepted connection
 */
int accept_connection(int socket)
{
  int addrlen = 0;
  struct sockaddr_in client_address;
  addrlen = sizeof(client_address);
  return accept(socket,(struct sockaddr*) &client_address,&addrlen);
}

/**
 * Get ip where client connected to
 * @param sock Commander socket connection
 * @param ip Result ip array (length must be 4 or greater)
 * result IP array e.g. {127,0,0,1}
 */
void getip(int sock, int *ip)
{
  socklen_t addr_size = sizeof(struct sockaddr_in);
  struct sockaddr_in addr;
  getsockname(sock, (struct sockaddr *)&addr, &addr_size);
 
  char* host = inet_ntoa(addr.sin_addr);
  sscanf(host,"%d.%d.%d.%d",&ip[0],&ip[1],&ip[2],&ip[3]);
}

/**
 * Lookup enum value of string
 * @param cmd Command string 
 * @return Enum index if command found otherwise -1
 */

int lookup_cmd(char *cmd){
  const int cmdlist_count = sizeof(cmdlist_str)/sizeof(char *);
  return lookup(cmd, cmdlist_str, cmdlist_count);
}

/**
 * General lookup for string arrays
 * It is suitable for smaller arrays, for bigger ones trie is better
 * data structure for instance.
 * @param needle String to lookup
 * @param haystack Strign array
 * @param count Size of haystack
 */
int lookup(char *needle, const char **haystack, int count)
{
  int i;
  for(i=0;i<count; i++){
    if(strcmp(needle,haystack[i])==0)return i;
  }
  return -1;
}


/** 
 * Writes current state to client
 */
void write_state(State *state)
{
  write(state->connection, state->message, strlen(state->message));
}

/**
 * Generate random port for passive mode
 * @param state Client state
 */
void gen_port(Port *port)
{
  srand(time(NULL));
  port->p1 = 128 + (rand() % 64);
  port->p2 = rand() % 0xff;

}

/**
 * Parses FTP command string into struct
 * @param cmdstring Command string (from ftp client)
 * @param cmd Command struct
 */
void parse_command(char *cmdstring, Command *cmd)
{
  sscanf(cmdstring,"%s %s",cmd->command,cmd->arg);
}

/**
 * Handles zombies
 * @param signum Signal number
 */
void my_wait(int signum)
{
  int status;
  wait(&status);
}

char ReadConfig(void)
{
	char row[1000];
    FILE *fptr; // pointer to received file
	
    if ((fptr = fopen("server_config.txt", "r")) == NULL)
    {
        printf("Error! opening file \"server_config.txt\"");
        //// Program exits if file pointer returns NULL.
        return 0;    
    }
	else
	{
		printf("opened file \"server_config.txt\ successfully \r\n");
		//// reads server 8021 user/pass
		while (fgets( row, sizeof( row ), fptr ) != NULL ) 
		{ 
			char** frame_parts;
			frame_parts = str_split(row, ',');
			if(frame_parts)
			{					
				if (strcmp(*(frame_parts),"user") == 0)
				{
					printf("setting server username...");
					
					sprintf(client_user_name,"%s", *(frame_parts + 1));
					printf("Server username: %s___", client_user_name);
					
					printf("Done\r\n");
				}
				else if (strcmp(*(frame_parts),"pass") == 0)
				{
					printf("setting server password...");
					sprintf(client_pass_word,"%s", *(frame_parts + 1));
					printf("Server password: %s___", client_pass_word);
					
					printf("Done\r\n");
				}
				else if (strcmp(*(frame_parts),"fname") == 0)
				{
					printf("setting valid name for command file...");
					sprintf(cmd_filename,"%s_cmd.txt", *(frame_parts + 1));
					printf("valid CMD file name: %s___", cmd_filename);
					printf("setting valid name for response file...");
					sprintf(response_filename,"%s_response.txt", *(frame_parts + 1));
					printf("valid Response file name: %s___", response_filename);
					printf("Done\r\n");
				}
				
				free(frame_parts);
			}
		}
		
		fclose(fptr);
		printf("Configuration completed!");
		return 1;
	}
}


int main ( int argc, char * argv[] )
{
  if(ReadConfig())
    server(8021);
  //WriteTag("BoardIO:digital_out_0", "0.0");

	
  return 0;

}
