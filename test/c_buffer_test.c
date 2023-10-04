#include <assert.h>
#include <string.h> /*strlen*/
#include <stdio.h> /*printf*/
#include "c_buffer.h"

static void TestCBufferCreate();
static void TestCBufferWriteAndRead();
static void TestCBufferErrors();

int main()
{
    TestCBufferCreate();
    TestCBufferWriteAndRead();
    TestCBufferErrors();

    printf("All tests have been passed syccessfully\n");

    return 0;
}

static void TestCBufferCreate()
{
    size_t capacity = 10;
    c_buffer_t *buffer = CBufferCreate(capacity);
    assert(CBufferFreeSpace(buffer) == capacity);
    assert(CBufferSize(buffer) == 0);
    assert(CBufferIsEmpty(buffer) == 1);
    
    CBufferDestroy(buffer);
}

static void TestCBufferWriteAndRead()
{
    size_t capacity = 20;
    size_t free_space = 0;
    size_t used_bytes = 0;
    c_buffer_t *buffer = CBufferCreate(capacity);

    char user_data[20] = "Hello world";
    size_t str_len = strlen(user_data);
    ssize_t bytes_written = 0;

    char dest[25] = {0};
    ssize_t bytes_read = 0;

    bytes_written = CBufferWrite(buffer, user_data, str_len);
    assert((size_t)bytes_written == str_len);
    assert(CBufferSize(buffer) == str_len);

    free_space = CBufferFreeSpace(buffer);
    bytes_written = CBufferWrite(buffer, user_data, str_len); /*no enough place for second write of 11 bytes*/
    assert((size_t)bytes_written == free_space);

    used_bytes = CBufferSize(buffer);
    bytes_read = CBufferRead(buffer, dest, 25);
    assert((size_t)bytes_read == used_bytes);
    assert(strncmp(dest, user_data, str_len) == 0);

    CBufferDestroy(buffer);
}

static void TestCBufferErrors()
{
    size_t capacity = 11;
    c_buffer_t *buffer = CBufferCreate(capacity);
    ssize_t bytes_written = 0;
    ssize_t bytes_read = 0;
    char user_data[20] = "Hello world";
    size_t str_len = strlen(user_data); /*11*/
    char dest[25] = {0};

    assert(CBufferIsEmpty(buffer) == 1);

    bytes_written = CBufferWrite(buffer, user_data, str_len);
    assert((size_t)bytes_written == str_len);
    assert(CBufferIsEmpty(buffer) == 0);
    bytes_written = CBufferWrite(buffer, user_data, 1); /*error - buffer is full*/
    assert(bytes_written == -1);

    bytes_read = CBufferRead(buffer, dest, 25);
    assert((size_t)bytes_read == str_len);
    assert(CBufferIsEmpty(buffer) == 1);

    bytes_read = CBufferRead(buffer, dest, 25); /*trying to read empty buffer*/
    assert(bytes_read == -1);

    CBufferDestroy(buffer);
}


