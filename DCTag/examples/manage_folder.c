#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

int remove_directory(const char *path);

int main()
{	time_t rawtime;
	time ( &rawtime );
    struct tm *info;
	info = localtime( &rawtime );
	
	
	
	char newfilename[100];
	char oldfilename[100];
	sprintf(newfilename, "%04d_%02d_%02d", (info->tm_year) + 1900, (info->tm_mon) + 1, info->tm_mday);
	
	struct tm *old_info;
	time_t old_time = rawtime - 2592000;
	old_info = localtime( &old_time );
	sprintf(oldfilename, "%04d_%02d_%02d", (old_info->tm_year) + 1900, (old_info->tm_mon) + 1, old_info->tm_mday);
	
	int result = mkdir(newfilename, 0777);
	remove_directory(oldfilename);
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