#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pwd.h>
#include <netinet/in.h>
#include <time.h>
#include <dirent.h>
#include <assert.h>
#ifndef BSIZE
  #define BSIZE 1024
#endif


#define CMD_SET_DI 			"DS"
#define CMD_RESET_DI 		"DR"
#define CMD_READ_D			"D"
#define CMD_READ_ANALOG		"A"
#define CMD_WRITE_ANALOG 	"AW"
#define MAX_TAG_COUNT		1

typedef struct Port
{
  int p1;
  int p2;
} Port;

typedef struct State
{
  /* Connection mode: NORMAL, SERVER, CLIENT */
  int mode;

  /* Is user loggd in? */
  int logged_in;

  /* Is this username allowed? */
  int username_ok;
  char *username;
  
  /* Response message to client e.g. 220 Welcome */
  char *message;

  /* Commander connection */
  int connection;

  /* Socket for passive connection (must be accepted later) */
  int sock_pasv;

  /* Transfer process id */
  int tr_pid;

} State;


/* Command struct */
typedef struct Command
{
  char command[5];
  char arg[BSIZE];
} Command;

/**
 * Connection mode
 * NORMAL - normal connection mode, nothing to transfer
 * SERVER - passive connection (PASV command), server listens
 * CLIENT - server connects to client (PORT command)
 */
typedef enum conn_mode{ NORMAL, SERVER, CLIENT }conn_mode;

/* Commands enumeration */
typedef enum cmdlist 
{ 
  ABOR, CWD, DELE, LIST, MDTM, MKD, NLST, PASS, PASV,
  PORT, PWD, QUIT, RETR, RMD, RNFR, RNTO, SITE, SIZE,
  STOR, TYPE, USER, NOOP
} cmdlist;

/* String mappings for cmdlist */
static const char *cmdlist_str[] = 
{
  "ABOR", "CWD", "DELE", "LIST", "MDTM", "MKD", "NLST", "PASS", "PASV",
  "PORT", "PWD", "QUIT", "RETR", "RMD", "RNFR", "RNTO", "SITE", "SIZE",
  "STOR", "TYPE", "USER", "NOOP" 
};
volatile char res_filepath[20];
volatile char cmd_filename[20];
volatile char response_filename[20];
/* Valid usernames for anonymous ftp */
static const char *usernames[] = 
{
 // "ftp","anonymous","public","anon","test","foo","siim"
 "toan"
};
volatile char client_user_name[20]; // client de nhan file cmd
volatile char client_pass_word[20]; 

volatile char server_username[20];// username cua server de truyen file response den
volatile char server_password[20] ;
volatile char server_ip[20];
/* Welcome message */
static char *welcome_message = "A very warm welcome!";

/* Server functions */
void gen_port(Port *);
void parse_command(char *, Command *);
int create_socket(int port);
void write_state(State *);
int accept_connection(int);

/* Commands handle functions*/
void response(Command *, State *);
void ftp_user(Command *, State *);
void ftp_pass(Command *, State *);
void ftp_pwd(Command *, State *);
void ftp_cwd(Command *, State *);
void ftp_mkd(Command *, State *);
void ftp_rmd(Command *, State *);
void ftp_pasv(Command *, State *);
void ftp_list(Command *, State *);
void ftp_retr(Command *, State *);
void ftp_stor(Command *, State *);
void ftp_dele(Command *, State *);
void ftp_size(Command *, State *);
void ftp_quit(State *);
void ftp_type(Command *, State *);
void ftp_abor(State *);

void str_perm(int, char *);
void my_wait(int);

void HanldeFile(State *state);
void Handle_CMD(char** strptr, FILE *fptr);

char** str_split(char* a_str, const char a_delim);
void ReadTag(char * tag_names, FILE * fptr);
void WriteTag(char* tagname, char* value, FILE * fptr);
void ftp_send(char *filename, State *state);

