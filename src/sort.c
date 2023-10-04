/********************
* Dev: Evgenii
* Rev: Aaron
* Date: 28.03.2023
*********************/

#include <assert.h>
#include <stdlib.h> /*malloc*/
#include <time.h>   /*srand*/
#include <string.h>   /*srand*/

#include "sort.h"


#define EXP_BASE (10)
#define DIGITS_NUMBER (10)
#define WORD_SIZE (__WORDSIZE / __CHAR_BIT__)

enum func_status
{
    ERROR = 1,
    SUCCESS = 0
};

typedef enum func_status f_status_t;

static void Swap(int *lhs, int *rhs);

static void FindMinAndMaxInArray(int arr[], size_t size, int *min, int *max);
static void FillTmpArray(int tmp_arr[], int base_array[], size_t base_size, int min);
static void RefillBaseArray(int tmp_arr[], size_t tmp_arr_size, int base_array[], int min);
static void R_Counting(int array[], int output[], size_t size, size_t exp);

/***************************ADDED 28.03.2023 *********************************/

static int Merge(int arr[], size_t left, size_t mid, size_t right);
static ssize_t RecBinary(int arr[], size_t left_i, size_t right_i, int num);
static int RecMergeSort(int arr[], size_t left, size_t right);
static void Swap_Q(void *a, void *b, size_t elm_size);
static void QsortRec(void *arr, size_t left_i, size_t right_i, size_t elm_size, size_t num_elms, int (*cmp_func)(const void *, const void *));
static size_t Partition(void *arr, size_t left_i, size_t right_i, size_t elm_size, int (*cmp_func)(const void *, const void *));

/**********************Functions*****************************/


void BubbleSort(int array[], size_t size, int (*cmp_func)(const int lhs, const int rhs))
{
    size_t i = 0;
    size_t j = 0;

    assert(NULL != cmp_func);
    assert(NULL != array);

    for (i = 0; i < size - 1; ++i)
    {
        for (j = 0; j < size - i - 1; ++j)
        {
            if (0 < cmp_func(array[j], array[j + 1]))
            {
                Swap(&array[j], &array[j + 1]);
            }
        }
    }
}

void SelectionSort(int array[], size_t size, int (*cmp_func)(const int lhs, const int rhs))
{
    size_t i = 0, j = 0;
    size_t min_index = 0;

    assert(NULL != cmp_func);
    assert(NULL != array);

    for (i = 0; i < size - 1; ++i)
    {
        min_index = i;
        for (j = i + 1; j < size; ++j)
        {
            if (0 > cmp_func(array[j], array[min_index]))
            {
                min_index = j;
            }
        }
        Swap(&array[i], &array[min_index]);
    }
}

void InsertionSort(int array[], size_t size, int (*cmp_func)(const int lhs, const int rhs))
{
    size_t i = 0;
    int j = 0;
    int tmp = 0;

    assert(NULL != cmp_func);
    assert(NULL != array);

    for (i = 1; i < size; ++i)
    {
        tmp = array[i];
        j = i - 1;

        while (0 <= j && 0 > cmp_func(tmp, array[j]))
        {
            array[j + 1] = array[j];
            --j;
        }

        array[j + 1] = tmp;
    }
}


int CountingSort(int array[], size_t size)
{
    int min = 0, max = 0;
    size_t tmp_arr_size = 0;
    int *tmp_arr = NULL;

    assert(NULL != array);

    FindMinAndMaxInArray(array, size, &min, &max);
    tmp_arr_size = (size_t)(max - min + 1);
    tmp_arr = (int *)calloc(tmp_arr_size, sizeof(int));

    if (NULL == tmp_arr)
    {
        return ERROR;
    }

    FillTmpArray(tmp_arr, array, size, min);
    RefillBaseArray(tmp_arr, tmp_arr_size, array, min);

    free(tmp_arr);

    return SUCCESS;
}


int RadixSort(int array[], size_t size)
{
    int min = 0;
    int max = 0;
    int *output = NULL;
    size_t exp = 0;

    assert(NULL != array);

    FindMinAndMaxInArray(array, size, &min, &max);

    output = (int *)calloc(size, sizeof(int));

    if (NULL == output)
    {
        return ERROR;
    }

    for (exp = 1; 0 < (max / exp); exp *= EXP_BASE)
    {
        R_Counting(array, output, size, exp);
    }

    free(output);

    return SUCCESS;
}

ssize_t BinarySearchIterative(int *arr, size_t size, int num)
{
    size_t left_i = 0;
    size_t right_i = size - 1;
    ssize_t res = -1;
    size_t mid = 0;

    assert(NULL != arr);

    while (left_i <= right_i)
    {
        mid = left_i + (right_i - left_i) / 2;

        if (arr[mid] == num)
        {
            return mid;
        }
        if (arr[mid] > num)
        {
            right_i = mid - 1;
        }
        else
        {
            left_i = mid + 1;
        }
    }

    return res;
}

ssize_t BinarySearchRecursive(int *arr, size_t size, int num)
{
    assert(NULL != arr);

    return RecBinary(arr, 0, size - 1, num);
}


int MergeSort(int *arr_to_sort, size_t num_elements)
{
    if (NULL == arr_to_sort)
    {
        return ERROR;
    }

    return RecMergeSort(arr_to_sort, 0, num_elements - 1);
}


void Qsort(void *base, size_t num_elements, size_t size, int (*compar)(const void *, const void *))
{
    assert(NULL != base);
    assert(NULL != compar);

    srand(time(NULL));
    QsortRec(base, 0, num_elements - 1, size, num_elements, compar);
}


/**********************Static functions***********************/


static void Swap(int *lhs, int *rhs)
{
    int tmp = *lhs;
    *lhs = *rhs;
    *rhs = tmp;
}


static void RefillBaseArray(int tmp_arr[], size_t tmp_arr_size, int base_array[], int min)
{
    size_t i = 0;

    for (i = 0; i < tmp_arr_size; ++i)
    {
        while (0 < tmp_arr[i])
        {
            *base_array = i + min;
            ++base_array;
            --tmp_arr[i];
        }
    }
}

static void FillTmpArray(int tmp_arr[], int base_array[], size_t base_size, int min)
{
    size_t i = 0;

    for (i = 0; i < base_size; ++i)
    {
        ++tmp_arr[base_array[i] - min];
    }
}

static void FindMinAndMaxInArray(int arr[], size_t size, int *min, int *max)
{
    size_t i = 0;
    *min = arr[0];
    *max = arr[0];

    for (i = 1; i < size; ++i)
    {
        if (arr[i] < *min)
        {
            *min = arr[i];
        }
        if (arr[i] > *max)
        {
            *max = arr[i];
        }
    }
}

static void R_Counting(int array[], int output[], size_t size, size_t exp)
{
    size_t i = 0;
    int digit = 0;
    int digit_array[DIGITS_NUMBER] = {0};

    for (i = 0; i < size; ++i)
    {
        digit = (array[i] / exp) % EXP_BASE;
        ++digit_array[digit];
    }

    for (i = 1; i < DIGITS_NUMBER; ++i)
    {
        digit_array[i] += digit_array[i - 1];
    }

    for (i = size - 1; (int)i >= 0; --i)
    {
        digit = (array[i] / exp) % EXP_BASE;
        output[--(digit_array[digit])] = array[i];
    }

    for (i = 0; i < size; ++i)
    {
        array[i] = output[i];
    }
}

/******************************Added 28.03.2023**********************************/


static ssize_t RecBinary(int arr[], size_t left_i, size_t right_i, int num)
{
    size_t mid = left_i + (right_i - left_i) / 2;

    if (left_i > right_i)
    {
        return -1;
    }

    if (arr[mid] == num)
    {
        return mid;
    }

    return arr[mid] > num ? RecBinary(arr, left_i, mid - 1, num) : RecBinary(arr, mid + 1, right_i, num);
}

static int RecMergeSort(int arr[], size_t left, size_t right)
{
    size_t mid = 0;
    int left_status = SUCCESS;
    int right_status = SUCCESS;
    int merge_status = SUCCESS;

    if (left < right)
    {
        mid = left + (right - left) / 2;
        left_status = RecMergeSort(arr, left, mid);
        right_status = RecMergeSort(arr, mid + 1, right);

        if (ERROR == left_status || ERROR == right_status)
        {
            return ERROR;
        }

        merge_status = Merge(arr, left, mid, right);

        if (ERROR == merge_status)
        {
            return ERROR;
        }
    }

    return SUCCESS;
}


static int Merge(int arr[], size_t left, size_t mid, size_t right)
{
    size_t left_size = mid - left + 1;
    size_t right_size = right - mid;
    size_t i = 0, j = 0, k = 0;

    int *left_arr = (int *)malloc(left_size * sizeof(int));
    int *right_arr = (int *)malloc(right_size * sizeof(int));

    if (NULL == left_arr || NULL == right_arr)
    {
        return ERROR;
    }

    for (i = 0; i < left_size; ++i)
    {
        left_arr[i] = arr[left + i];
    }

    for (i = 0; i < right_size; ++i)
    {
        right_arr[i] = arr[mid + 1 + i];
    }
    for (i = 0, j = 0, k = left; k <= right; ++k)
    {
        if ((i < left_size) && (j >= right_size || left_arr[i] <= right_arr[j]))
        {
            arr[k] = left_arr[i];
            ++i;
        }
        else
        {
            arr[k] = right_arr[j];
            ++j;
        }
    }

    free(left_arr);
    free(right_arr);

    return SUCCESS;
}


static void QsortRec(void *arr, size_t left_i, size_t right_i, size_t elm_size, size_t num_elms, int (*cmp_func)(const void *, const void *))
{
    size_t pivot_index = 0;

    if (left_i < right_i && right_i <= num_elms)
    {
        pivot_index = Partition(arr, left_i, right_i, elm_size, cmp_func);
        QsortRec(arr, left_i, pivot_index - 1, elm_size, num_elms, cmp_func);
        QsortRec(arr, pivot_index + 1, right_i, elm_size, num_elms, cmp_func);
    }
}

static size_t Partition(void *arr, size_t left_i, size_t right_i, size_t elm_size, int (*cmp_func)(const void *, const void *))
{
    size_t i = 0;
    size_t j = 0;
    void *a = NULL;
    void *b = NULL;
    void *pivot = NULL;

    size_t pivot_index = left_i + (rand() % (right_i - left_i));

    if (pivot_index != right_i)
    {
        Swap_Q(((char *)arr + (pivot_index * elm_size)), ((char *)arr + (right_i * elm_size)), elm_size);
    }

    pivot = (char *)arr + (right_i * elm_size);

    for (i = left_i, j = left_i; j < right_i; ++j)
    {
        a = (char *)arr + (j * elm_size);
        b = (char *)arr + (i * elm_size);

        if (0 < cmp_func(pivot, a))
        {
            Swap_Q(a, b, elm_size);
            ++i;
        }
    }

    b = (char *)arr + (i * elm_size);
    Swap_Q(b, pivot, elm_size);

    return i;
}

static void Swap_Q(void *a, void *b, size_t elm_size)
{
    char *runner_a = (char *)a;
    char *runner_b = (char *)b;
    char tmp[WORD_SIZE] = {0};

    while (elm_size >= WORD_SIZE)
    {
        memcpy(tmp, runner_a, WORD_SIZE);
        memcpy(runner_a, runner_b, WORD_SIZE);
        memcpy(runner_b, tmp, WORD_SIZE);

        elm_size -= WORD_SIZE;
        runner_a += WORD_SIZE;
        runner_b += WORD_SIZE;
    }

    memcpy(tmp, runner_a, elm_size);
    memcpy(runner_a, runner_b, elm_size);
    memcpy(runner_b, tmp, elm_size);
}





