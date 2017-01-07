#include "../include/pc_buffer.h"

void pc_buffer_init(PC_Buffer *buffer, uint16_t buffer_size) {
	buffer->array = malloc(sizeof(char)*buffer_size);
	buffer->buffer_SIZE = buffer_size;
	buffer->consume_count = 0;
	buffer->produce_count = 0;
}

void pc_buffer_add(PC_Buffer *buffer, char data) {
	buffer->array[buffer->produce_count++ % buffer->buffer_SIZE] = data;
}

void pc_buffer_remove(PC_Buffer *buffer, char *data) {
	*data = buffer->array[buffer->consume_count++ % buffer->buffer_SIZE];
}

bool pc_buffer_empty(PC_Buffer *buffer) {
	return buffer->produce_count == buffer->consume_count;
}

bool pc_buffer_full(PC_Buffer *buffer) {
	return buffer->produce_count - buffer->consume_count == buffer->buffer_SIZE;
}
