/***********************************************************
	file: 				u8_ringbuffer.c
	
	file description: 	TODO: add your file description

	author:				Florian
	created on:			03.04.2020
***********************************************************/


/***********************************************************
						includes
***********************************************************/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <util_ringbuffer_u8.h>


/***********************************************************
					configuration options
***********************************************************/



/***********************************************************
					private definitions
***********************************************************/



/***********************************************************
					   private types
***********************************************************/



/***********************************************************
					private variables
***********************************************************/



/***********************************************************
				private function declarations
***********************************************************/



/***********************************************************
				public function definitions
***********************************************************/
extern void UTIL_RINGBUFFER_U8_init(UTIL_RINGBUFFER_U8* buffer,
									uint8_t* aBuffer,
									const uint32_t size)
{
	buffer->in = 0;
	buffer->out = 0;
	buffer->data = aBuffer;
	buffer->size = size;
	buffer->dataContaining = 0;
}

extern void UTIL_U8_RINGBUFFER_put(UTIL_RINGBUFFER_U8* buffer,
								   const uint8_t data)
{
	buffer->data[buffer->in] = data;
	buffer->dataContaining++;
	if(buffer->in < buffer->size)
	{
		buffer->in++;
	}
	else
	{
		buffer->in = 0;
	}

}

extern uint8_t UTIL_U8_RINGBUFFER_get(UTIL_RINGBUFFER_U8* buffer)
{
	uint8_t value = buffer->data[buffer->out];
	buffer->dataContaining--;
	if(buffer->out < buffer->size)
		buffer->out++;
	else
	{
		buffer->out = 0;
	}
	return value;
}

extern bool UTIL_U8_RINGBUFFER_isFull(UTIL_RINGBUFFER_U8* buffer)
{
	if(buffer->dataContaining == buffer->size)
	{
		return true;
	}
	else
	{
		return false;
	}
}

extern bool UTIL_U8_RINGBUFFER_isEmpty(UTIL_RINGBUFFER_U8* buffer)
{
	if(buffer->dataContaining == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

extern uint32_t UTIL_U8_RINGBUFFER_spaceAvailable(UTIL_RINGBUFFER_U8* buffer)
{
	return (buffer->size - buffer->dataContaining);
}


/***********************************************************
				private function definitions
***********************************************************/
