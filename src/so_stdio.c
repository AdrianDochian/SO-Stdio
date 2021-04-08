#include "so_stdio.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define BUFF_SIZE 4096

enum LAST_OPERATION {
	READ, WRITE, NONE
};

typedef struct _so_file {
	int file_descriptor;
	int buffer_position;
	int buffer_actual_length;
	char *buffer;
	enum LAST_OPERATION last_operation;
} SO_FILE;

SO_FILE *so_fopen(const char *pathname, const char *mode)
{
	SO_FILE *stream = malloc(sizeof(SO_FILE));
	int fd = -1;

	if (strcmp(mode, "r") == 0)
		fd = open(pathname, O_RDONLY);
	else if (strcmp(mode, "w") == 0)
		fd = open(pathname, O_WRONLY|O_CREAT|O_TRUNC, 0444);
	else if (strcmp(mode, "a") == 0) {
		fd = open(pathname, O_WRONLY | O_APPEND);
		lseek(fd, 0, SEEK_END);
	} else if (strcmp(mode, "r+") == 0)
		fd = open(pathname, O_RDONLY, O_CREAT);
	else if (strcmp(mode, "w+") == 0)
		fd = open(pathname, O_WRONLY, O_CREAT);
	else if (strcmp(mode, "a+") == 0) {
		fd = open(pathname, O_WRONLY | O_APPEND | O_CREAT, 044);
		lseek(fd, 0, SEEK_END);
	}

	if (fd < 0) {
		free(stream);
		return NULL;
	}

	stream->file_descriptor = fd;
	stream->buffer_position = 0;
	stream->buffer_actual_length = 0;
	stream->buffer = calloc(BUFF_SIZE, sizeof(char));
	stream->last_operation = NONE;

	return stream;
}

int so_fclose(SO_FILE *stream)
{
	so_fflush(stream);
	free(stream->buffer);
	close(stream->file_descriptor);
	free(stream);
	return 0;
}

int so_fflush(SO_FILE *stream)
{
	if (stream == NULL)
		return SO_EOF;

	if (stream->last_operation == WRITE) {
		int to_be_write = stream->buffer_actual_length;
		int index_of_write = 0;
		int count = 0;

		while (to_be_write > 0) {
			count = write(stream->file_descriptor, stream->buffer + index_of_write, to_be_write);
			if (count < 0)
				break;

			to_be_write -= count;
			index_of_write += count;
		}

		stream->buffer_actual_length = 0;
	}

	return 0;
}

int so_fseek(SO_FILE *stream, long offset, int whence)
{
	return -1;
}

long so_ftell(SO_FILE *stream)
{
	return -1;
}


size_t so_fread(void *ptr, size_t size, size_t nmemb, SO_FILE *stream)
{
	if (stream == NULL)
		return SO_EOF;

	stream->last_operation = READ;
	int remaining_bytes_to_be_write = size * nmemb;
	int index_of_write = 0;
	int count;

	while (remaining_bytes_to_be_write > 0) {
		// check if buffer is empty and fill it if it's the case
		if (stream->buffer_position == stream->buffer_actual_length) {
			count = read(stream->file_descriptor, stream->buffer, BUFF_SIZE);
			stream->buffer_position = 0;
			stream->buffer_actual_length = count;
			if (count < 0)
				return SO_EOF;
			continue;
		}

		int left_in_buffer = stream->buffer_actual_length - stream->buffer_position;
		int bytes_to_be_write_now = left_in_buffer < remaining_bytes_to_be_write
			? left_in_buffer
			: remaining_bytes_to_be_write;

		memcpy(ptr + index_of_write, stream->buffer + stream->buffer_position, bytes_to_be_write_now);
		stream->buffer_position += bytes_to_be_write_now;
		index_of_write += bytes_to_be_write_now;
		remaining_bytes_to_be_write -= bytes_to_be_write_now;
	}
	return index_of_write / size;
}

size_t so_fwrite(const void *ptr, size_t size, size_t nmemb, SO_FILE *stream)
{
	if (stream == NULL)
		return SO_EOF;

	stream->last_operation = WRITE;

	int bytes_to_write = size * nmemb;
	int index_of_read = 0;

	while (bytes_to_write > 0) {
		if (stream->buffer_actual_length == BUFF_SIZE) {
			so_fflush(stream);
			continue;
		}

		int left_in_buffer = BUFF_SIZE - stream->buffer_actual_length;
		int bytes_to_be_write_now = left_in_buffer < bytes_to_write
			? left_in_buffer
			: bytes_to_write;

		memcpy(stream->buffer + stream->buffer_actual_length,
			ptr + index_of_read, bytes_to_be_write_now);

		stream->buffer_actual_length += bytes_to_be_write_now;
		bytes_to_write -= bytes_to_be_write_now;
		index_of_read += bytes_to_be_write_now;
	}

	return index_of_read / size;
}

int so_fgetc(SO_FILE *stream)
{
	if (stream == NULL)
		return SO_EOF;

	stream->last_operation = READ;

	if (stream->buffer_position < stream->buffer_actual_length)
		return stream->buffer[stream->buffer_position++];

	int count = read(stream->file_descriptor, stream->buffer, BUFF_SIZE);

	if (count < 0)
		return SO_EOF;

	stream->buffer_position = 0;
	stream->buffer_actual_length = count;

	return stream->buffer[stream->buffer_position++];
}
int so_fputc(int c, SO_FILE *stream)
{
	if (stream == NULL)
		return SO_EOF;

	stream->last_operation = WRITE;

	if (stream->buffer_actual_length == BUFF_SIZE)
		so_fflush(stream);

	stream->buffer[stream->buffer_actual_length++] = c;
	return c;
}

int so_feof(SO_FILE *stream)
{
	return -1;
}

int so_ferror(SO_FILE *stream)
{
	return -1;
}

SO_FILE *so_popen(const char *command, const char *type)
{
	return NULL;
}
int so_pclose(SO_FILE *stream)
{
	return -1;
}

int so_fileno(SO_FILE *stream)
{
	return stream->file_descriptor;
}
