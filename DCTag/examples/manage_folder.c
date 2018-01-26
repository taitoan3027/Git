#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>
#include<dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>

int remove_directory(const char *path);
char** str_split(char* a_str, const char a_delim);
int GetMaxDay();
uint32_t max_day = 0;
int main()
{	
	if((max_day = GetMaxDay()) != 0)
	{
		time_t rawtime;
		time ( &rawtime );
		struct tm *info;
		info = localtime( &rawtime );
		
		
		
		char newfilename[100];
		char oldfilename[100];
		sprintf(newfilename, "%04d_%02d_%02d", (info->tm_year) + 1900, (info->tm_mon) + 1, info->tm_mday);
		
		struct tm *old_info;
		time_t old_time = rawtime - max_day * 86400;
		old_info = localtime( &old_time );
		sprintf(oldfilename, "%04d_%02d_%02d", (old_info->tm_year) + 1900, (old_info->tm_mon) + 1, old_info->tm_mday);
		
		int result = mkdir(newfilename, 0777);
		remove_directory(oldfilename);
	}
	return 0;
 }

 int remove_directory(const char *path)
{
   DIR *d = opendir(path);
   size_t path_len = strlen(path);
   int r = -1;

   if (d)
   {
      struct dirent *p;

      r = 0;

      while (!r && (p=readdir(d)))
      {
          int r2 = -1;
          char *buf;
          size_t len;

          /* Skip the names "." and ".." as we don't want to recurse on them. */
          if (!strcmp(p->d_name, ".") || !strcmp(p->d_name, ".."))
          {
             continue;
          }

          len = path_len + strlen(p->d_name) + 2; 
          buf = malloc(len);

          if (buf)
          {
             struct stat statbuf;

             snprintf(buf, len, "%s/%s", path, p->d_name);

             if (!stat(buf, &statbuf))
             {
                if (S_ISDIR(statbuf.st_mode))
                {
                   r2 = remove_directory(buf);
                }
                else
                {
                   r2 = unlink(buf);
                }
             }

             free(buf);
          }

          r = r2;
      }

      closedir(d);
   }

   if (!r)
   {
      r = rmdir(path);
   }

   return r;
}
int GetMaxDay()
{
	int i_day;
    FILE *r_fptr; // pointer to config file
	char row[100];
	if ((r_fptr = fopen("client_config.txt", "r")) == NULL)
    {
        printf("Error! can't found config file");  
		return 0;
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
				if(strcmp(*(frame_parts), "day") == 0)
				{
					
					i_day = atoi(*(frame_parts+1));
					printf("Set Max Storage timestamp (day): %d", i_day);
					//sprintf(server_username,"%s", *(frame_parts+1));
				}
			}
		}
		return i_day;
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