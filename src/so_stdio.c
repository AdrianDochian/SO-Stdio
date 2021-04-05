#include "so_stdio.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

typedef struct _so_file {
    int fileDescriptor;
} SO_FILE;

SO_FILE *so_fopen(const char *pathname, const char *mode)
{
    SO_FILE *f = malloc(sizeof(SO_FILE));
    int fd = -1;

    if (strcmp(mode, "r") == 0)
    {
        fd = open(pathname, O_RDONLY);
    }
    else if (strcmp(mode, "w") == 0)
    {
        fd = open(pathname, O_WRONLY);
    }
    else if (strcmp(mode, "a") == 0)
    {
        fd = open(pathname, O_APPEND);
    }
    else if (strcmp(mode, "r+") == 0)
    {
        fd = open(pathname, O_RDONLY, O_CREAT);
    }
    else if (strcmp(mode, "w+") == 0)
    {
        fd = open(pathname, O_WRONLY, O_CREAT);
    }
    else if (strcmp(mode, "a+") == 0)
    {
        fd = open(pathname, O_APPEND, O_CREAT);
    }

    if (fd < 0)
    {
        free(f);
        return NULL;
    }

    f->fileDescriptor = fileDescriptor;

    return f;
}

int so_fclose(SO_FILE *stream) {
    return -1;
}

int so_fflush(SO_FILE *stream)
{
    return -1;
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
    size_t result = -1;
    return result;
}

size_t so_fwrite(const void *ptr, size_t size, size_t nmemb, SO_FILE *stream)
{
    size_t result = -1;

    return result;
}

int so_fgetc(SO_FILE *stream)
{
    return -1;
}
int so_fputc(int c, SO_FILE *stream)
{
    return -1;
}

int so_feof(SO_FILE *stream)
{
    return - 1;
}
int so_ferror(SO_FILE *stream)
{
    return -1;
}

SO_FILE *so_popen(const char *command, const char *type)
{
    return NULL;
}
int so_pclose(SO_FILE *stream) {
    return -1;
}

int so_fileno(SO_FILE *stream) {
    return -1;
}
