
#include <assert.h>
#include <stdio.h>  /*printf*/
#include <time.h>
#include <stdlib.h> /*rand*/

#include "sort.h"

#define ARRAY_SIZE (5000)
#define MAX_NUMBER (1000)
#define MIN_NUMBER (0)

static void TestBubbleSort(void);
static void TestSelectionSort(void);
static void TestInsertionSort(void);
static void TestCounting(void);
static void TestRadix(void);
static void TestQSort(void);

static void FillArray(int ar[], size_t size, int min, int max);
static int IsSortedArray(int ar[], size_t size);
static int QSortCompare(const void *a, const void *b);
static int CmpNumbers(const int a, const int b);

/*****Added 28.03.2023*********/

static void TestBinary(void);
static void TestMerge(void);
static void TestMyQsort(void);

int main()
{
    srand(time(NULL));

    TestBubbleSort();
    TestSelectionSort();
    TestInsertionSort();
    
    TestCounting();
    TestRadix();
    
    TestQSort();
    
    /*****Added 28.03.2023*********/
    
    TestBinary();
    TestMerge();
    TestMyQsort();


    printf("all tests have been passed\n");

    return 0;
}

/*********************************Test Functions***************************/

static void TestQSort()
{
    int ar[ARRAY_SIZE] = {0};
    clock_t start1 = 0, end1 = 0;

    FillArray(ar, ARRAY_SIZE, MIN_NUMBER, MAX_NUMBER);

    start1 = clock();
    qsort(ar, ARRAY_SIZE, sizeof(int), QSortCompare);
    end1 = clock();

    assert(0 == IsSortedArray(ar, ARRAY_SIZE));

    printf("Elapsed time for library qsort : %f seconds\n", ((double)(end1 - start1)) / CLOCKS_PER_SEC);
}

static void TestBubbleSort()
{
    int ar[ARRAY_SIZE] = {0};
    clock_t start1 = 0, end1 = 0;

    FillArray(ar, ARRAY_SIZE, MIN_NUMBER, MAX_NUMBER);

    start1 = clock();
    BubbleSort(ar, ARRAY_SIZE, CmpNumbers);
    end1 = clock();

    assert(0 == IsSortedArray(ar, ARRAY_SIZE));

    printf("Elapsed time for bubble sort: %f seconds\n", ((double)(end1 - start1)) / CLOCKS_PER_SEC);
}

static void TestSelectionSort()
{
    int ar[ARRAY_SIZE] = {0};
    clock_t start1 = 0, end1 = 0;

    FillArray(ar, ARRAY_SIZE, MIN_NUMBER, MAX_NUMBER);

    start1 = clock();
    SelectionSort(ar, ARRAY_SIZE, CmpNumbers);
    end1 = clock();

    assert(0 == IsSortedArray(ar, ARRAY_SIZE));

    printf("Elapsed time for Selection sort: %f seconds\n", ((double)(end1 - start1)) / CLOCKS_PER_SEC);
}

static void TestInsertionSort()
{
    int ar[ARRAY_SIZE] = {0};
    clock_t start1 = 0, end1 = 0;

    FillArray(ar, ARRAY_SIZE, MIN_NUMBER, MAX_NUMBER);
    start1 = clock();
    InsertionSort(ar, ARRAY_SIZE, CmpNumbers);
    end1 = clock();

    assert(0 == IsSortedArray(ar, ARRAY_SIZE));

    printf("Elapsed time for Insertion sort: %f seconds\n", ((double)(end1 - start1)) / CLOCKS_PER_SEC);
}

static void TestCounting()
{
    int ar[ARRAY_SIZE] = {0};
    int ar_negative[ARRAY_SIZE] = {0};
    clock_t start1 = 0, end1 = 0;

    FillArray(ar, ARRAY_SIZE, MIN_NUMBER, MAX_NUMBER);
    FillArray(ar_negative, ARRAY_SIZE, -1000, MAX_NUMBER);
    
    start1 = clock();
    assert(0 == CountingSort(ar, ARRAY_SIZE));
    end1 = clock();
    
    assert(0 == CountingSort(ar_negative, ARRAY_SIZE));

    assert(0 == IsSortedArray(ar, ARRAY_SIZE));
    assert(0 == IsSortedArray(ar_negative, ARRAY_SIZE));
    
    printf("Elapsed time for Counting sort: %f seconds\n", ((double)(end1 - start1)) / CLOCKS_PER_SEC);
}

static void TestRadix()
{
    int ar[ARRAY_SIZE] = {0};
    clock_t start1 = 0, end1 = 0;
    
    FillArray(ar, ARRAY_SIZE, MIN_NUMBER, MAX_NUMBER);
    
    start1 = clock();
    assert(0 == RadixSort(ar, ARRAY_SIZE));
    end1 = clock();
    
    assert(0 == IsSortedArray(ar, ARRAY_SIZE));
    printf("Elapsed time for Radix sort: %f seconds\n", ((double)(end1 - start1)) / CLOCKS_PER_SEC);
}

static void TestBinary(void)
{
    int arr[] = {10, 20, 30, 40, 50, 60, 70, 80, 90};
    size_t size = sizeof(arr) / sizeof(int);

    assert(3 == BinarySearchRecursive(arr, size, 40));
    assert(3 == BinarySearchIterative(arr, size, 40));
    assert(-1 == BinarySearchRecursive(arr, size, 55));
    assert(-1 == BinarySearchIterative(arr, size, 55));
}

static void TestMerge(void)
{
    int arr[ARRAY_SIZE] = {0};
    size_t size = sizeof(arr) / sizeof(int);
    clock_t start1 = 0, end1 = 0;
    FillArray(arr, ARRAY_SIZE, MIN_NUMBER, MAX_NUMBER);
    
    start1 = clock();
    assert(0 == MergeSort(arr, size));
    end1 = clock();
    
    assert(0 == IsSortedArray(arr, ARRAY_SIZE));
    printf("Elapsed time for MergeSort: %f seconds\n", ((double)(end1 - start1)) / CLOCKS_PER_SEC);
}

static void TestMyQsort(void)
{
    int ar[ARRAY_SIZE] = {0};
    clock_t start1 = 0, end1 = 0;
    
    FillArray(ar, ARRAY_SIZE, MIN_NUMBER, MAX_NUMBER);
    
    start1 = clock();
    Qsort(ar, ARRAY_SIZE, sizeof(int), QSortCompare);
    end1 = clock();
    
    assert(0 == IsSortedArray(ar, ARRAY_SIZE));
    printf("Elapsed time for My Qsort: %f seconds\n", ((double)(end1 - start1)) / CLOCKS_PER_SEC);
}

/**********************Static Common functions*****************************************/

static void FillArray(int ar[], size_t size, int min, int max)
{
    size_t i = 0;

    for (i = 0; i < size; ++i)
    {
        ar[i] = (rand() % (max - min + 1)) + min;
    }
}

static int CmpNumbers(const int a, const int b)
{
    return a - b;
}

static int IsSortedArray(int ar[], size_t size)
{
    size_t i = 0;
    int isError = 0;

    for (i = 0; i < size - 1; i++)
    {
        if (ar[i] > ar[i + 1])
        {
            isError = 1;
            break;
        }
    }

    return isError;
}

static int QSortCompare(const void *a, const void *b)
{
    return *(int *)a - *(int *)b;
}


