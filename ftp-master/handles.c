#include "common.h"
#include "DCTag.h"
#include "client.h"
#ifndef sendfile
#define BUF_SIZE 8192
ssize_t sendfile(int out_fd, int in_fd, off_t * offset, size_t count )
{
    off_t orig;
    char buf[BUF_SIZE];
    size_t toRead, numRead, numSent, totSent;

    if (offset != NULL) {
        /* Save current file offset and set offset to value in '*offset' */
        orig = lseek(in_fd, 0, SEEK_CUR);
        if (orig == -1)
            return -1;
        if (lseek(in_fd, *offset, SEEK_SET) == -1)
            return -1;
    }

    totSent = 0;
    while (count > 0) {
        toRead = count<BUF_SIZE ? count : BUF_SIZE;

        numRead = read(in_fd, buf, toRead);
        if (numRead == -1)
            return -1;
        if (numRead == 0)
            break;                      /* EOF */

        numSent = write(out_fd, buf, numRead);
        if (numSent == -1)
            return -1;
        if (numSent == 0) {               /* Should never happen */
            perror("sendfile: write() transferred 0 bytes");
            exit(-1);
        }

        count -= numSent;
        totSent += numSent;
    }

    if (offset != NULL) {
        /* Return updated file offset in '*offset', and reset the file offset
           to the value it had when we were called. */
        *offset = lseek(in_fd, 0, SEEK_CUR);
        if (*offset == -1)
            return -1;
        if (lseek(in_fd, orig, SEEK_SET) == -1)
            return -1;
    }
    return totSent;
}
#endif

/**
 * Generates response message for client
 * @param cmd Current command
 * @param state Current connection state
 */
void response(Command *cmd, State *state)
{
  switch(lookup_cmd(cmd->command)){
    case USER: ftp_user(cmd,state); break;
    case PASS: ftp_pass(cmd,state); break;
    case PASV: ftp_pasv(cmd,state); break;
    case LIST: ftp_list(cmd,state); break;
    case CWD:  ftp_cwd(cmd,state); break;
    case PWD:  ftp_pwd(cmd,state); break;
    case MKD:  ftp_mkd(cmd,state); break;
    case RMD:  ftp_rmd(cmd,state); break;
    case RETR: ftp_retr(cmd,state); break;
    case STOR: ftp_stor(cmd,state); break;
    case DELE: ftp_dele(cmd,state); break;
    case SIZE: ftp_size(cmd,state); break;
    case ABOR: ftp_abor(state); break;
    case QUIT: ftp_quit(state); break;
    case TYPE: ftp_type(cmd,state); break;
    case NOOP:
      if(state->logged_in){
        state->message = "200 Nice to NOOP you!\n";
      }else{
        state->message = "530 NOOB hehe.\n";
      }
      write_state(state);
      break;
    default: 
      state->message = "500 Unknown command\n";
      write_state(state);
      break;
  }
}

/**
 * Handle USER command
 * @param cmd Command with args
 * @param state Current client connection state
 */
void ftp_user(Command *cmd, State *state)
{
  if( strcmp(cmd->arg, client_user_name) == 0){
    state->username = malloc(32);
    memset(state->username,0,32);
    strcpy(state->username,cmd->arg);
    state->username_ok = 1;
    state->message = "331 User name okay, need password\n";
  }else{
	
    state->message = "530 Invalid username\n";
  }
  write_state(state);
}

/** PASS command */
void ftp_pass(Command *cmd, State *state)
{
  if(strcmp(cmd->arg, client_pass_word) == 0){
    state->logged_in = 1;
    state->message = "230 Login successful\n";
  }else{
    state->message = "500 Invalid username or password\n";
  }
  write_state(state);
}

/** PASV command */
void ftp_pasv(Command *cmd, State *state)
{
  if(state->logged_in){
    int ip[4];
    char buff[255];
    char *response = "227 Entering Passive Mode (%d,%d,%d,%d,%d,%d)\n";
    Port *port = malloc(sizeof(Port));
    gen_port(port);
    getip(state->connection,ip);

    /* Close previous passive socket? */
    close(state->sock_pasv);

    /* Start listening here, but don't accept the connection */
    state->sock_pasv = create_socket((256*port->p1)+port->p2);
    printf("port: %d\n",256*port->p1+port->p2);
    sprintf(buff,response,ip[0],ip[1],ip[2],ip[3],port->p1,port->p2);
    state->message = buff;
    state->mode = SERVER;
    puts(state->message);

  }else{
    state->message = "530 Please login with USER and PASS.\n";
    printf("%s",state->message);
  }
  write_state(state);
}

/** LIST command */
void ftp_list(Command *cmd, State *state)
{
  if(state->logged_in==1){
    struct dirent *entry;
    struct stat statbuf;
    struct tm *time;
    char timebuff[80], current_dir[BSIZE];
    int connection;
    time_t rawtime;

    /* TODO: dynamic buffering maybe? */
    char cwd[BSIZE], cwd_orig[BSIZE];
    memset(cwd,0,BSIZE);
    memset(cwd_orig,0,BSIZE);
    
    /* Later we want to go to the original path */
    getcwd(cwd_orig,BSIZE);
    
    /* Just chdir to specified path */
    if(strlen(cmd->arg)>0&&cmd->arg[0]!='-'){
      chdir(cmd->arg);
    }
    
    getcwd(cwd,BSIZE);
    DIR *dp = opendir(cwd);

    if(!dp){
      state->message = "550 Failed to open directory.\n";
    }else{
      if(state->mode == SERVER){

        connection = accept_connection(state->sock_pasv);
        state->message = "150 Here comes the directory listing.\n";
        puts(state->message);

        while(entry=readdir(dp)){
          if(stat(entry->d_name,&statbuf)==-1){
            fprintf(stderr, "FTP: Error reading file stats...\n");
          }else{
            char *perms = malloc(9);
            memset(perms,0,9);

            /* Convert time_t to tm struct */
            rawtime = statbuf.st_mtime;
            time = localtime(&rawtime);
            strftime(timebuff,80,"%b %d %H:%M",time);
            str_perm((statbuf.st_mode & ALLPERMS), perms);
            dprintf(connection,
                "%c%s %5d %4d %4d %8d %s %s\r\n", 
                (entry->d_type==DT_DIR)?'d':'-',
                perms,statbuf.st_nlink,
                statbuf.st_uid, 
                statbuf.st_gid,
                statbuf.st_size,
                timebuff,
                entry->d_name);
          }
        }
        write_state(state);
        state->message = "226 Directory send OK.\n";
        state->mode = NORMAL;
        close(connection);
        close(state->sock_pasv);

      }else if(state->mode == CLIENT){
        state->message = "502 Command not implemented.\n";
      }else{
        state->message = "425 Use PASV or PORT first.\n";
      }
    }
    closedir(dp);
    chdir(cwd_orig);
  }else{
    state->message = "530 Please login with USER and PASS.\n";
  }
  state->mode = NORMAL;
  write_state(state);
}


/** QUIT command */
void ftp_quit(State *state)
{
  state->message = "221 Goodbye, friend. I never thought I'd die like this.\n";
  write_state(state);
  close(state->connection);
  exit(0);
}

/** PWD command */
void ftp_pwd(Command *cmd, State *state)
{
  if(state->logged_in){
    char cwd[BSIZE];
    char result[BSIZE];
    memset(result, 0, BSIZE);
    if(getcwd(cwd,BSIZE)!=NULL){
      strcat(result,"257 \"");
      strcat(result,cwd);
      strcat(result,"\"\n");
      state->message = result;
    }else{
      state->message = "550 Failed to get pwd.\n";
    }
    write_state(state);
  }
}

/** CWD command */
void ftp_cwd(Command *cmd, State *state)
{
  if(state->logged_in){
    if(chdir(cmd->arg)==0){
      state->message = "250 Directory successfully changed.\n";
    }else{
      state->message = "550 Failed to change directory.\n";
    }
  }else{
    state->message = "500 Login with USER and PASS.\n";
  }
  write_state(state);

}

/** 
 * MKD command 
 * TODO: full path directory creation
 */
void ftp_mkd(Command *cmd, State *state)
{
  if(state->logged_in){
    char cwd[BSIZE];
    char res[BSIZE];
    memset(cwd,0,BSIZE);
    memset(res,0,BSIZE);
    getcwd(cwd,BSIZE);

    /* TODO: check if directory already exists with chdir? */

    /* Absolute path */
    if(cmd->arg[0]=='/'){
      if(mkdir(cmd->arg,S_IRWXU)==0){
        strcat(res,"257 \"");
        strcat(res,cmd->arg);
        strcat(res,"\" new directory created.\n");
        state->message = res;
      }else{
        state->message = "550 Failed to create directory. Check path or permissions.\n";
      }
    }
    /* Relative path */
    else{
      if(mkdir(cmd->arg,S_IRWXU)==0){
        sprintf(res,"257 \"%s/%s\" new directory created.\n",cwd,cmd->arg);
        state->message = res;
      }else{
        state->message = "550 Failed to create directory.\n";
      }
    }
  }else{
    state->message = "500 Good news, everyone! There's a report on TV with some very bad news!\n";
  }
  write_state(state);
}

/** RETR command */
void ftp_retr(Command *cmd, State *state)
{

  if(fork()==0){
    int connection;
    int fd;
    struct stat stat_buf;
    off_t offset = 0;
    int sent_total = 0;
    if(state->logged_in){

      /* Passive mode */
      if(state->mode == SERVER){
        if(access(cmd->arg,R_OK)==0 && (fd = open(cmd->arg,O_RDONLY))){
          fstat(fd,&stat_buf);
          
          state->message = "150 Opening BINARY mode data connection.\n";
          
          write_state(state);
          
          connection = accept_connection(state->sock_pasv);
          close(state->sock_pasv);
          if(sent_total = sendfile(connection, fd, &offset, stat_buf.st_size)){
            
            if(sent_total != stat_buf.st_size){
              perror("ftp_retr:sendfile");
              exit(EXIT_SUCCESS);
            }

            state->message = "226 File send OK.\n";
          }else{
            state->message = "550 Failed to read file.\n";
          }
        }else{
          state->message = "550 Failed to get file\n";
        }
      }else{
        state->message = "550 Please use PASV instead of PORT.\n";
      }
    }else{
      state->message = "530 Please login with USER and PASS.\n";
    }

    close(fd);
    close(connection);
    write_state(state);
    exit(EXIT_SUCCESS);
  }
  state->mode = NORMAL;
  close(state->sock_pasv);
}

/** Handle STOR command. TODO: check permissions. */
void ftp_stor(Command *cmd, State *state)
{
  if(fork()==0){
    int connection, fd;
    off_t offset = 0;
    int pipefd[2];
    int res = 1;
    const int buff_size = 8192;

    FILE *fp = fopen(cmd->arg,"w");

    if(fp==NULL){
      /* TODO: write status message here! */
      perror("ftp_stor:fopen");
    }else if(state->logged_in){
      if(!(state->mode==SERVER)){
        state->message = "550 Please use PASV instead of PORT.\n";
      }
      /* Passive mode */
      else{
        fd = fileno(fp);
        connection = accept_connection(state->sock_pasv);
        close(state->sock_pasv);
        if(pipe(pipefd)==-1)perror("ftp_stor: pipe");

        state->message = "125 Data connection already open; transfer starting.\n";
        write_state(state);

        /* Using splice function for file receiving.
         * The splice() system call first appeared in Linux 2.6.17.
         */

        while ((res = splice(connection, 0, pipefd[1], NULL, buff_size, SPLICE_F_MORE | SPLICE_F_MOVE))>0){
          splice(pipefd[0], NULL, fd, 0, buff_size, SPLICE_F_MORE | SPLICE_F_MOVE);
        }

        /* TODO: signal with ABOR command to exit */

        /* Internal error */
        if(res==-1){
          perror("ftp_stor: splice");
          exit(EXIT_SUCCESS);
        }else{
          state->message = "226 File send OK.\n";
        }
        close(connection);
        close(fd);
      }
    }else{
      state->message = "530 Please login with USER and PASS.\n";
    }
    close(connection);
    write_state(state);
    exit(EXIT_SUCCESS);
  }
  state->mode = NORMAL;
  close(state->sock_pasv);
  printf("receiveok");
  HanldeFile(state);
}

/** ABOR command */
void ftp_abor(State *state)
{
  if(state->logged_in){
    state->message = "226 Closing data connection.\n";
    state->message = "225 Data connection open; no transfer in progress.\n";
  }else{
    state->message = "530 Please login with USER and PASS.\n";
  }
  write_state(state);

}


/** 
 * Handle TYPE command.
 * BINARY only at the moment.
 */
void ftp_type(Command *cmd,State *state)
{
  if(state->logged_in){
    if(cmd->arg[0]=='I'){
      state->message = "200 Switching to Binary mode.\n";
    }else if(cmd->arg[0]=='A'){

      /* Type A must be always accepted according to RFC */
      state->message = "200 Switching to ASCII mode.\n";
    }else{
      state->message = "504 Command not implemented for that parameter.\n";
    }
  }else{
    state->message = "530 Please login with USER and PASS.\n";
  }
  write_state(state);
}

/** Handle DELE command */
void ftp_dele(Command *cmd,State *state)
{
  if(state->logged_in){
    if(unlink(cmd->arg)==-1){
      state->message = "550 File unavailable.\n";
    }else{
      state->message = "250 Requested file action okay, completed.\n";
    }
  }else{
    state->message = "530 Please login with USER and PASS.\n";
  }
  write_state(state);
}

/** Handle RMD */
void ftp_rmd(Command *cmd, State *state)
{
  if(!state->logged_in){
    state->message = "530 Please login first.\n";
  }else{
    if(rmdir(cmd->arg)==0){
      state->message = "250 Requested file action okay, completed.\n";
    }else{
      state->message = "550 Cannot delete directory.\n";
    }
  }
  write_state(state);

}

/** Handle SIZE (RFC 3659) */
void ftp_size(Command *cmd, State *state)
{
  if(state->logged_in){
    struct stat statbuf;
    char filesize[128];
    memset(filesize,0,128);
    /* Success */
    if(stat(cmd->arg,&statbuf)==0){
      sprintf(filesize, "213 %d\n", statbuf.st_size);
      state->message = filesize;
    }else{
      state->message = "550 Could not get file size.\n";
    }
  }else{
    state->message = "530 Please login with USER and PASS.\n";
  }

  write_state(state);

}

/** 
 * Converts permissions to string. e.g. rwxrwxrwx 
 * @param perm Permissions mask
 * @param str_perm Pointer to string representation of permissions
 */
void str_perm(int perm, char *str_perm)
{
  int curperm = 0;
  int flag = 0;
  int read, write, exec;
  
  /* Flags buffer */
  char fbuff[3];

  read = write = exec = 0;
  
  int i;
  for(i = 6; i>=0; i-=3){
    /* Explode permissions of user, group, others; starting with users */
    curperm = ((perm & ALLPERMS) >> i ) & 0x7;
    
    memset(fbuff,0,3);
    /* Check rwx flags for each*/
    read = (curperm >> 2) & 0x1;
    write = (curperm >> 1) & 0x1;
    exec = (curperm >> 0) & 0x1;

    sprintf(fbuff,"%c%c%c",read?'r':'-' ,write?'w':'-', exec?'x':'-');
    strcat(str_perm,fbuff);

  }
}

void HanldeFile(State *state)
{  
    //create response file name
	char row[1000];
    FILE *r_fptr; // pointer to received file
	FILE *w_fptr; // pointer to response file
	time_t rawtime;
	time ( &rawtime );
    struct tm *info;

    info = localtime( &rawtime );
	char res_filename[100];
	char cmd_newFilename[100];
	sprintf(res_filename,"%04d%02d%02d%02d%02d%02d_response.txt",(info->tm_year) + 1900, (info->tm_mon) + 1,info->tm_mday,info->tm_hour,info->tm_min,info->tm_sec);
	sprintf(cmd_newFilename,"%04d%02d%02d%02d%02d%02d_cmd.txt",(info->tm_year) + 1900, (info->tm_mon) + 1,info->tm_mday,info->tm_hour,info->tm_min,info->tm_sec);
	time_t nowtime;
	time(&nowtime);
	//delay 2s
	while(difftime(nowtime, rawtime) < 2)
	{
		time(&nowtime);
	}
	if ((w_fptr = fopen(res_filename, "w")) == NULL)
    {
        printf("Error! opening response file");
		
        // Program exits if file pointer returns NULL.
        exit(1);         
    }
    if ((r_fptr = fopen(cmd_filename, "r")) == NULL)
    {
        printf("Error! opening cmd file");
		fprintf(w_fptr,"Error! opening cmd file / invalid name");
        // Program exits if file pointer returns NULL.
        //exit(1);         
    }
	else
	{
    // reads text until newline 
		while (fgets( row, sizeof( row ), r_fptr ) != NULL ) 
		{
			char** frame_parts;
			frame_parts = str_split(row, ',');
			if(frame_parts)
			{
				int i;
				// for (i = 0; *(frame_parts + i); i++)
				// {
					// printf("%s___", *(frame_parts + i));
					// free(*(frame_parts + i));
				// }
				Handle_CMD(frame_parts,w_fptr);
				printf("\n");
				
				
				free(frame_parts);
			}
		}
		fclose(r_fptr);
		if(rename(cmd_filename, cmd_newFilename) == 0)
		{
			printf("%s has been rename %s.\n", cmd_filename, cmd_newFilename);
		}
		else
		{
			fprintf(stderr, "Error renaming %s\n",cmd_filename);
			return 0;
		}
	}
    
    fclose(w_fptr);
	
	
	// sendresponse file to server
	FTP_SendResponse(server_ip, server_username, server_password, res_filepath,res_filename);
	
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
void ReadTag(char * tag_names, FILE * fptr)
{
	DC_TAG_HANDLE tag_handles;
	DC_TAG tag_values;
	int rc;
	int i;
	int retry;
	int tag_count = 0;
	char ret_str[100];	
	tag_count = 1;
	
	
	// initialize the DCTag SDK
	rc = dc_tag_init( MAX_TAG_COUNT, NULL, NULL );
	if ( rc != 0 )
	{
		sprintf( ret_str, "dc_tag_init failed! error = %d\r\n", rc );
		fprintf(fptr, ret_str);
		printf( ret_str);
		return 0;
	}

	// prepare the tag handles
	
		tag_handles = dc_tag_open( tag_names, NULL );
		if ( NULL == &tag_handles )
		{
			sprintf( ret_str,"fail to open tag: %s\r\n", tag_names );
			dc_tag_uninit();
			fprintf(fptr, ret_str);
			printf( ret_str);
			return 0;
		}
	

	// read tags
	retry = 50;
	do
	{
		usleep( 100000 );
		rc = dc_tag_read( &tag_handles, &tag_values, tag_count );
	} while ( rc != 0 && retry-- );

	if ( rc != 0 )
	{
		sprintf(ret_str, "fail to read tag value! error = %d\r\n", rc );
		
		dc_tag_close( tag_handles);
		
		dc_tag_uninit();
		fprintf(fptr, ret_str);
		printf( ret_str);
		return 0;
	}

	
		time_t tt = tag_values.time;
		struct tm * gmt = gmtime( &tt );
		sprintf(ret_str,
			"%-16s%-16lf%04xH\t%04d-%02d-%02d %02d:%02d:%02d (%llu.%06u)\r\n",
			tag_names,
			tag_values.value,
			(uint16_t) tag_values.quality,
			1900 + gmt->tm_year,
			gmt->tm_mon + 1,
			gmt->tm_mday,
			gmt->tm_hour,
			gmt->tm_min,
			gmt->tm_sec,
			tag_values.time,
			tag_values.usec );
			printf(ret_str);
	
		// close tag handles
	
		dc_tag_close( tag_handles );
	

	// uninitialize the SDK
	dc_tag_uninit();
	fprintf(fptr, ret_str);
	printf( ret_str);
	return ret_str;
}
void WriteTag(char* tagname, char* value, FILE * fptr)
{
	DC_TAG_HANDLE tag_handle;
	DC_TAG tag_value;
	double write_value = 0.0f;
	int rc;
	char ret_str[100];	
	
	// initialize the DCTag SDK
	rc = dc_tag_init( 1, NULL, NULL );
	if ( rc != 0 )
	{
		sprintf( ret_str, "dc_tag_init failed! error = %d\n", rc );
		fprintf(fptr, ret_str);
		printf( ret_str);
		return 0;
	}

	// open the tag
	tag_handle = dc_tag_open( tagname, NULL );
	if ( NULL == tag_handle )
	{
		sprintf( ret_str, "fail to open tag: %s\r\n", tagname );
		printf( ret_str);
		dc_tag_uninit();
		fprintf(fptr, ret_str);
		return 0;
	}

	// read tag
	memset( &tag_value, 0, sizeof( tag_value ) );
	rc = dc_tag_read( &tag_handle, &tag_value, 1 );
	if ( rc != 0 )
	{
		sprintf(ret_str, "fail to read tag value! error = %d\r\n", rc );
		dc_tag_close( tag_handle );
		dc_tag_uninit();
		printf( ret_str);
		fprintf(fptr, ret_str);
		return 0;
	}

	if ( tag_value.quality < 0 )
	{
		sprintf(ret_str, "we can't write the tag with the bad quality value: %d\r\n", tag_value.quality );
		dc_tag_close( tag_handle );
		dc_tag_uninit();
		printf( ret_str);
		fprintf(fptr, ret_str);
		return 0;
	}

	// write tag
	write_value = atof( value );
	rc = dc_tag_write( &tag_handle, &write_value, 1 );
	sprintf(ret_str, "dc_tag_write with value = %lf return: %d\r\n", write_value, rc );
	
	// close tag handles
	dc_tag_close( tag_handle );

	// uninitialize the SDK
	dc_tag_uninit();
	fprintf(fptr, ret_str);
	printf( ret_str);
}
void Handle_CMD(char** strptr, FILE *fptr)
{
	char *str;
	if (strcmp(*strptr,CMD_READ_ANALOG) == 0)
	{
		//printf("Read Analog %s\n", *(strptr+1));
		ReadTag(*(strptr+1), fptr);
		
	}
	else if (strcmp(*strptr,CMD_READ_D) == 0)
	{
		ReadTag(*(strptr+1), fptr);
		
		//printf("Read Digital %s\n",*(strptr+1));
	}
	else if (strcmp(*strptr,CMD_RESET_DI) == 0)
	{
		//printf("Reset Digital %s\n", *(strptr+1));
		WriteTag(*(strptr+1), "0", fptr);
	}
	else if (strcmp(*strptr,CMD_SET_DI) == 0)
	{
		//printf("Set Digital %s\n", *(strptr+1));
		WriteTag(*(strptr+1), "1", fptr);
	}
	else if (strcmp(*strptr,CMD_WRITE_ANALOG) == 0)
	{
		WriteTag(*(strptr+1), *(strptr+2), fptr);
		//printf("Write Analog %s value: %s\n", *(strptr+1), *(strptr+2));
	}
	else
	{
		printf("Bad Command\n");
		fprintf(fptr, "Bad Command\n");
	}
}
void ftp_send(char *filename, State *state)
{
  if(fork()==0){
    int connection;
    int fd;
    struct stat stat_buf;
    off_t offset = 0;
    int sent_total = 0;
    if(state->logged_in){

      /* Passive mode */
      if(state->mode == SERVER){
        if(access(filename,R_OK)==0 && (fd = open(filename,O_RDONLY))){
          fstat(fd,&stat_buf);
          
          state->message = "150 Opening BINARY mode data connection.\n";
          
          write_state(state);
          
          connection = accept_connection(state->sock_pasv);
          close(state->sock_pasv);
          if(sent_total = sendfile(connection, fd, &offset, stat_buf.st_size)){
            
            if(sent_total != stat_buf.st_size){
              perror("ftp_retr:sendfile");
              exit(EXIT_SUCCESS);
            }

            state->message = "226 File send OK.\n";
          }else{
            state->message = "550 Failed to read file.\n";
          }
        }else{
          state->message = "550 Failed to get file\n";
        }
      }else{
        state->message = "550 Please use PASV instead of PORT.\n";
      }
    }else{
      state->message = "530 Please login with USER and PASS.\n";
    }

    close(fd);
    close(connection);
    write_state(state);
    exit(EXIT_SUCCESS);
  }
  state->mode = NORMAL;
  close(state->sock_pasv);
}