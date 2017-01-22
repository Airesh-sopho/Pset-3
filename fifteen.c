/**
 * fifteen.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */
 
#define _XOPEN_SOURCE 500
 
#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
 
// constants
#define DIM_MIN 3
#define DIM_MAX 9
 
// board
int board[DIM_MAX][DIM_MAX];
 
// dimensions
int dimensions;
 
// row and column variables for the 'for' loops
 // row
int i;
 // column
int j;
 
// position of the blank space
int row_pos;
int col_pos;
 
// just a temporary variable
int temp;
 
// prototypes
void clear(void);
void greet(void);
void init(int dim);
void draw(int dim);
bool move(int tile, int dim);
bool won(int dim);
 
int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }
 
    // ensure valid dimensions
    dimensions = atoi(argv[1]);
    if (dimensions < DIM_MIN || dimensions > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }
 
    // open log
    FILE* file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }
 
    // greet user with instructions
    greet();
 
    // initialize the board
    init(dimensions);
 
    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();
 
        // draw the current state of the board
        draw(dimensions);
 
        // log the current state of the board (for testing)
        for (int i = 0; i < dimensions; i++)
        {
            for (int j = 0; j < dimensions; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < dimensions - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);
 
        // check for win
        if (won(dimensions))
        {
            printf("ftw!\n");
            break;
        }
 
        // prompt for move
        printf("Tile to move: ");
        int tile = GetInt();
        
        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }
 
        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);
 
        // move if possible, else report illegality
        if (!move(tile,dimensions))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }
 
        // sleep thread for animation's sake
        usleep(500000);
    }
    
    // close log
    fclose(file);
 
    // success
    return 0;
}
 
/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}
 
/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(2000000);
}
 
/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).  
 */
void init(int dim)
{
    int tally;
    tally = (dim * dim) - 1;
    for(i = 0; i < dim; i++)
    {
        for(j = 0; j < dim; j++)
        {
            board[i][j] = tally;
            tally--;
        }
    }
    if ((dim % 2) == 0)
    {
        board[dim - 1][dim - 2] = 2;
        board[dim - 1][dim - 3] = 1;
    }
    row_pos = dim - 1;
    col_pos = dim - 1;
}
 
/**
 * Prints the board in its current state.
 */
void draw(int dim)
{
    for(i = 0; i < dim; i++)
    {
        for(j = 0; j < dim; j++)
        {
            if (i == row_pos && j == col_pos)
            {
                printf(" _  ");
            }
            else
            {
                printf("%2d  ", board[i][j]);
            }
        }
        printf("\n");
    }
}
 
/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile, int dim)
{
    // temperory row and column position of the tile
    int temp_row; 
    int temp_col; 
    for(i = 0; i < dim; i++)
    {
        for(j = 0; j < dim; j++)
        {
            if (board[i][j] == tile)
            {
                temp_row = i;
                temp_col = j;
            }
        }
    }
    if (((row_pos + 1) == temp_row && (col_pos == temp_col)) || 
            ((row_pos - 1) == temp_row && (col_pos == temp_col)))
    {
        board[row_pos][col_pos] = tile;
        board[temp_row][temp_col] = 0;
        row_pos = temp_row;
        col_pos = temp_col;
        return true;
    }
    if (((row_pos) == temp_row && (col_pos == temp_col + 1)) || 
            ((row_pos) == temp_row && (col_pos == temp_col - 1)))
    {
        board[row_pos][col_pos] = tile;
        board[temp_row][temp_col] = 0;
        row_pos = temp_row;
        col_pos = temp_col;
        return true;
    }
    return false;
}
 
/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(int dim)
{
    temp = 1;
    for(i = 0; i < dim; i++)
    {
        for(j = 0; j < dim; j++)
        {
            if (i == dim - 1 && j == dim - 1 && board[i][j] == 0)
            {
                continue;
            }
            if (board[i][j] != temp)
            {
                return false;
            }
            temp++;
        }
    }
    return true;
}