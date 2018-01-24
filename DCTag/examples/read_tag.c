#include <memory.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "DCTag.h"
#include <dirent.h>
#include <assert.h>


#define MAX_TAG_COUNT					100
void LookupUnit(char* pchr_tag_name, char* pchr_result);
char** str_split(char* a_str, const char a_delim);

int main ( int argc, char * argv[] )
{
	char ** tag_names;
	DC_TAG_HANDLE tag_handles[ MAX_TAG_COUNT ];
	DC_TAG tag_values[ MAX_TAG_COUNT ];
	int rc;
	int i;
	int retry;
	int tag_count = 0;
	
	if ( argc < 2 )
	{
		if ( !isatty( fileno( stdin ) ) )
		{
			// try to read the tag name from stdin
			char * line;
			size_t n;
			ssize_t len;
			
			tag_names = calloc( MAX_TAG_COUNT + 1, sizeof( char* ) );
			if ( NULL == tag_names )
			{
				printf( "alloc memory error!\n" );
				return -1;
			}
			
			do
			{
				line = NULL;
				n = 0;
				len = getline( &line, &n, stdin );
				if ( len > 1 )
				{
					while ( len > 1
						&& ( line[ len - 1 ] == '\x0d' || line[ len - 1 ] == '\x0a' ) )
					{
						line[ len - 1 ] = 0;
						len--;
					}
				}

				if ( len > 0 )
				{
					tag_names[ tag_count++ ] = line;
				}
			} while ( len != -1 && tag_count < MAX_TAG_COUNT );
		}

		if ( tag_count <= 0 )
		{
			printf( "usage: dc_tag_read <tag_name> ...\n" );
			printf( "    or echo \"<tag_name>\" ... | dc_tag_read\n" );
			printf( "    or cat tag_list.txt | dc_tag_read\n" );
			printf( "    or dc_tag_read < tag_list.txt\n" );
			return -2;
		}
	}
	else
	{
		tag_names = &argv[ 1 ];
		tag_count = argc - 1;
	}

	if ( argc > MAX_TAG_COUNT )
	{
		printf( "too many tag name specified, the max tag count is %d!\n", MAX_TAG_COUNT );
		return -3;
	}

	memset( tag_handles, 0, sizeof( tag_handles ) );
	memset( tag_values, 0, sizeof( tag_values ) );
	
	// initialize the DCTag SDK
	rc = dc_tag_init( MAX_TAG_COUNT, NULL, NULL );
	if ( rc != 0 )
	{
		printf( "dc_tag_init failed! error = %d\n", rc );
		return -4;
	}

	// prepare the tag handles
	for ( i = 0; i < tag_count; i++ )
	{
		tag_handles[ i ] = dc_tag_open( tag_names[ i ], NULL );
		if ( NULL == tag_handles[ i ] )
		{
			printf( "fail to open tag: %s\n", tag_names[ i ] );
			dc_tag_uninit();
			return -5;
		}
	}

	// read tags
	retry = 50;
	do
	{
		usleep( 100000 );
		rc = dc_tag_read( tag_handles, tag_values, tag_count );
	} while ( rc != 0 && retry-- );

	if ( rc != 0 )
	{
		printf( "fail to read tag value! error = %d\n", rc );
		for ( i = 0; i < tag_count; i++ )
		{
			dc_tag_close( tag_handles[ i ] );
		}
		dc_tag_uninit();
		return -6;
	}

	FILE *ptr_file;
	
	time_t f_time ;
	time(&f_time);
	struct tm *f_tm ;
	f_tm = localtime(&f_time);
	char filename[50];
	sprintf(filename,"%04d_%02d_%02d/temp.txt", (f_tm->tm_year)+1900, (f_tm->tm_mon)+1, f_tm->tm_mday);
	ptr_file =fopen(filename, "w");
	
	
	if (!ptr_file)
		return 1;
	
	printf( "\n%-16s%-16s%-8s%s\n", "Tag Name", "Value", "Quality", "Time Stamp (UTC)" );
	printf( "--------------- --------------- ------- ---------------------------------------\n" );
	
	for ( i = 0; i < tag_count; i++ )
	{
		time_t tt = tag_values[ i ].time;
		struct tm * gmt = localtime( &tt );
		printf(
			"%-16s%-16lf%04xH\t%04d-%02d-%02d %02d:%02d:%02d (%llu.%06u)\n",
			tag_names[ i ],
			tag_values[ i ].value,
			(uint16_t) tag_values[ i ].quality,
			1900 + gmt->tm_year,
			gmt->tm_mon + 1,
			gmt->tm_mday,
			gmt->tm_hour,
			gmt->tm_min,
			gmt->tm_sec,
			tag_values[ i ].time,
			tag_values[ i ].usec );
			
			//get unit from lookup file
			//memset(p_unit,0,sizeof(p_unit));			
			char p_unit[5];
			LookupUnit(tag_names[ i ], p_unit);
			
		fprintf(ptr_file,
			"%s\t%5.6lf\t%s\t%04d%02d%02d%02d%02d%02d\t%04xH\r\n",
			tag_names[ i ],
			tag_values[ i ].value,
			p_unit,
			1900 + gmt->tm_year,
			gmt->tm_mon + 1,
			gmt->tm_mday,
			gmt->tm_hour,
			gmt->tm_min,
			gmt->tm_sec,
			(uint16_t) tag_values[ i ].quality );
	}
	
	printf( "\n" );

	// close tag handles
	for ( i = 0; i < tag_count; i++ )
	{
		dc_tag_close( tag_handles[ i ] );
	}

	// uninitialize the SDK
	dc_tag_uninit();
	fclose(ptr_file);
	
	return 0;
}
void LookupUnit(char* pchr_tag_name, char* pchr_result)
{
	// open unit config file
	FILE *ptr_unitfile;
	if ((ptr_unitfile = fopen("unit_config.txt", "r")) != NULL)
	{
		char row[50];
		char c_ufound = 0;

		while (fgets( row, sizeof( row ), ptr_unitfile ) != NULL ) 
		{
			char** frame_parts;
			frame_parts = str_split(row, ',');
			if(frame_parts)
			{
				if(strcmp(pchr_tag_name, *(frame_parts))==0)
				{
					c_ufound = 1;
					sprintf(pchr_result,"%s", *(frame_parts+1));
				}
			}
		}
		if (c_ufound == 0)
		{
			sprintf(pchr_result,"-");
		}
		fclose(ptr_unitfile);
	}
	else
	{
		sprintf(pchr_result,"!!!");
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