/**
 * fifteen.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Implements the Game of Fifteen (generalized to d x d).
 *
 * Usage: ./fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [MIN,MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */
 
#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// board's minimal dimension
#define MIN 3

// board's maximal dimension
#define MAX 9

// board, whereby board[i][j] represents row i and column j
int board[MAX][MAX];

// board's dimension
int d;
int dim;

// empty tile
int ePos = 0;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);
void save(void);

int main(int argc, string argv[])
{
    // greet player
    greet();

    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: ./fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < MIN || d > MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            MIN, MIN, MAX, MAX);
        return 2;
    }

    // initialize the board
    init();
    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // saves the current state of the board (for testing)
        save();

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = GetInt();

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep for animation's sake
        usleep(500000);
    }

    // that's all folks
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
    printf("GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1,
 * (i.e., fills board with values but does not actually print them),
 * whereby board[i][j] represents row i and column j.
 */
void init(void)
{
    //initialize tiles
    dim = d*d - 1;
    ePos = dim;
    for (int i = 0, k = dim; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            if ( dim % 2 == 1 && k == 1 )
            {
                board[i][j] = 2;
                k--;
                continue;
            } 
            else if ( dim % 2 == 1 && k == 2 )
            {
                board[i][j] = 1;
                k--;
                continue;
            }
            board[i][j] = k;
            k--;
        }
    }

    /*for (int i = 0; i < d; i++)
        for (int j = 0; j < d; j++)
            printf ("%d ", board[i][j]);
    */
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    //creating the board
    printf ("\n");
    for (int i = 0; i < d; i++)
    {
        printf ("  ");
        for (int j = 0; j < d; j++)
        {
            if (board[i][j] == 0)
            {
                printf (" %c ", '_');
                continue;
            }
            if (board[i][j]/10 == 0)
                printf (" %d ", board[i][j]);
            else    
                printf ("%d ", board[i][j]);
        }
        printf ("\n\n");
    }
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile)
{
    // make legal move
    int eC = ePos % d, eR = (ePos - eC)/d;
    for (int mR =0; mR < d; mR++)
    {
        for (int mC = 0; mC < d; mC++)
        {
            if (tile == board[mR][mC])
            {
                int mid = mR*d + mC;
                if (ePos == mid - 1 || ePos == mid + 1 || ePos == mid + d || ePos == mid - d)
                {
                    board[mR][mC] = board[eR][eC] + board[mR][mC];
                    board[eR][eC] = board[mR][mC] - board[eR][eC];
                    board[mR][mC] = board[mR][mC] - board[eR][eC];
                    ePos = mid;
                    return true;
                }
                else
                    return false;
            }
        }
    }
    /*while (start <= end)
    int start = 0, end = dim; 
    {
        int mid = dim % 2 ? (start + end + 1)/2 : (start + end)/2;
        int mC = mid % d, mR = (mid - mC)/d;
        if (tile == board[mR][mC])
        {
            if (ePos == mid - 1 || ePos == mid + 1 || ePos == mid + dim || ePos == mid - dim)
            {
                board[mR][mC] = board[eR][eC] + board[mR][mC];
                board[eR][eC] = board[mR][mC] - board[eR][eC];
                board[mR][mC] = board[mR][mC] - board[eR][eC];
                return true;
            }
            else
                return false;
        }
        if (tile > board[mR][mC])
            start = mid + 1;
        else 
            end = mid - 1;
    }*/

    return false;
}

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(void)
{
    // check with iterator
    for (int i = 0, k = 1; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            if (board[i][j] != k && k != dim + 1)
                return false;
            k++;
        }
    }
    return true;
}

/**
 * Saves the current state of the board to disk (for testing).
 */
void save(void)
{
    // log
    const string log = "log.txt";

    // delete existing log, if any, before first save
    static bool saved = false;
    if (!saved)
    {
        unlink(log);
        saved = true;
    }

    // open log
    FILE* p = fopen(log, "a");
    if (p == NULL)
    {
        return;
    }

    // log board
    fprintf(p, "{");
    for (int i = 0; i < d; i++)
    {
        fprintf(p, "{");
        for (int j = 0; j < d; j++)
        {
            fprintf(p, "%i", board[i][j]);
            if (j < d - 1)
            {
                fprintf(p, ",");
            }
        }
        fprintf(p, "}");
        if (i < d - 1)
        {
            fprintf(p, ",");
        }
    }
    fprintf(p, "}\n");

    // close log
    fclose(p);
}
