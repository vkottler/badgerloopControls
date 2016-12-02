#ifndef __PC_BUFFER_H__
#define __PC_BUFFER_H__

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct {
	uint32_t produce_count, consume_count;
	uint16_t buffer_SIZE;
	char *array;
} PC_Buffer;

void pc_buffer_init(PC_Buffer *buffer, uint16_t buffer_size);
void pc_buffer_add(PC_Buffer *buffer, char data);
void pc_buffer_remove(PC_Buffer *buffer, char *data);
bool pc_buffer_empty(PC_Buffer *buffer);
bool pc_buffer_full(PC_Buffer *buffer);

#endif
