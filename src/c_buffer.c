/*************************************************
* developer: Evgenii
* reviewer: Neviu
* date: 15.02.2023
*
**************************************************/

#include <stdlib.h> /*malloc*/
#include <stdio.h>  /*perror*/
#include <string.h> /*memcpy*/
#include <errno.h>
#include <assert.h>

#include "c_buffer.h"

#define BUFFER_END (buffer->data[buffer->capacity])

enum buffer_status
{
    EMPTY = 0,
    FULL = 1
};

enum error_status
{
    BUFFER_IS_FULL = -1,
    NOTHING_WRITTEN_TO_BUFFER = 0,
    BUFFER_IS_EMPTY = -1,
    NOTHING_READ_FROM_BUFFER = 0
};

struct c_buffer
{
    size_t capacity;
    size_t read;
    size_t write;
    char data[1];
};

/***************************Functions***********************************/

c_buffer_t *CBufferCreate(size_t capacity)
{
    c_buffer_t *new_buffer = (c_buffer_t *)malloc(sizeof(c_buffer_t) + capacity);

    if (NULL == new_buffer)
    {
        perror("Memory allocation error\n");
    }
    else
    {
        new_buffer->capacity = capacity;
        new_buffer->read = 0;
        new_buffer->write = 0;
        new_buffer->data[capacity] = EMPTY;
    }

    return new_buffer;
}

void CBufferDestroy(c_buffer_t *buffer)
{
    assert(NULL != buffer);

    free(buffer);
}

ssize_t CBufferWrite(c_buffer_t *buffer, const void *src, size_t data_size)
{
    size_t bytes_to_boundary = 0;
    size_t second_byte_block = 0;
    size_t actuall_size = 0;
    size_t free_space = CBufferFreeSpace(buffer);
    size_t write = 0;
    size_t read = 0;

    assert(NULL != buffer);
    assert(NULL != src);

    if (0 == free_space)
    {
        errno = ECANCELED;
        perror("The buffer is full\n");
        return BUFFER_IS_FULL;
    }

    actuall_size = (free_space < data_size) ? free_space : data_size;
    write = buffer->write;
    read = buffer->read;
    bytes_to_boundary = (buffer->capacity - write > actuall_size) ? actuall_size : buffer->capacity - write;
    
    if (0 == actuall_size)
    {
        return NOTHING_WRITTEN_TO_BUFFER;
    }

    memcpy(buffer->data + write, src, bytes_to_boundary);
    buffer->write = (write + bytes_to_boundary) % buffer->capacity;
    write = buffer->write;

    if (bytes_to_boundary < actuall_size)
    {
        second_byte_block = actuall_size - bytes_to_boundary;
        memcpy(buffer->data + write, (char *)src + bytes_to_boundary, second_byte_block);
        buffer->write = (write + second_byte_block) % buffer->capacity;
    }

    if (buffer->write == read)
    {
        BUFFER_END = FULL;
    }

    return actuall_size;
}

ssize_t CBufferRead(c_buffer_t *buffer, void *dest, size_t data_size)
{
    size_t bytes_to_boundary = 0;
    size_t second_byte_block = 0;
    size_t actuall_size = 0;
    size_t used_space = CBufferSize(buffer);
    size_t write = 0;
    size_t read = 0;

    assert(NULL != buffer);
    assert(NULL != dest);
    
    if (0 == used_space)
    {
        errno = ECANCELED;
        perror("The buffer is empty\n");
        return BUFFER_IS_EMPTY;
    }

    actuall_size = (used_space < data_size) ? used_space : data_size;
    
    if (0 == actuall_size)
    {
        return NOTHING_READ_FROM_BUFFER;
    }

    write = buffer->write;
    read = buffer->read;

    bytes_to_boundary = (buffer->capacity - read > actuall_size) ? actuall_size : buffer->capacity - read;
    memcpy(dest, buffer->data + read, bytes_to_boundary);
    buffer->read = (read + bytes_to_boundary) % buffer->capacity;
    read = buffer->read;

    if (actuall_size > bytes_to_boundary)
    {
        second_byte_block = actuall_size - bytes_to_boundary;
        memcpy((char *)dest + bytes_to_boundary, buffer->data + read, second_byte_block);
        buffer->read = (read + second_byte_block) % buffer->capacity;
    }

    if (buffer->read == write)
    {
        BUFFER_END = EMPTY;
    }

    return actuall_size;
}

size_t CBufferFreeSpace(const c_buffer_t *buffer)
{
    assert(NULL != buffer);

    return buffer->capacity - CBufferSize(buffer);
}

int CBufferIsEmpty(const c_buffer_t *buffer)
{
    assert(NULL != buffer);

    return (0 == CBufferSize(buffer));
}

size_t CBufferSize(const c_buffer_t *buffer)
{
    size_t write = 0;
    size_t read = 0;
    size_t res_size = 0;

    assert(NULL != buffer);

    write = buffer->write;
    read = buffer->read;

    if (write == read)
    {
        if (FULL == BUFFER_END)
        {
            res_size =  buffer->capacity;
        }
        else
        {
            res_size =  0;
        }
        
        return res_size;
    }

    return (write > read) ? write - read : buffer->capacity - (read - write);
}
