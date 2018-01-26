#include <memory.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "DCTag.h"
#include <dirent.h>
#include <assert.h>


#define MAX_TAG_COUNT					20
void LookupUnit(char* pchr_tag_name, char* pchr_result);
char** str_split(char* a_str, const char a_delim);
void ReadTag( FILE * fptr, char * tag_names, char* unit, char scale, char* min, char* max);

int main ()
{
	char *tag_names[ MAX_TAG_COUNT ];
	char *tag_units[ MAX_TAG_COUNT ];
	DC_TAG_HANDLE tag_handles[ MAX_TAG_COUNT ];
	DC_TAG tag_values[ MAX_TAG_COUNT ];
	int rc;
	int i;
	int retry;
	int tag_cnt = 0;
	char ret_str[100];
	
	FILE *ptr_trc_file;
	//read tag_list
	if ((ptr_trc_file = fopen("tag_read_config.txt", "r")) != NULL)
	{
		char row[100];	
		FILE *ptr_file;
				
		time_t f_time ;
		time(&f_time);
		struct tm *f_tm ;
		f_tm = localtime(&f_time);
		char filename[50];
		sprintf(filename,"%04d_%02d_%02d/temp.txt", (f_tm->tm_year)+1900, (f_tm->tm_mon)+1, f_tm->tm_mday);
		ptr_file =fopen(filename, "w");
		
		while (fgets( row, sizeof( row ), ptr_trc_file ) != NULL ) 
		{
			char** frame_parts;
			frame_parts = str_split(row, ',');
			if(frame_parts)
			{			
				tag_names[tag_cnt] = *(frame_parts);
				tag_units[tag_cnt++] = *(frame_parts + 1);
			}
		}
		fclose(ptr_trc_file);
		// limit tag num
		if ( tag_cnt > MAX_TAG_COUNT + 1 )
		{
			printf( "too many tag name specified, the max tag count is %d!\n", MAX_TAG_COUNT );
			return -3;
		}
		// init to read tag
		memset( tag_handles, 0, sizeof( tag_handles ) );
		memset( tag_values, 0, sizeof( tag_values ) );
		
		rc = dc_tag_init( MAX_TAG_COUNT, NULL, NULL );
		if ( rc != 0 )
		{
			printf( "dc_tag_init failed! error = %d\n", rc );
			return -4;
		}

		// prepare the tag handles
		for ( i = 0; i < tag_cnt; i++ )
		{
			tag_handles[ i ] = dc_tag_open( tag_names[ i ], NULL );
			if ( NULL == tag_handles[ i ] )
			{
				printf( "fail to open tag: %s\n", tag_names[ i ] );
				dc_tag_uninit();
				return -5;
			}
		}
		//read tags
		retry = 50;
		do
		{
			usleep( 100000 );
			rc = dc_tag_read( tag_handles, tag_values, tag_cnt );
		} while ( rc != 0 && retry-- );

		if ( rc != 0 )
		{
			printf( "fail to read tag value! error = %d\n", rc );
			for ( i = 0; i < tag_cnt; i++ )
			{
				dc_tag_close( tag_handles[ i ] );
			}
			dc_tag_uninit();
			return -6;
		}
		for ( i = 0; i < tag_cnt; i++ )
		{
			time_t tt = tag_values[i].time;
			struct tm * gmt = gmtime( &tt );
			sprintf(ret_str,
				"%s\t%5.6lf\t%s\t%04d%02d%02d%02d%02d%02d\t%04xH\r\n",
				tag_names[i],
				tag_values[i].value,
				tag_units[i],
				1900 + gmt->tm_year,
				gmt->tm_mon + 1,
				gmt->tm_mday,
				gmt->tm_hour,
				gmt->tm_min,
				gmt->tm_sec,
				(uint16_t) tag_values[i].quality );

			fprintf(ptr_file, ret_str);
			printf( ret_str);
		}
		// close tag handles
		for ( i = 0; i < tag_cnt; i++ )
		{
			dc_tag_close( tag_handles[ i ] );
		}

		// uninitialize the SDK
		dc_tag_uninit();
		fclose(ptr_file);
	}
	else
	{
		printf("file \"tag_read_config.txt\" not found!!!");
		return 0;
	}
	return 0;
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
void ReadTag( FILE * fptr, char * tag_names, char* unit, char scale, char* min, char* max)
{
	DC_TAG_HANDLE tag_handles;
	DC_TAG tag_values;
	int rc;
	
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
		"%s\t%5.6lf\t%s\t%04d%02d%02d%02d%02d%02d\t%04xH\r\n",
		tag_names,
		tag_values.value,
		unit,
		1900 + gmt->tm_year,
		gmt->tm_mon + 1,
		gmt->tm_mday,
		gmt->tm_hour,
		gmt->tm_min,
		gmt->tm_sec,
		(uint16_t) tag_values.quality );

	// close tag handles

	dc_tag_close( tag_handles );


	// uninitialize the SDK
	dc_tag_uninit();
	fprintf(fptr, ret_str);
	printf( ret_str);
	return ret_str;
}