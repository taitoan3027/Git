#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include "DCTag.h"

#define MAX_TAG_COUNT					3

DC_TAG tag_data[ MAX_TAG_COUNT ];
DC_TAG_HANDLE tag_added[ MAX_TAG_COUNT ];
DC_TAG_HANDLE tag_opened[ MAX_TAG_COUNT ];

static uint32_t get_tick_count ( void )
{
	uint32_t tick;
	struct timespec tp = { 0, 0 };

	clock_gettime( CLOCK_MONOTONIC, &tp );
	tick = tp.tv_sec * 1000 + tp.tv_nsec / ( 1000 * 1000 );

	return tick;
}

static int on_read_tag (
	DC_TAG_HANDLE tag_handles[],
	void * tag_context[],
	DC_TAG tag_values[],
	uint32_t count,
	void * user_context )
{
	uint32_t i;

	printf( "%s: count = %u\n", __func__, count );

	for ( i = 0; i < count; i++ )
	{
		DC_TAG * tag = (DC_TAG*) tag_context[ i ];

		if ( tag == &tag_data[ 0 ] )
		{
			printf( "%s: tag_1 = %lf\n", __func__, tag->value );
		}
		else if ( tag == &tag_data[ 1 ] )
		{
			printf( "%s: tag_2 = %lf\n", __func__, tag->value );
		}
		else if ( tag == &tag_data[ 2 ] )
		{
			printf( "%s: tag_3 = %lf\n", __func__, tag->value );
		}
		else
		{
			printf( "%s: unexpected tag context %p\n", __func__, tag );
			continue;
		}

		tag_values[ i ] = *tag;
	}

	return 0;
}

static void recalc_tag_3 ( struct timeval * tv )
{
	double value;

	value = tag_data[ 0 ].value + tag_data[ 1 ].value;

	if ( value != tag_data[ 2 ].value )
	{
		tag_data[ 2 ].value = value;
		tag_data[ 2 ].time = tv->tv_sec;
		tag_data[ 2 ].usec = tv->tv_usec;

		dc_tag_update( &tag_added[ 2 ], &tag_data[ 2 ], 1 );
	}
}

static int on_write_tag (
	DC_TAG_HANDLE tag_handles[],
	void * tag_context[],
	double tag_values[],
	uint32_t count,
	void * user_context )
{
	struct timeval tv;
	struct timezone tz;
	uint32_t i;

	printf( "%s: %u count = %u\n", __func__, get_tick_count(), count );

	gettimeofday( &tv, &tz );

	for ( i = 0; i < count; i++ )
	{
		DC_TAG * tag = (DC_TAG*) tag_context[ i ];

		if ( tag == &tag_data[ 0 ] )
		{
			printf( "write tag_1 with %lf!\n", tag_values[ i ] );

			if ( tag->value != tag_values[ i ] )
			{
				tag->value = tag_values[ i ];
				tag->time = tv.tv_sec;
				tag->usec = tv.tv_usec;

				recalc_tag_3( &tv );

				dc_tag_update( &tag_handles[ 0 ], tag, 1 );
			}
		}
		else if ( tag == &tag_data[ 1 ] )
		{
			printf( "write tag_2 with %lf!\n", tag_values[ i ] );

			if ( tag->value != tag_values[ i ] )
			{
				tag->value = tag_values[ i ];
				tag->time = tv.tv_sec;
				tag->usec = tv.tv_usec;

				recalc_tag_3( &tv );

				dc_tag_update( &tag_handles[ 1 ], tag, 1 );
			}
		}
		else if ( tag == &tag_data[ 2 ] )
		{
			printf( "Writing to tag_3 is not allowed!\n" );
		}
	}

	printf( "%s: %u finished\n", __func__, get_tick_count() );

	return 0;
}

static int on_update_tag (
	DC_TAG_HANDLE tag_handle,
	void * tag_context,
	DC_TAG * tag_value,
	void * user_context )
{
	if ( tag_handle == tag_opened[ 0 ] )
	{
		printf( "%s: tag_1 = %lf\n", __func__, tag_value->value );
	}
	else if ( tag_handle == tag_opened[ 1 ] )
	{
		printf( "%s: tag_2 = %lf\n", __func__, tag_value->value );
	}
	else if ( tag_handle == tag_opened[ 2 ] )
	{
		printf( "%s: tag_3 = %lf\n", __func__, tag_value->value );
	}
	else
	{
		printf( "%s: unexpected tag handle %p\n", __func__, tag_handle );
		return -1;
	}

	return 0;
}

int main ( int argc, char * argv[] )
{
	DC_TAG_CALLBACK callback;
	char tag_name[ 100 ];
	struct timeval tv;
	struct timezone tz;
	int rc;
	int i;
	
	srand( time( NULL ) );

	callback.size = sizeof( callback );
	callback.read_tag = on_read_tag;
	callback.write_tag = on_write_tag;
	callback.update_tag = on_update_tag;

	// initialize the DCTag SDK
	rc = dc_tag_init( MAX_TAG_COUNT * 2, &callback, NULL );
	if ( rc != 0 )
	{
		printf( "dc_tag_init failed! error = %d\n", rc );
		return -1;
	}

	gettimeofday( &tv, &tz );

	// prepare the tag handles
	for ( i = 0; i < MAX_TAG_COUNT; i++ )
	{
		sprintf( tag_name, "tag_%d", i + 1 );

		tag_data[ i ].value = rand() % 10;
		tag_data[ i ].quality = 0;
		tag_data[ i ].time = tv.tv_sec;
		tag_data[ i ].usec = tv.tv_usec;

		printf( "%s = %lf\n", tag_name, tag_data[ i ].value );

		tag_added[ i ] = dc_tag_add(
			tag_name,
			&tag_data[ i ] );
		if ( NULL == tag_added[ i ] )
		{
			printf( "fail to add tag: %s\n", tag_name );
			dc_tag_uninit();
			return -2;
		}

		tag_opened[ i ] = dc_tag_open( tag_name, NULL );
		if ( NULL == tag_opened[ i ] )
		{
			printf( "fail to open tag: %s\n", tag_name );
			dc_tag_uninit();
			return -3;
		}
	}

	while ( 1 )
	{
		sleep( 10 );
		gettimeofday( &tv, &tz );

		for ( i = 0; i < 2; i++ )
		{
			tag_data[ i ].value += 1;
			tag_data[ i ].time = tv.tv_sec;
			tag_data[ i ].usec = tv.tv_usec;
		}

		rc = dc_tag_update( tag_added, tag_data, 2 );
		if ( rc != 0 )
		{
			printf( "dc_tag_update failed! error = %d\n", rc );
		}

		recalc_tag_3( &tv );
	}

	// close tag handles
	for ( i = 0; i < MAX_TAG_COUNT; i++ )
	{
		dc_tag_remove( tag_added[ i ] );
	}

	// uninitialize the SDK
	dc_tag_uninit();
	
	return 0;
}
