/*************************************************
 * developer: Evgenii F.
 * reviewer: Nir D.
 * date: 1.02.2023
 *
 **************************************************/
#include <stdlib.h> /*malloc*/
#include <stdio.h> /*perror*/
#include <string.h>/*memcpy*/
#include <assert.h>

#include "vector.h"

#define GROWTH_FACTOR ((size_t)2)
#define DECREASE_FACTOR ((size_t)4)

static void *ChangeVector(vector_t *vector, size_t bytes_num);

struct vector
{
    void *data;
    size_t item_size;
    size_t capacity;
    size_t size;
};

vector_t *VectorCreate(size_t capacity, size_t item_size)
{
    vector_t *new_vector = (vector_t *)malloc(sizeof(vector_t));
    vector_t *res = new_vector;
    if (NULL == new_vector)
    {
        perror("Memory allocation error\n");
        res = NULL;
    }
    else
    {
        new_vector->capacity = capacity;
        new_vector->item_size = item_size;
        new_vector->size = 0;
        new_vector->data = malloc(capacity * item_size);
        if (NULL == new_vector->data)
        {
            perror("Memory allocation error\n");
            res = NULL;
            free(new_vector);
        }
    }
    return res;
}

void VectorDestroy(vector_t *vector)
{
    assert(NULL != vector);
    free(vector->data);
    free(vector);
}

int VectorPushBack(vector_t *vector, const void *element)
{
    int res = 0;
    void *tmp_vector = NULL;
    assert(NULL != vector);
    assert(NULL != element);
    assert(vector->size < vector->capacity);
    
    if (vector->size + 1 >= vector->capacity)
    {
        tmp_vector = ChangeVector(vector, vector->capacity * vector->item_size * GROWTH_FACTOR);
        if (NULL == tmp_vector)
        {
            perror("Memory allocation problem\n");
            res = 1;
        }
        else
        {
            vector->data = tmp_vector;
            vector->capacity *= GROWTH_FACTOR;
        }
    }
    memcpy((char *)vector->data + (vector->size * vector->item_size), element, vector->item_size);
    ++vector->size;

    return res;
}

void VectorPopBack(vector_t *vector)
{
    void *tmp_vector = NULL;
    assert(NULL != vector);
    assert(0 != vector->size);
    
    --vector->size;
    if (vector->size <= (vector->capacity / DECREASE_FACTOR))
    {
        tmp_vector = ChangeVector(vector, vector->capacity * vector->item_size * GROWTH_FACTOR);
        if (NULL == tmp_vector)
        {
            perror("Memory allocation problem\n");
        }
        else
        {
            vector->data = tmp_vector;
            vector->capacity /= GROWTH_FACTOR;
        }
    }
}

void *VectorGetAccessToElement(const vector_t *vector, size_t index)
{
    assert(NULL != vector);
    assert(index < vector->size);
    return (char *)vector->data + (index * vector->item_size);
}

int VectorReserve(vector_t *vector, size_t new_capacity)
{
    int res = 0;
    void *tmp_vector = NULL;
    assert(NULL != vector);
    if (vector->capacity < new_capacity)
    {
    	tmp_vector = ChangeVector(vector, vector->item_size * new_capacity);
	if (NULL == tmp_vector)
	{
	    perror("Memory allocation problem\n");
	    res = 1;
	}
        else
	{
            vector->data = tmp_vector;
	    vector->capacity = new_capacity;
	}
    }
    return res;
}

int VectorShrinkToSize(vector_t *vector)
{
    int res = 0;
    void *tmp_vector = NULL;
    assert(NULL != vector);
    tmp_vector = ChangeVector(vector, ((vector->size + 1) * vector->item_size));
    if (NULL == tmp_vector)
    {
        perror("Memory allocation problem\n");
        res = 1;
    }
    else
    {
        vector->data = tmp_vector;
        vector->capacity = vector->size + 1;
    }

    return res;
}

int VectorIsEmpty(const vector_t *vector)

{
    assert(NULL != vector);
    return 0 == vector->size;
}

size_t VectorCapacity(const vector_t *vector)
{
    assert(NULL != vector);
    return vector->capacity;
}

size_t VectorSize(const vector_t *vector)
{
    assert(NULL != vector);
    return vector->size;
}

static void *ChangeVector(vector_t *vector, size_t bytes_num)
{
    assert(NULL != vector);
    return realloc(vector->data, bytes_num);
}

