
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
#include <dirent.h>
#include <assert.h>

/*for getting file size using stat()*/
#include <sys/stat.h>

/*for sendfile()*/
#include <sys/sendfile.h>

/*for O_RDONLY*/
#include <fcntl.h>



//#define MAX_TAG_COUNT					100


typedef enum { false, true } Boolean;
#define PORT 21
#define N 1024


void replylogcode(int code);
void send226_transfer_complete(int socket);
void service_freeArgs(char* *ap_argv, const int a_argc);
char** service_parseArgs(const char* a_cmdStr, int *ap_argc);
char session_create(int sock, char * ip, char * user, char * pass);
//void session_create(int sock, char * ip);
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
char put(char *file, const int sd,char * ip);
void service_handleCmd(const int socket, char**ap_argv, const int a_argc, char * ip);
void service_create(int socket, char* ip);
char ReadConfig(char* ip, char* username, char* password, char* filename);
char** str_split(char* a_str, const char a_delim);
void InsertWaitingList(char* fpath, char* fname);
void RefillFile();

/////////////// main ///////////////////////////////
char ip[30];
char username[30];
char password[30];
char filename[30];
	
int main() // <ip> <us> <pass> <province_s_s>
{
	//rename newest file

	
	if(ReadConfig(ip, username, password, filename ))
	{
		time_t rawtime;
		time ( &rawtime );
		struct tm *info;

		info = localtime( &rawtime );
		char newfilename[100];
		char newfilepath[100];
		char oldfilepath[100];
		char filepath[50];
		sprintf(filepath,"%04d_%02d_%02d/", (info->tm_year)+1900, (info->tm_mon)+1, info->tm_mday);
		
		sprintf(oldfilepath,"%stemp.txt",filepath);
		sprintf(newfilename,"%s_%04d%02d%02d%02d%02d%02d.txt",filename,(info->tm_year) + 1900, (info->tm_mon) + 1,info->tm_mday,info->tm_hour,info->tm_min,info->tm_sec);
		sprintf(newfilepath,"%s%s",filepath, newfilename);
		
		if(rename(oldfilepath, newfilepath) == 0)
		{
			printf("%s has been rename %s.\n", oldfilepath, newfilepath);
		}
		else
		{
			fprintf(stderr, "Error renaming %s.\n", oldfilepath);
			return 0;
		}
		


		printf("Connecting...\n");
		struct sockaddr_in *remote;
		struct stat obj;
		int sock;
		int  tmpres;
		//create socket va connect to server ftp ///////////////////////////
		
		sock = socket(AF_INET, SOCK_STREAM, 0);
		if(sock == -1)
		{
			printf("socket creation failed");
			InsertWaitingList(filepath, newfilename);
			exit(1);
		}

		
		remote = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in *));
		remote->sin_family = AF_INET;
		
		tmpres = inet_pton(AF_INET, ip, (void *)(&(remote->sin_addr.s_addr)));
		if( tmpres < 0)  
		{
			perror("Can't set remote->sin_addr.s_addr\n");
			InsertWaitingList(filepath, newfilename);
			exit(1);
		}else if(tmpres == 0)
		{
			fprintf(stderr, "%s is not a valid IP address\n", ip);
			InsertWaitingList(filepath, newfilename);
			exit(1);
		}
		remote->sin_port = htons(PORT);
		
		tmpres = connect(sock,(struct sockaddr*)remote, sizeof(struct sockaddr));
		if(tmpres == -1)
		{
			printf("Connect Error\n");
			InsertWaitingList(filepath, newfilename);
			exit(1);
		}
		printf("connect ok");
		// create session user , password /////////////////////

		if(!session_create(sock, ip, username, password))
		{
			InsertWaitingList(filepath, newfilename);
			exit(1);
		}
		else
		{
			//send current file //////////////////////////////////////////
			lcd(filepath);
			put(newfilename, sock, ip);
			lcd("..");
			close(sock);
			//refill
			RefillFile();
			printf("complete");			
		}
			
		//while(1);
		return 0;
	}
	else
	{
		
	}
}

void replylogcode(int code)
{
	switch(code){
		case 200:
		printf("Command okay");
		break;
		case 500:
		printf("Syntax error, command unrecognized.");
		printf("This may include errors such as command line too long.");
		break;
		case 501:
		printf("Syntax error in parameters or arguments.");
		break;
		case 202:
		printf("Command not implemented, superfluous at this site.");
		break;
		case 502:
		printf("Command not implemented.");
		break;
		case 503:
		printf("Bad sequence of commands.");
		break;
		case 150:
		printf("150 Accepted data connection");
		break;
		case 530:
		printf("Not logged in.");
		break;
	}
	printf("\n");
}

void send226_transfer_complete(int socket)
{
	char buff[N];
	memset(buff, '\0', N*sizeof(char));
	int tmpres;
	int codeftp;
	char * str=NULL;
	while((tmpres = recv(socket, buff, BUFSIZ, 0)) > 0)
	{
		sscanf(buff,"%d", &codeftp);
		printf("%s", buff);
		if(codeftp != 226) 
		{
			replylogcode(codeftp);
			break;
		}

		
		str = strstr(buff,"226");
		if(str != NULL){
			break;
		}
		memset(buff, 0, tmpres);
	}
}

void service_freeArgs(char* *ap_argv, const int a_argc)
{
	// check args
	if(ap_argv == NULL || a_argc <= 0)
		return;
	
	int i;
	
	// free array items
	for(i = 0; i<a_argc; i++)
	{
		if(ap_argv[i] != NULL)
			free(ap_argv[i]);
	}
	
	free(ap_argv);
}


char** service_parseArgs(const char* a_cmdStr, int *ap_argc)
{
	char* buf; 
	char**p_args; 
	char*arg;
	int i;
	
	// init vars
	if((buf = calloc(strlen(a_cmdStr)+1, sizeof(char))) == NULL)
		return NULL;

	strcpy(buf, a_cmdStr);
	
	// parse words
	for(p_args = NULL, i=0, arg = strtok(buf, " \t"); arg != NULL; i++, arg = strtok(NULL, " \t"))
	{
			// expand array
		if((p_args = realloc(p_args, (i+1) * sizeof(char*))) == NULL)
		{
			free(buf);
			return NULL;
		}

			// expand item
		if((p_args[i] = calloc(strlen(arg)+1, sizeof(char))) == NULL)
		{
			service_freeArgs(p_args, i);
			free(buf);

			return NULL;
		}

			// store item
		strcpy(p_args[i], arg);
	}


	*ap_argc = i;



	// clean up
	free(buf);

	return p_args;
}



char session_create(int sock, char * ip, char * user, char * pass)
{
	/*
	Connection Establishment
	   120
		  220
	   220
	   421
	Login
	   USER
		  230
		  530
		  500, 501, 421
		  331, 332
	   PASS
		  230
		  202
		  530
		  500, 501, 503, 421
		  332
	*/
		  int tmpres;
		  char buf[BUFSIZ+1];
		  char * str;
		  int codeftp;
		  printf("Connection established, waiting for welcome message...\n");


		  while((tmpres = recv(sock, buf, BUFSIZ, 0)) > 0)
		  {
		  	sscanf(buf,"%d", &codeftp);
		  	printf("%s", buf);
				if(codeftp != 220) //120, 240, 421: something wrong
				{
					replylogcode(codeftp);
					printf("Not 220...\n");
					return 0;
					//exit(1);
				}
				printf("220...\n");
				//str = strstr(buf, "220 \r\n");
				str = strstr(buf, "220");
				if(str != NULL){
					break;
				}
				memset(buf, 0, tmpres);
			}

	//Send Username ///////////////////////////////////////////////////////////
			char info[50];
			printf("Name (%s): ", ip);
			memset(buf, 0, sizeof buf);
			//scanf("%s", info);

			sprintf(buf,"USER %s\r\n",user);
			//sprintf(buf,"USER toan\r\n");
			tmpres = send(sock, buf, strlen(buf), 0);

			memset(buf, 0, sizeof buf);
			tmpres = recv(sock, buf, BUFSIZ, 0);

			sscanf(buf,"%d", &codeftp);
			if(codeftp != 331)
			{
				replylogcode(codeftp);
				return 0;
				//exit(1);
			}
			printf("%s", buf);

	//Send Password /////////////////////////////////////////////////////////////
			memset(info, 0, sizeof info);
			printf("Password: ");
			memset(buf, 0, sizeof buf);
			//scanf("%s", info);

			sprintf(buf,"PASS %s\r\n",pass);
			//sprintf(buf,"PASS tn\r\n");
			tmpres = send(sock, buf, strlen(buf), 0);

			memset(buf, 0, sizeof buf);
			tmpres = recv(sock, buf, BUFSIZ, 0);

			sscanf(buf,"%d", &codeftp);
			if(codeftp != 230)
			{
				replylogcode(codeftp);
				return 0;
				//exit(1);
			}
			printf("%s", buf);

			memset(buf, 0, sizeof buf);
			sprintf(buf,"SYST %s\r\n",info);
			tmpres = send(sock, buf, strlen(buf), 0);
//
		while((recv(sock, buf, BUFSIZ, 0)) > 0)
		  {
		  	sscanf(buf,"%d", &codeftp);
		  	printf("%s", buf);
			if(codeftp != 215) 
			{
				replylogcode(codeftp);
				printf("Not 215...\n");
				return 0;
				//exit(1);
			}
			printf("%s", buf);
			//str = strstr(buf, "220 \r\n");
			str = strstr(buf, "215");
			if(str != NULL)
			{
				break;
			}
			memset(buf, 0, sizeof buf);
		 }
	return 1;
}

void pwd(const int sock)
{
	char buff[N];
	char *pwd =NULL;
	pwd = "PWD \r\n";

	if( send(sock, pwd, strlen(pwd), 0) < 0 ){

		perror("Error: can't send");
		exit(2);
	}
	memset(buff, '\0', N*sizeof(char));


	if( recv(sock, buff, sizeof(buff), 0 ) < 0){

		perror("Error: can't recv");
		exit(5);
	}
	else 
	{
		int codeftp;
		printf("%s",buff);
		sscanf(buff,"%d", &codeftp);
		if(codeftp != 257)
		{
			replylogcode(codeftp);
			exit(1);
		}
	}



}


void lpwd()
{
	char buf[100];
	memset(buf, '\0', 100*sizeof(char));
	getcwd(buf,100);
	printf("%s \n",buf);
}
void lcd(char * arg)
{
	char buf[N];


	if (chdir(arg) == -1)
	{ 
		     	/* change cwd to path */   
		fprintf(stderr, "error: could not change to dir %s\n", arg);
		return;
	}

	getcwd(buf, N);
		    printf("Local current directory: %s\n", buf); /* print cwd as obtained from getcwd() */
}

void cd(char *file, const int sock)
{

	int tmpres;
	int codeftp;
	char buf[BUFSIZ+1];



	sprintf(buf,"CWD %s\r\n",file);
	tmpres = send(sock, buf, strlen(buf), 0);

	memset(buf, 0, sizeof buf);
	tmpres = recv(sock, buf, BUFSIZ, 0);

	printf("%s", buf);

}
void quit(int sd)
{

	char quit[N];
	char buff[N];

	memset(quit, '\0', N*sizeof(char));
	strcat(quit,"QUIT\r\n");


	if( send(sd, quit, strlen(quit)+1, 0) < 0 ){

		perror("send");
		exit(2);
	}

	memset(buff, '\0', N*sizeof(char));

	if( recv(sd, buff, sizeof(buff), 0 ) < 0){

		perror("Error: can't recv");
		exit(5);
	}
	else 
		printf("%s",buff);

	exit(1);

	close(sd);
	sd=0;

}


void _rmdir(char *file,const int sd, char * ip)
{

	char rmd[N];
	char buff[N];

	memset(rmd, '\0', N*sizeof(char));
	strcat(rmd,"RMD ");
	strcat(rmd, file);
	strcat(rmd,"\r\n");

		//printf("\n%s\n", file);

	if( send(sd, rmd, strlen(rmd), 0) < 0 ){

		perror("send");
		exit(2);

	}

	memset(buff, '\0', N*sizeof(char));
	if( recv(sd, buff, sizeof(buff), 0 ) < 0){

		perror("recv");
		exit(5);
	}else 
	printf("%s", buff);



}

void _mkdir(char *file,const int sd){

	char mkd[N];
	char buff[N];

	memset(mkd, '\0', N*sizeof(char));
	strcat(mkd,"MKD ");
	strcat(mkd, file);
	strcat(mkd,"\r\n");

	if( send(sd, mkd, strlen(mkd), 0) < 0 ){

		perror("error: cannot send");
		exit(2);
	}

	memset(buff, '\0', N*sizeof(char));
	if( recv(sd, buff, sizeof(buff), 0 ) < 0){

		perror("error: cannot recv");
		exit(5);
	}
	else printf("%s",buff);
}

void delete(char *file, const int sd){

	char dele[N]; 
	char buff[N];

	memset(dele, '\0', N*sizeof(char));
	strcat(dele,"DELE ");
	strcat(dele, file);
	strcat(dele,"\r\n");

	if( send(sd, dele, strlen(dele), 0) < 0 ){

		perror("error: cannot send");
		exit(2);

	}

	memset(buff, '\0', N*sizeof(char));
	if( recv(sd, buff, sizeof(buff), 0 ) < 0){

		perror("error: cannot recv");
		exit(5);
	}
	else printf("%s",buff);


}



void dir_non_path(char* file,const int sd, char * ip)
{

	char dir[BUFSIZ +1];
	char buff[BUFSIZ +1];


	int sockfd;




	memset(dir, 0, sizeof dir);
	sprintf(dir,"LIST\r\n");


	sockfd = pasv(sd,ip);

	if( sockfd ==0 )
	{
		printf("[!] Cannot connect mode passive\n");
		return;
	}



	//LIST
	if( send(sd, dir, strlen(dir), 0) < 0 ){

		perror("error: cannot send");
		exit(2);

	}



	memset(buff,0, sizeof buff);
	recv(sd, buff, BUFSIZ, 0 );
	printf("%s", buff);


	//Nhan du lieu tu server luong connect data
	int len;
	do{

		memset(buff, 0, sizeof buff);
		len = recv(sockfd, buff, BUFSIZ, 0 ); 
		printf("%s",buff);
	}while(len>0);


	// nhan 226 ok thanh cong
	send226_transfer_complete(sd);

	close(sockfd); //Dong socket

	return;

	
}

void dir_path(char* file,const int sd, char * ip)
{

	char dir[BUFSIZ +1];
	char buff[BUFSIZ +1];
	
	
	int sockfd;


	memset(dir,0, sizeof dir);
	sprintf(dir,"LIST %s\r\n",file);
	


	sockfd = pasv(sd, ip);

	if( sockfd ==0 )
	{
		printf("[!] Cannot connect mode passive\n");
		return;
	}
	
	

	//LIST
	if( send(sd, dir, strlen(dir), 0) < 0 ){

		perror("error: cannot send");
		exit(2);

	}



	memset(buff,0, sizeof buff);
	recv(sd, buff, BUFSIZ, 0 );
	printf("%s", buff);


	//Nhan du lieu tu server luong connect data
	int len;
	do{
		
		memset(buff, 0, sizeof buff);
		len = recv(sockfd, buff, BUFSIZ, 0 ); 
		printf("%s",buff);
	}while(len>0);



	// nhan 226 ok thanh cong
	send226_transfer_complete(sd);

	close(sockfd); //Dong socket

	return;

	
}


int pasv(int sd, char* ip)
{

	char buff[N] ;
	char pasv[N];
	
	int sockfd = 0;
    struct sockaddr_in serv_addr; //Cau truc chua dia chi server ma client can biet de ket noi toi


    memset(pasv, 0, N*sizeof(char));
    sprintf(pasv,"PASV\r\n");

	//PASV
    if( send(sd, pasv, strlen(pasv), 0) < 0 ){

    	perror("error: can not send");
    	exit(2);

    }
    memset(buff, '\0', N*sizeof(char));
    
    if(recv(sd, buff, N*sizeof(char) , 0 ) < 0)
    {
    	perror("error: can not recv");
    	exit(2);

    }
    else
    	printf("%s",buff);


	// tachschuoi nhan duoc de lay port p1,p2

    char * t = "(";
    char * t1 = ",";
    char * t2 = ")";
    char* p=NULL;
    p = strtok(buff,t);

    int j;
    j=0;
    int port1 =0, port2 =0, portno = 0;
    while ( p != NULL)
    {

    	p = strtok(NULL,t1);
    	if( j == 4)
    	{
    		if( p != NULL)
    			port1 = atoi(p);

    		break;
    	}
    	j++;
    }
    p = strtok(NULL,t2);
    if( p != NULL)
    {
    	port2 = atoi(p);

    }

    portno = (port1 * 256) + port2 ;
    

	//Tao socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
    	error("ERROR opening socket");

    
    memset(&serv_addr, '0', sizeof(serv_addr));
    //struct sockaddr_in serv_addr; //Cau truc chua dia chi server ma client can biet de ket noi toi


    //Thiet lap dia chi cua server de ket noi den
    serv_addr.sin_family = AF_INET;        //Mac dinh
    serv_addr.sin_port = htons(portno);    //Cong dich vu

    //Dia chi ip/domain may chu
    if(inet_pton(AF_INET, ip, &serv_addr.sin_addr)<=0)
    {
    	printf("\n inet_pton error occured\n");
    	exit(1);
    }

    //Goi ham connect de thuc hien mot ket noi den server
    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof( struct sockaddr)) < 0)
    {
    	printf("\n[!]Fail connect to server \n");
    	close(sockfd);
    	return;
    }

    t=NULL;
    t1 = NULL;
    t2 = NULL;
    p = NULL;
	printf("Passive ok \r\n");
    return sockfd;

}

void ls_non_path(char* file,const int sd, char * ip)
{

	char retr[N];
	char ls[N];
	char buff[N];
	int codeftp;
	int sockfd;
	char *str;
	memset(ls, '\0', N*sizeof(char));
	//strcat(ls,"NLST\r\n");
        
        sprintf(ls,"PORT 192,168,100,222,128,164\r\n");
	send(sd, ls, strlen(ls), 0);

	
	// cho o che do PASSIVE
	//sockfd =  pasv(sd,ip);



	//RETR 

	while((recv(sd, buff, N*sizeof(char), 0)) > 0)
		  {
		  	sscanf(buff,"%d", &codeftp);
		  	printf("%s", buff);
			if(codeftp != 200) 
			{
				replylogcode(codeftp);
				printf("Not 200...\n");
				exit(1);
			}
			//printf("%s", buff);
			//str = strstr(buf, "220 \r\n");
			str = strstr(buff, "200");
			if(str != NULL)
			{
				break;
			}
			memset(buff, 0, sizeof buff);
		 }



	//if( send(sd, ls, strlen(ls), 0) < 0 ){

	//	perror("send");
	//	exit(2);
//
	//}

	//memset(buff, '\0', N*sizeof(char));
	//if( recv(sd, buff, N*sizeof(char), 0 ) < 0){

	//	perror("recv");
	//	exit(5);
	//}
	//else printf("%s\n",buff);
       struct sockaddr_in serv_addr; //Cau truc chua dia chi server ma client can biet de ket noi toi


      //Thiet lap dia chi cua server de ket noi den
        serv_addr.sin_family = AF_INET;        //Mac dinh
        serv_addr.sin_port = htons(32932);    //Cong dich vu
	
        memset(ls, '\0', N*sizeof(char));   
        sprintf(ls,"LIST\r\n");
	send(sd, ls, strlen(ls), 0);

	while((recv(sd, buff, N*sizeof(char), 0)) > 0)
	{
		str = strstr(buff, "\r\n");
			if(str != NULL)
			{
				printf("%s", buff);
				//break;
			}
			memset(buff, 0, sizeof buff);  	
	 }
	
	//int len;
	//Nhan du lieu tu server
	//memset(buff, '\0', N*sizeof(char));
	//len = recv(sd, buff, N*sizeof(char), 0 ); 
	//printf("%s\n",buff);


		// nhan 226 ok thanh cong
	//send226_transfer_complete(sd);


	//close(sockfd); //Dong socket
}

void ls_path(char* file,const int sd, char * ip)
{

	char retr[N];
	char ls[N];
	char buff[N];
	
	int sockfd;
	
	memset(ls, '\0', N*sizeof(char));
	sprintf(ls,"NLST %s\r\n",file);

	// cho o che do PASSIVE
	sockfd =  pasv(sd,ip);
	
	

	//RETR 
	if( send(sd, ls, strlen(ls), 0) < 0 ){

		perror("send");
		exit(2);

	}
	
	memset(buff, '\0', N*sizeof(char));
	if( recv(sd, buff, N*sizeof(char), 0 ) < 0){

		perror("recv");
		exit(5);
	}
	else printf("%s\n",buff);


	int len;
	//Nhan du lieu tu server
	memset(buff, '\0', N*sizeof(char));
	len = recv(sockfd, buff, N*sizeof(char), 0 ); 
	printf("%s\n",buff);


		// nhan 226 ok thanh cong
	send226_transfer_complete(sd);


	close(sockfd); //Dong socket
}


void  rm_non_empty_dir(char* file,const int sd, char * ip)
{

	char retr[N];
	char list[N];

	char buff[N];
	char buff1[N];
	char* toke;
	char  path[25];



	// lay thu muc lam viec hien hanh


	char *pwd =NULL;
	pwd = "PWD\r\n";

	if( send(sd, pwd, strlen(pwd), 0) < 0 ){

		perror("Error: can't send");
		exit(2);
	}

	memset(buff, '\0', N*sizeof(char));


	if( recv(sd, buff, sizeof(buff), 0 ) < 0){

		perror("Error: can't recv");
		exit(5);
	}
	else 
	{
		int codeftp;
		printf("%s",buff);
		sscanf(buff,"%d", &codeftp);
		if(codeftp == 257)
		{
			char *x ="\"";

			toke = strtok(buff,x);
			toke = strtok(NULL,x);


		}
	}
	memset(path, '\0', 25*sizeof(char));
	strcpy(path,toke);
	printf("%s\n",path);




	

		/////////


	int sockfd;
	// cho o che do PASSIVE
	sockfd =  pasv(sd,ip);


	memset(list, '\0', N*sizeof(char));
	sprintf(list,"LIST %s\r\n",file);

	
	
	

	
	if( send(sd, list, strlen(list), 0) < 0 ){

		perror("send");
		exit(2);

	}


	
	memset(buff, '\0', N*sizeof(char));
	if( recv(sd, buff, sizeof(buff), 0 ) < 0){

		perror("recv");
		exit(5);
	}
	else
		printf("%s",buff);
	



	int len;
	//Nhan du lieu tu server
	memset(buff, '\0', N*sizeof(char));
	len = recv(sockfd, buff, sizeof(buff), 0 ); 
	printf("%s",buff);

	// nhan 226 thanh cong 
	send226_transfer_complete( sd);
    //printf("%s\n",buff);
	strcpy(buff1, buff);


	cd(file, sd);


    // xu ly chuoi// lay tung dong
	char *temp[N];
	char * t ="\n";
	char * token =strtok(buff,t);
	int i ;
	i = 0;
	while(token != NULL)
	{
		temp[i]=token;
		token =strtok(NULL,t);

		i++;

	}


	char bien1[100];
	char bien2[100];

	int h;

	for(h =0; h < i; h++)
	{
		
		if(temp[h][0]== '-')
		{
				// la tap tin
			memset(bien1, '\0', 100*sizeof(char));
			sscanf(temp[h],"%*s %*s %*s %*s %*s %*s %*s %*s %s",bien1);  // lay ten tap tin 
			delete(bien1,sd);
			
			
		}
		else if(temp[h][0]== 'd')
		{
			// la thu muc
			memset(bien2, '\0', 100*sizeof(char));
			sscanf(temp[h],"%*s %*s %*s %*s %*s %*s %*s %*s %s",bien2);  // lay ten thu muc

			rm_non_empty_dir(bien2,sd,ip);
			
		}
		

	}
	

	cd(path, sd);

	_rmdir(file,sd,ip);


	close(sockfd); //Dong socket

}


void get(char *file, const int sd,char * ip)
{

	
	char retr[N];
	char buff[N];
	int fd;
	int sockfd;

	

	memset(retr, '\0', N*sizeof(char));
	sprintf(retr,"RETR %s\r\n",file);

	// cho o che do PASSIVE
	sockfd =  pasv(sd,ip);
	
	
	

	//RETR 
	if( send(sd, retr, strlen(retr), 0) < 0 ){

		perror("send");
		exit(2);

	}
	
	int codeftp;
	memset(buff, '\0', N*sizeof(char));
	if( recv(sd, buff, sizeof(buff), 0 ) < 0){

		perror("recv");
		exit(5);
	}
	else
	{
		printf("%s", buff);
		sscanf(buff,"%d", &codeftp);
		if(codeftp == 550)
		{
			replylogcode(codeftp);
			return;
		}
		
	}

	if(0 > (fd = open(file, O_CREAT|O_TRUNC|O_WRONLY, S_IRUSR|S_IWUSR))) {  //read/write

		perror("open");
		close(sockfd); //Dong socket
		return;
	}


	int len;
	//Nhan du lieu tu server
	memset(buff, '\0', N*sizeof(char));
	len = recv(sockfd, buff, sizeof(buff), 0 ); 
	write(fd, buff, len);


	send226_transfer_complete(sd);


	close(sockfd); //Dong socket

}

char put(char *file, const int sd,char * ip)
{	
	char stor[N];

	char buff[N];
	int fd;
	int sockfd;

	memset(stor, '\0', N*sizeof(char));
	sprintf(stor,"STOR %s\r\n",file);

	// cho o che do PASSIVE
	sockfd =  pasv(sd,ip);
	
	
	if( 0 > (fd = open(file, O_RDONLY)) ){

		perror("open");
		return 2;
	}

	//RETR 
	if( send(sd, stor, strlen(stor), 0) < 0 ){

		perror("send");
		return 0;
		//exit(2);

	}
	
	memset(buff, '\0', N*sizeof(char));
	if( recv(sd, buff, sizeof(buff), 0 ) < 0){

		perror("recv");
		return 0;
		// exit(5);
	}
	else printf("%s",buff);



	int len;
	memset(buff, '\0', N*sizeof(char));
	while( (len = read(fd, buff, N))  > 0){

		if( 0 > send(sockfd, buff, len, 0) ){

			perror("send");
			return 0;
			// exit(1);
		}
	}
	close(sockfd); //Dong socket
	return 1;
}

void service_handleCmd(const int socket, char**ap_argv, const int a_argc, char * ip)
{

	///////////  pwd   ///////////////////////
	if(strcmp(ap_argv[0], "lpwd") == 0)
	{
		lpwd();

	}
	else if(strcmp(ap_argv[0], "pwd") == 0)
	{
		pwd(socket);

	}
	else if(strcmp(ap_argv[0], "cd") == 0)
	{
		if( a_argc != 2)
		{
			printf("usage: cd remote-directory\n");
			return;
		}

		cd( ap_argv[1] ,socket);
	}
	else if(strcmp(ap_argv[0], "lcd") == 0)
	{
		if(a_argc != 2)			
		{
			printf("usage: lcd remote-directory\n");
			return;
		}

		lcd( ap_argv[1] );
	}
	else if(strcmp(ap_argv[0], "quit") == 0)
	{
		quit(socket);
	}
	else if(strcmp(ap_argv[0], "get") == 0)
	{

		if(a_argc != 2)			
		{
			printf("usage: get filename\n");
			return;
		}
		get(ap_argv[1] ,socket, ip);


	}
	else if(strcmp(ap_argv[0], "put") == 0)
	{
		if(a_argc != 2)			
		{
			printf("usage: put filename\n");
			return;
		}
		put(ap_argv[1] ,socket,ip);
		send226_transfer_complete(socket);
	}
	else if(strcmp(ap_argv[0], "mkdir") == 0)
	{
		if( a_argc != 2)
		{
			printf("usage: mkdir directory\n");
			return;
		}

		_mkdir(ap_argv[1] ,socket);
	}
	else if(strcmp(ap_argv[0], "mrmdir") == 0)
	{
		if( a_argc != 2)
		{
			printf("usage: mrmdir directory\n");
			return;
		}
		else if( a_argc == 2)
			rm_non_empty_dir(ap_argv[1],socket, ip);
	}
	else if(strcmp(ap_argv[0], "rmdir") == 0)
	{
		if( a_argc != 2)
		{
			printf("usage: rmdir directory\n");
			return;
		}
		_rmdir(ap_argv[1],socket,ip);
		
	}
	else if(strcmp(ap_argv[0], "delete") == 0)
	{
		if( a_argc != 2)
		{
			printf("usage: delete filename\n");
			return;
		}
		delete(ap_argv[1] ,socket);
	}
	else if(strcmp(ap_argv[0], "dir") == 0)
	{
		if(a_argc == 1)
			dir_non_path(ap_argv[1],socket, ip);
		else			
			dir_path(ap_argv[1],socket, ip);

		
	}
	else if(strcmp(ap_argv[0], "ls") == 0)
	{
		if(a_argc ==1)
			ls_non_path(ap_argv[1] ,socket, ip);
		else
			ls_path(ap_argv[1] ,socket, ip);
		
		
	}
	else
		printf("?Invalid command\n");


}


void service_create(int socket, char* ip)
{

	char cmd[50];
	char**p_argv;
	int argc;
	char * bufCR;

	for(;;)
	{	
		__fpurge(stdin);
		printf("ftp> ");
		__fpurge(stdin);
		memset(&cmd, '\0', sizeof(cmd)); // clear buffer
		fgets(cmd, sizeof(cmd), stdin);
		__fpurge(stdin);

		// remove newline
		if((bufCR = strrchr(cmd, '\n')) != NULL)
			*bufCR = '\0';

		p_argv =NULL;


		if(strcmp(cmd, "exit") == 0)
		{
			exit(1);
		}
		else if(strcmp(cmd, "help") == 0)
		{
			printf("\tls ");
			printf("\n\tdir ");
			printf("\n\tpwd");
			printf("\n\tlpwd");
			printf("\n\tcd");
			printf("\n\tlcd");
			printf("\n\tget");
			printf("\n\tput ");
			printf("\n\thelp");
			printf("\n\tquit");
			printf("\n\tmkdir");
			printf("\n\trmdir");
			printf("\n\tmrmdir");
			printf("\n\tdelete");
			printf("\n\texit\n");
		}
		else if(strlen(cmd) > 0)
		{
				// parse command arguments

			p_argv = service_parseArgs(cmd, &argc);
			if(p_argv == NULL || argc <= 0)
			{
				
				printf("?Invalid command\n");
			}
			else
			{
				service_handleCmd(socket, p_argv, argc,ip);
				
			}

				// clean up
			service_freeArgs(p_argv, argc);
			p_argv = NULL;
		}

	}

}

char ReadConfig(char* ip, char* username, char* password, char* filename)
{
	char row[100];
    FILE *fptr; // pointer to received file
	
    if ((fptr = fopen("client_config.txt", "r")) == NULL)
    {
        printf("Error! Config file not found");
		return 0;
        //// Program exits if file pointer returns NULL.                 
    }
	else
	{
		printf("opened config succesfully \r\n");
		//// reads server 8021 user/pass
		while (fgets( row, sizeof( row ), fptr ) != NULL ) 
		{ 
			char** frame_parts;
			frame_parts = str_split(row, ',');
			if(frame_parts)
			{					
				if (strcmp(*(frame_parts),"user") == 0)
				{
					printf("setting server's username...");
					
					sprintf(username,"%s", *(frame_parts + 1));
					printf("Server username: %s___", username);
					printf("Done\r\n");
				}
				else if (strcmp(*(frame_parts),"pass") == 0)
				{
					printf("setting server's password...");
					sprintf(password,"%s", *(frame_parts + 1));
					printf("Server password: %s___", password);
					printf("Done\r\n");
				}
				else if (strcmp(*(frame_parts),"ip") == 0)
				{
					printf("setting server ip address...");
					sprintf(ip,"%s", *(frame_parts + 1));
					printf("Server IP address: %s___", ip);
					printf("Done\r\n");
				}
				else if (strcmp(*(frame_parts),"fname") == 0)
				{
					printf("setting valid name for command file...");
					sprintf(filename,"%s", *(frame_parts + 1));
					printf("Name: %s___", filename);
					printf("Done\r\n");
				}
				
				free(frame_parts);
			}
		}
		
		fclose(fptr);
		printf("Configuration completed!");
		return  1;
	}
}
char** str_split(char* a_str, const char a_delim)
{
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}
//read waiting_list and refill ip connect eshtablished succesfully
void RefillFile()
{
	char row[100];
    FILE *fptr; // pointer to received file
	FILE *tmp_ptr;
	
    if ((fptr = fopen("waiting_list.txt", "r+")) == NULL)
    {
        printf("Error! waiting_list not found");
		return 0;
        //// Program exits if file pointer returns NULL.                 
    }
	tmp_ptr = fopen("temp_wlist.txt", "w"); //save exits file name and filepath after refill
	
	
		printf("opened \"waiting_list.txt\" succesfully \r\n");
		//// reads server 8021 user/pass
		while (fgets( row, sizeof( row ), fptr ) != NULL ) 
		{ 
			printf("Sending file %s ...", row);
			char** frame_parts;
			frame_parts = str_split(row, ',');
			if(frame_parts)
			{	
				struct sockaddr_in *remote;
				struct stat obj;
				int sock;
				int  tmpres;
				sock = socket(AF_INET, SOCK_STREAM, 0);
				if(sock != -1)
				{
					remote = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in *));
					remote->sin_family = AF_INET;
					tmpres = inet_pton(AF_INET, ip, (void *)(&(remote->sin_addr.s_addr)));
					if( tmpres > 0)  
					{
						remote->sin_port = htons(PORT);
						tmpres = connect(sock,(struct sockaddr*)remote, sizeof(struct sockaddr));
						if(tmpres != -1)
						{
							if(session_create(sock, ip, username, password))
							{
								lcd(*(frame_parts));
								char c_put_result = 0;
								if((c_put_result=put(*(frame_parts+1), sock, ip)) == 1)
								{
									close(sock);
								}
								else if(c_put_result == 0) // == 2 as not found file, so not add to waiting list again	
								{
									fprintf(tmp_ptr, "%s,%s,\r\n",*(frame_parts),*(frame_parts+1));
								}	
								lcd("..");
								free(frame_parts);								
							}
							else	
							{
								fprintf(tmp_ptr, "%s,%s,\r\n",*(frame_parts),*(frame_parts+1));
							}	
						}
						else	
						{
							fprintf(tmp_ptr, "%s,%s,\r\n",*(frame_parts),*(frame_parts+1));
						}	
					}
					else	
					{
						fprintf(tmp_ptr, "%s,%s,\r\n",*(frame_parts),*(frame_parts+1));
					}	
				}
				else	
				{
					fprintf(tmp_ptr, "%s,%s,\r\n",*(frame_parts),*(frame_parts+1));
				}					
			}	
		}
		fclose(fptr);
		fclose(tmp_ptr);
		remove("waiting_list.txt");
		rename("temp_wlist.txt", "waiting_list.txt");
		//printf("Refill completed!");
		return  1;
	
}
void InsertWaitingList(char* fpath, char* fname)
{
	char row[100];
    FILE *fptr; // pointer to received file
	
    fptr = fopen("waiting_list.txt", "a");
    fprintf(fptr, "%s,%s,\r\n",fpath,fname);
	printf("Add to \"wating_list.txt\"");
}