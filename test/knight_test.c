#include <stdio.h>

#include "knight.h"


static void TestNaive();
static void TestWarnsdorff();
extern int KnightTourWarnsdorff(int matrix[SIZE_ROW][SIZE_COL], int row, int col);

int main()
{
    TestNaive();
    TestWarnsdorff();

    return 0;
}

static void TestNaive()
{
    int matrix[SIZE_ROW][SIZE_COL];
    int res = KnightTour(matrix, 4, 3);

    printf("\nNaive solution. Start point (4, 3), Matrix %d on %d\n", SIZE_ROW, SIZE_COL);
    res == 0 ? printf("Success\n") : printf("Failure\n");
    if (-1 == res)
    {
        printf("Out of time error\n");
    }
    PrintBoard(matrix);

    res = KnightTour(matrix, 0, 0);
    printf("Naive solution. Start point (0, 0), Matrix %d on %d\n", SIZE_ROW, SIZE_COL);
    res == 0 ? printf("Success\n") : printf("Failure\n");
    if (-1 == res)
    {
        printf("Out of time error\n");
    }
    PrintBoard(matrix);
}

static void TestWarnsdorff()
{
    int matrix[SIZE_ROW][SIZE_COL];
    int res = KnightTourWarnsdorff(matrix, 4, 3);

    printf("\nWarnsdorff algorithm.Start point (4, 3), Matrix %d on %d\n", SIZE_ROW, SIZE_COL);
    res == 0 ? printf("Success\n") : printf("Failure\n");
    PrintBoard(matrix);

    res = KnightTourWarnsdorff(matrix, 0, 0);
    printf("\nWarnsdorff algorithm. Start point (0, 0), Matrix %d on %d\n", SIZE_ROW, SIZE_COL);
    res == 0 ? printf("Success\n") : printf("Failure\n");
    PrintBoard(matrix);
}


void PrintBoard(int solution[SIZE_ROW][SIZE_COL])
{
    size_t i = 0;
    size_t j = 0;

    for (i = 0; i < SIZE_ROW; ++i)
    {
        for (j = 0; j < SIZE_COL; ++j)
        {
            printf("%.2d ", solution[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

