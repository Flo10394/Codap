/***********************************************************
	file: 				u8_ringbuffer.h
	
	file description: 	TODO: add your file description

	author:				Florian
	created on:			03.04.2020
***********************************************************/

#ifndef UTIL_INCLUDE_U8_RINGBUFFER_H_
#define UTIL_INCLUDE_U8_RINGBUFFER_H_

/***********************************************************
						includes
***********************************************************/
#include <stdio.h>
#include <stdint.h>
#include <util_ringbuffer_u8_config.h>


/***********************************************************
				      public defines
***********************************************************/



/***********************************************************
				  public type definitions
***********************************************************/
typedef struct{
	uint32_t in;
	uint32_t out;
	uint8_t* data;
	uint32_t size;
	uint32_t dataContaining;
}UTIL_RINGBUFFER_U8;

/***********************************************************
				public function definitions
***********************************************************/
extern void UTIL_RINGBUFFER_U8_init(UTIL_RINGBUFFER_U8* buffer,
									uint8_t* aBuffer,
									const uint32_t size);

extern void UTIL_U8_RINGBUFFER_put(UTIL_RINGBUFFER_U8* buffer,
								   const uint8_t data);

extern uint8_t UTIL_U8_RINGBUFFER_get(UTIL_RINGBUFFER_U8* buffer);

extern bool UTIL_U8_RINGBUFFER_isFull(UTIL_RINGBUFFER_U8* buffer);

extern bool UTIL_U8_RINGBUFFER_isEmpty(UTIL_RINGBUFFER_U8* buffer);

extern uint32_t UTIL_U8_RINGBUFFER_spaceAvailable(UTIL_RINGBUFFER_U8* buffer);

#endif /* UTIL_INCLUDE_U8_RINGBUFFER_H_ */
