#include <memory.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "DCTag.h"

#define MAX_TAG_COUNT					100

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

	printf( "\n%-16s%-16s%-8s%s\n", "Tag Name", "Value", "Quality", "Time Stamp (UTC)" );
	printf( "--------------- --------------- ------- ---------------------------------------\n" );

	for ( i = 0; i < tag_count; i++ )
	{
		time_t tt = tag_values[ i ].time;
		struct tm * gmt = gmtime( &tt );
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
	}
	
	printf( "\n" );

	// close tag handles
	for ( i = 0; i < tag_count; i++ )
	{
		dc_tag_close( tag_handles[ i ] );
	}

	// uninitialize the SDK
	dc_tag_uninit();
	
	return 0;
}
