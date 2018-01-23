#include <memory.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include "DCTag.h"

#define MAX_TAG_COUNT					20

struct tag_data
{
	char name[ 100 ];
	DC_TAG_HANDLE handle;
	double value;
};
typedef struct tag_data					TAG_DATA;

TAG_DATA tags[ MAX_TAG_COUNT ];

static int on_read_tag (
	DC_TAG_HANDLE tag_handles[],
	void * tag_context[],
	DC_TAG tag_values[],
	uint32_t count,
	void * user_context )
{
	uint32_t i;
	struct timeval tv;
	struct timezone tz;

	gettimeofday( &tv, &tz );

	printf( "%s: count = %u\n", __func__, count );

	for ( i = 0; i < count; i++ )
	{
		TAG_DATA * tag = (TAG_DATA*) tag_context[ i ];

		if ( tag != &tags[ 2 ] )
		{
			tag_values[ i ].value = tag->value;
		}
		else
		{
			// tag_3 = tag_1 * tag_2
			tag_values[ i ].value = tags[ 0 ].value * tags[ 1 ].value;
		}

		tag_values[ i ].quality = 0;
		tag_values[ i ].time = tv.tv_sec;
		tag_values[ i ].usec = tv.tv_usec;

		printf(
			"tag name = %s, value = %lf, quality = %04xH, time = %llu.%u\n",
			tag->name,
			tag_values[ i ].value,
			tag_values[ i ].quality,
			tag_values[ i ].time,
			tag_values[ i ].usec );
	}

	return 0;
}

static int on_write_tag (
	DC_TAG_HANDLE tag_handles[],
	void * tag_context[],
	double tag_values[],
	uint32_t count,
	void * user_context )
{
	uint32_t i;

	printf( "%s: count = %u\n", __func__, count );

	for ( i = 0; i < count; i++ )
	{
		TAG_DATA * tag = (TAG_DATA*) tag_context[ i ];

		if ( tag != &tags[ 2 ] )
		{
			tag->value = tag_values[ i ];
			printf(
				"tag name = %s, value = %lf\n",
				tag->name,
				tag->value );
		}
		else
		{
			printf( "tag name = %s, don't allow writing!\n", tag->name );
		}
	}

	return 0;
}


int main ( int argc, char * argv[] )
{
	DC_TAG_CALLBACK callback;
	int rc;
	int i;
	
	memset( tags, 0, sizeof( tags ) );

	callback.size = sizeof( callback );
	callback.read_tag = on_read_tag;
	callback.write_tag = on_write_tag;
	callback.update_tag = NULL;

	// initialize the DCTag SDK
	rc = dc_tag_init( MAX_TAG_COUNT, &callback, NULL );
	if ( rc != 0 )
	{
		printf( "dc_tag_init failed! error = %d\n", rc );
		return -1;
	}

	// prepare the tag handles
	for ( i = 0; i < MAX_TAG_COUNT; i++ )
	{
		sprintf( tags[ i ].name, "tag_%d", i + 1 );

		tags[ i ].handle = dc_tag_add(
			tags[ i ].name,
			&tags[ i ] );
		if ( NULL == tags[ i ].handle )
		{
			printf( "fail to add tag: %s\n", tags[ i ].name );
			dc_tag_uninit();
			return -2;
		}
	}

	while ( 1 )
	{
		sleep( 1 );
	}

	// close tag handles
	for ( i = 0; i < MAX_TAG_COUNT; i++ )
	{
		dc_tag_remove( tags[ i ].handle );
	}

	// uninitialize the SDK
	dc_tag_uninit();
	
	return 0;
}
