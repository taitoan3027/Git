#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "DCTag.h"

void print_tag_value ( DC_TAG * tag )
{
	time_t tt;
	struct tm * gmt;

	printf( "Value: %lf\n", tag->value );
	printf( "Quality: %04xH\n", (uint16_t) tag->quality );
	tt = tag->time;
	gmt = gmtime( &tt );
	printf(
		"Time Stamp (UTC): %04d-%02d-%02d %02d:%02d:%02d (%llu.%06u)\n\n",
		1900 + gmt->tm_year,
		gmt->tm_mon + 1,
		gmt->tm_mday,
		gmt->tm_hour,
		gmt->tm_min,
		gmt->tm_sec,
		tag->time,
		tag->usec );
}

int main ( int argc, char * argv[] )
{
	DC_TAG_HANDLE tag_handle;
	DC_TAG tag_value;
	double write_value = 0.0f;
	int rc;
	
	if ( argc != 3 )
	{
		printf( "usage: dc_tag_write <tag_name> <tag_value>\n" );
		return -1;
	}

	// initialize the DCTag SDK
	rc = dc_tag_init( 1, NULL, NULL );
	if ( rc != 0 )
	{
		printf( "dc_tag_init failed! error = %d\n", rc );
		return -2;
	}

	// open the tag
	tag_handle = dc_tag_open( argv[ 1 ], NULL );
	if ( NULL == tag_handle )
	{
		printf( "fail to open tag: %s\n", argv[ 1 ] );
		dc_tag_uninit();
		return -3;
	}

	// read tag
	memset( &tag_value, 0, sizeof( tag_value ) );
	rc = dc_tag_read( &tag_handle, &tag_value, 1 );
	if ( rc != 0 )
	{
		printf( "fail to read tag value! error = %d\n", rc );
		dc_tag_close( tag_handle );
		dc_tag_uninit();
		return -4;
	}

	if ( tag_value.quality < 0 )
	{
		printf( "we can't write the tag with the bad quality value: %d\n", tag_value.quality );
		dc_tag_close( tag_handle );
		dc_tag_uninit();
		return -5;
	}

	// write tag
	write_value = atof( argv[ 2 ] );
	rc = dc_tag_write( &tag_handle, &write_value, 1 );
	printf( "dc_tag_write with value = %lf return: %d\n", write_value, rc );
	printf( "\n" );

	// close tag handles
	dc_tag_close( tag_handle );

	// uninitialize the SDK
	dc_tag_uninit();
	
	return 0;
}
