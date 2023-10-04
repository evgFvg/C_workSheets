#include <time.h> 
#include <string.h> /*memcpy*/

#include "knight.h"

#define NUM_OF_MOVES (8)
#define UNVISITED (-1)
#define LIMIT_TIME (10)

typedef enum status
{
    SUCCESS = 0,
    FAILURE = 1,
    OUT_OF_TIME = -1
} status_t;


static void InitMatrix(int matrix[SIZE_ROW][SIZE_COL]);
static void InitPossibleMovesArray(int x_move[], int y_move[]);
static status_t IsValidStep(int matrix[SIZE_ROW][SIZE_COL], int x, int y);
static status_t IsSolved(int solution_matrix[SIZE_ROW][SIZE_COL], int step_count, int x, int y, int x_move[], int y_move[], time_t start_time);

int KnightTourWarnsdorff(int matrix[SIZE_ROW][SIZE_COL], int row, int col);
static int GetDegree(int matrix[SIZE_ROW][SIZE_COL], int row, int col, int *row_moves, int *col_moves);

/***********************************Naive Solution****************************************/

int KnightTour(int solution_matrix[SIZE_ROW][SIZE_COL], int row, int col)
{

    int x_move[NUM_OF_MOVES] = {0};
    int y_move[NUM_OF_MOVES] = {0};
    time_t start_time = time(NULL);
    
    InitMatrix(solution_matrix);
    InitPossibleMovesArray(x_move, y_move);

    return IsSolved(solution_matrix, 1, row, col, x_move, y_move, start_time);
}

/*****************************Warnsdorff's solution************************************/


int KnightTourWarnsdorff(int matrix[SIZE_ROW][SIZE_COL], int row, int col)
{
    int row_moves[NUM_OF_MOVES] = {2, 1, -1, -2, -2, -1, 1, 2};
    int col_moves[NUM_OF_MOVES] = {1, 2, 2, 1, -1, -2, -2, -1};
    int degree_arr[NUM_OF_MOVES] = {0};
    int min_degree = 0;
    int min_degree_index = 0;
    int count_moves = 0;
    int next_row = 0;
    int next_col = 0;
    int i = 0;
    int j = 0;

    InitMatrix(matrix);
    matrix[row][col] = count_moves;
    ++count_moves;

    for (i = 0; i < SIZE_ROW * SIZE_COL - 1; ++i)
    {
        min_degree = NUM_OF_MOVES + 1;
        min_degree_index = -1;

        for (j = 0; j < NUM_OF_MOVES; ++j)
        {
            next_row = row + row_moves[j];
            next_col = col + col_moves[j];

            if (SUCCESS == IsValidStep(matrix, next_row, next_col))
            {
                degree_arr[j] = GetDegree(matrix, next_row, next_col, row_moves, col_moves);

                if (min_degree > degree_arr[j])
                {
                    min_degree = degree_arr[j];
                    min_degree_index = j;
                }
            }
        }

        if (-1 == min_degree_index) /*No move is valid*/
        {
            return FAILURE;
        }

        row += row_moves[min_degree_index];
        col += col_moves[min_degree_index];
        matrix[row][col] = count_moves;
        ++count_moves;
    }

    return SUCCESS;
}

/*****************************************Static Functions*******************************************/

static status_t IsValidStep(int matrix[SIZE_ROW][SIZE_COL], int x, int y)
{
    if (x < SIZE_ROW && y < SIZE_COL && x >= 0 && y >= 0 && (UNVISITED == matrix[x][y]))
    {
        return SUCCESS;
    }

    return FAILURE;
}

static int GetDegree(int matrix[SIZE_ROW][SIZE_COL], int row, int col, int *row_moves, int *col_moves)
{
    int i = 0;
    int degree_count = 0;

    for (i = 0; i < NUM_OF_MOVES; ++i)
    {
        if (SUCCESS == IsValidStep(matrix, row + row_moves[i], col + col_moves[i]))
        {
            ++degree_count;
        }
    }

    return degree_count;
}

static status_t IsSolved(int solution_matrix[SIZE_ROW][SIZE_COL], int step_count, int x, int y, int x_move[], int y_move[], time_t start_time)
{
    int i = 0;
    int next_x = 0;
    int next_y = 0;
    time_t curr_time = time(NULL);
    status_t exit_status  = SUCCESS;
    
    if (difftime(curr_time, start_time) > LIMIT_TIME)
    {
        return OUT_OF_TIME;
    }

    if (SUCCESS == IsValidStep(solution_matrix, x, y))
    {
        solution_matrix[x][y] = step_count;

        if (step_count == SIZE_COL * SIZE_ROW)
        {
            return SUCCESS;
        }

        for (i = 0; i < NUM_OF_MOVES; ++i)
        {
            next_x = x + x_move[i];
            next_y = y + y_move[i];
            exit_status =  IsSolved(solution_matrix, step_count + 1, next_x, next_y, x_move, y_move, start_time);

            if (SUCCESS == exit_status || OUT_OF_TIME == exit_status)
            {
                return exit_status;
            }
        }

        solution_matrix[x][y] = UNVISITED;
    }

    return FAILURE;
}


static void InitMatrix(int matrix[SIZE_ROW][SIZE_COL])
{
    int i = 0;
    int j = 0;
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

