#include <time.h>
#include <string.h>

#include "knight.h"

#define NUM_OF_MOVES (8)
#define UNVISITED (-1)
#define TRUE (1)
#define LIMIT_TIME (10)
#define INVALID_DEGREE (9)

typedef enum status
{
    SUCCESS = 0,
    FAILURE = 1,
    OUT_OF_TIME = -1
} status_t;

static void InitMatrix(int matrix[SIZE_ROW][SIZE_COL]);
static void InitPossibleMovesArray(int x_move[], int y_move[]);
static int IsValidStep(int matrix[SIZE_ROW][SIZE_COL], int x, int y);
static status_t IsSolved(int solution_matrix[SIZE_ROW][SIZE_COL], int x, int y, int x_move[], int y_move[], time_t start_time);

int KnightTourWarnsdorff(int matrix[SIZE_ROW][SIZE_COL], int row, int col);
static int WarnsdorfRec(int matrix[SIZE_ROW][SIZE_COL], int row, int col, int row_moves[], int col_moves[]);
static void SortMoves(int matrix[SIZE_ROW][SIZE_COL], int sorted_moves[], int row, int col, int row_moves[], int col_moves[]);
static int GetMinIndex(int arr[]);

/****************************************User Functions****************************************/

int KnightTour(int solution_matrix[SIZE_ROW][SIZE_COL], int row, int col)
{
    int x_move[NUM_OF_MOVES] = {0};
    int y_move[NUM_OF_MOVES] = {0};
    time_t start_time = time(NULL);

    InitPossibleMovesArray(x_move, y_move);
    InitMatrix(solution_matrix);
    solution_matrix[row][col] = 1;

    return IsSolved(solution_matrix, row, col, x_move, y_move, start_time);
}


int KnightTourWarnsdorff(int matrix[SIZE_ROW][SIZE_COL], int row, int col)
{
    int row_moves[NUM_OF_MOVES] = {0};
    int col_moves[NUM_OF_MOVES] = {0};

    InitMatrix(matrix);
    InitPossibleMovesArray(row_moves, col_moves);
    matrix[row][col] = 1;
    
    return WarnsdorfRec(matrix, row, col, row_moves, col_moves);;
}

/***********************************Static Functions***********************************/

static int IsValidStep(int matrix[SIZE_ROW][SIZE_COL], int x, int y)
{
    return (x < SIZE_ROW && y < SIZE_COL && x >= 0 && y >= 0 && (UNVISITED == matrix[x][y]));
}


static status_t IsSolved(int solution_matrix[SIZE_ROW][SIZE_COL], int x, int y, int x_move[], int y_move[], time_t start_time)
{
    size_t i = 0;
    int next_x = 0;
    int next_y = 0;
    time_t curr_time = time(NULL);
    status_t exit_status = SUCCESS;
    int step_count = solution_matrix[x][y];

    if (difftime(curr_time, start_time) > LIMIT_TIME)
    {
        return OUT_OF_TIME;
    }

    if (step_count == SIZE_COL * SIZE_ROW)
    {
        return SUCCESS;
    }

    for (i = 0; i < NUM_OF_MOVES; ++i)
    {
        next_x = x + x_move[i];
        next_y = y + y_move[i];

        if (TRUE == IsValidStep(solution_matrix, next_x, next_y))
        {
            solution_matrix[next_x][next_y] = step_count + 1;
            exit_status = IsSolved(solution_matrix, next_x, next_y, x_move, y_move, start_time);

            if (SUCCESS == exit_status || OUT_OF_TIME == exit_status)
            {
                return exit_status;
            }

            solution_matrix[next_x][next_y] = UNVISITED;
        }
    }

    return FAILURE;
}

static int WarnsdorfRec(int matrix[SIZE_ROW][SIZE_COL], int row, int col, int row_moves[], int col_moves[])
{
    int next_row = 0;
    int next_col = 0;
    int sorted_moves[NUM_OF_MOVES] = {0};
    int prev_step = matrix[row][col];
    int i = 0;

    if (prev_step == SIZE_ROW * SIZE_COL)
    {
        return SUCCESS;
    }

    SortMoves(matrix, sorted_moves, row, col, row_moves, col_moves);

    for (i = 0; i < NUM_OF_MOVES; ++i)
    {
        next_row = row + row_moves[sorted_moves[i]];
        next_col = col + col_moves[sorted_moves[i]];

        if (TRUE == IsValidStep(matrix, next_row, next_col))
        {
            matrix[next_row][next_col] = prev_step + 1;

            if (SUCCESS == WarnsdorfRec(matrix, next_row, next_col, row_moves, col_moves))
            {
                return SUCCESS;
            }
            else
            {
                matrix[next_row][next_col] = UNVISITED;
            }
        }
    }

    return FAILURE;
}


static void SortMoves(int matrix[SIZE_ROW][SIZE_COL], int sorted_moves[], int row, int col, int row_moves[], int col_moves[])
{
    int degree[NUM_OF_MOVES] = {0};
    int min_degree_indx = 0;
    int next_x = 0;
    int next_y = 0;
    int i = 0;
    int j = 0;

    for (i = 0; i < NUM_OF_MOVES; ++i)
    {
        next_x = row + row_moves[i];
        next_y = col + col_moves[i];

        if (TRUE == IsValidStep(matrix, next_x, next_y))
        {
            for (j = 0; j < NUM_OF_MOVES; ++j)
            {
                degree[i] += IsValidStep(matrix, next_x + row_moves[j], next_y + col_moves[j]);
            }
        }
    }

    for (i = 0; i < NUM_OF_MOVES; ++i)
    {
        min_degree_indx = GetMinIndex(degree);
        sorted_moves[i] = min_degree_indx;
        degree[min_degree_indx] = INVALID_DEGREE;
    }
}

static int GetMinIndex(int arr[])
{
    size_t i = 0;
    size_t min_index = 0;

    for (i = 0; i < NUM_OF_MOVES; ++i)
    {
        if (arr[i] < arr[min_index])
        {
            min_index = i;
        }
    }
    return min_index;
}



/*******************************Init Functions***********************************/

static void InitMatrix(int matrix[SIZE_ROW][SIZE_COL])
{
    size_t i = 0;
    size_t j = 0;
    for (i = 0; i < SIZE_ROW; ++i)
    {
        for (j = 0; j < SIZE_COL; ++j)
        {
            matrix[i][j] = UNVISITED;
        }
    }
}

static void InitPossibleMovesArray(int x_move[], int y_move[])
{
    int x_arr[NUM_OF_MOVES] = {2, 1, -1, -2, -2, -1, 1, 2};
    int y_arr[NUM_OF_MOVES] = {1, 2, 2, 1, -1, -2, -2, -1};

    memcpy(x_move, x_arr, sizeof(int) * NUM_OF_MOVES);
    memcpy(y_move, y_arr, sizeof(int) * NUM_OF_MOVES);
}




