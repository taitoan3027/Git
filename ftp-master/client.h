

/*FTP Client*/

#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <limits.h>
#include <dirent.h>
#include <netdb.h>
#include <unistd.h>
#include <time.h>


/*for getting file size using stat()*/
#include <sys/stat.h>

/*for sendfile()*/
#include <sys/sendfile.h>

/*for O_RDONLY*/
#include <fcntl.h>

//typedef enum { false, true } Boolean;
#define PORT 21
#define N 1024

void FTP_SendResponse(char* ip, char* username, char* password, char* filepath, char* filename);
void replylogcode(int code);
void send226_transfer_complete(int socket);
void service_freeArgs(char* *ap_argv, const int a_argc);
char** service_parseArgs(const char* a_cmdStr, int *ap_argc);
void session_create(int sock, char * ip, char * user, char * pass);

void pwd(const int sock);
void lpwd();
void lcd(char * arg);
void cd(char *file, const int sock);
void quit(int sd);
void _rmdir(char *file,const int sd, char * ip);
void _mkdir(char *file,const int sd);
void delete(char *file, const int sd);
void dir_non_path(char* file,const int sd, char * ip);
void dir_path(char* file,const int sd, char * ip);
int pasv(int sd, char* ip);
void ls_non_path(char* file,const int sd, char * ip);
void ls_path(char* file,const int sd, char * ip);
void  rm_non_empty_dir(char* file,const int sd, char * ip);
void get(char *file, const int sd,char * ip);
void put(char *file, const int sd,char * ip);
void service_handleCmd(const int socket, char**ap_argv, const int a_argc, char * ip);
void service_create(int socket, char* ip);
