#ifndef _MEM_H_
#define _MEM_H_


#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include <sys/mman.h>

#define HEAP_START ((void*)0x04040000)

void* _malloc( size_t query );
void  _free( void* mem );
void* heap_init( size_t initial_size );
void heap_destroy(size_t size);

#endif
