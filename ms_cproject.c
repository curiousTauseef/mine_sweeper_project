/*********************************************************************************

1403724, WON SEOB SEO

G E N E R A L 	 I N F O R M A T I O N

All	games are text based games,	no need for	any	graphics

(of course other than ASCII	graphics, so for example
if you build minesweeper or	battleship game	it is not enough
to have	it completely text based, at least some	printouts
made with characters that represent	the	current	situation should be	used.)

No need	to build functionality to use mouse, so	player selecting
square from	minesweeper game can be	asked to input the square
by typing in for example "A1" and press	enter.

Project	must include: Design part (MS Visio	drawing	or something similar)
and	the	source code	that is	well commented and clear to	read.

Projects must include the source code and design documentation!

It is adamant that the code is well indented, there are clear comments
and code is divided to sections(such as if you are using functions
they are grouped to	a clearly understandable way for example at	the	end	of
the	source code	and	their location is clearly documented in	the	top	of the file).

Minesweeper PROJECT	TASK

Build a minesweeper	game (http://en.wikipedia.org/wiki/Minesweeper_%28video_game%29).
At least 10	x 10 grid and possibility to mark found mines. Test the game to
see how it works and do your best to implement the same functionality in your project.

**********************************************************************************/

/* Include files */
#include <stdio.h>
#include <stdlib.h> // The stdlib .h header defines four variable types, several macros and various
                    // functions for performing general functions.
#include <ctype.h>  // The ctype.h header file of the C Standard Library provides
                    // declares several functions useful for testing and mapping characters.
#include <time.h> // The time.h header defines four variable types, two macro and various functions
                  // for manipulating date and time.


/* Function prototypes */
void initial_message();
char set_difficulty();
void begin(char difficulty);
char* create_board(char difficulty);
char** create_gboard(char difficulty);
int set_size(char difficulty);
char** put_mines(char difficulty, char** board);
void print_fullboard(char difficulty, char** board);
void print_board(char difficulty, char** game_board);
void star_message();
int play_game(char difficulty, char** gboard, char** board, int lost);
void replay(char difficulty);
int check_win(char difficulty, char** game_board, char** board);
int check_mine(int x, int y, char** board);
int counthowmanymines(int x, int y, char difficulty, char** board);

// Main function
int main ()
{
    char difficulty;
    initial_message();
    difficulty = set_difficulty(); // set difficulty level
    // Get rid of welcome screen
    system("cls"); // system("cls"), sending "cls" command to the operating system to clear the screen
                    // Alternatively, clrscr();
    begin(difficulty); // starts the game
    return 0;

}

// Displays the initial_message, game difficulty option
void initial_message()
{
    printf("------------------------------------------------------------\n");
    printf("Minesweeper game! \nAuthor:Won Seob Seo\nStudent number:1403724\n");
    printf("Choose difficulty. 'n' for easy, 'h' for hard.\n");
    printf("------------------------------------------------------------\n");
    puts("\n\n");
}

/* Function to set the game difficulty */
char set_difficulty()
{
    char difficulty;
    do
    {
        printf("Select difficulty level(h for hard, n for normal>");
        difficulty = getchar();
        difficulty = tolower(difficulty);
        // tolower(difficulty);
            if ((difficulty == 'h') || (difficulty == 'n'))
            {
                return difficulty;
                break;
            }
    } while (1);
}

/* function to begin the game */
void begin(char difficulty)
{
    int lost = 0;    // User hasn't lost yet
    // create both the boards for the user to see,
    // and the one with the mines).
    char** board = create_board(difficulty);
    char** game_board = create_gboard(difficulty);

    // Start playing game
    do
    {
        play_game(difficulty, game_board, board, lost);
        print_board(difficulty, game_board);
    } while(lost != 1);    // While the user hasn't lost, loop.

    // Once user is lost, print the board with all the mines.
    print_fullboard(difficulty, board);

    // Play again?
    replay(difficulty);
}


/* Build board used for created random mines */
char** create_board(char difficulty)
{
    // set size according to the difficulty level and then create the board
    int size, i, j;
    size = set_size(difficulty);
    char board[size][size];

    if (difficulty == 'h')
    {
        for(i = 0; i < size; i++)
            for(j = 0; j < size; j++)
                board[i][j] = '@';
    }

    // Put mines in this board
    board = put_mines(difficulty, board);
    return board;
}

/* Build game board for user input */
char ** create_gboard(char difficulty)
{
    // set size according to the difficulty level
    int size, i, j;
    size = set_size(difficulty);
    int x, y, cur = 4;
    char game_board[size][size];
    //Draw board
    for(i = 0; i < size; i++)
        for(j = 0; j < size; j++)
            game_board[i][j] = 'o';

    //--------------------------------
    // Print board
    //--------------------------------
    printf("  ");
    for(x = 0; x < size; x++)
        printf("%d ", x + 1);

    printf("\n\n");

    for(y = 0; y < size; y++)
    {
        printf("%d \n", y + 1);
        for(x = 0; x < size; x++)
        {
            printf("%c ", game_board[y][x]);
        }
    }

    return game_board;
}

int set_size(char difficulty)
{
    int size;
    if (difficulty == 'h')
    {
        size = 15;
    }
    else if (difficulty == 'n')
    {
        size = 10;
    }
    return size;
}

/* Randomly create mines in the array*/
char ** put_mines(char difficulty, char ** board)
{
    int i, coordinate;
    time_t t;

    /* Intializes random number generator
       so that mine locations aren't the same every*/
    srand((unsigned) time(&t));
    int size = set_size(difficulty);

    for (i = 0; i < size; i++)
    {
        coordinate = rand() % (size);
        board[i][coordinate] = '*';
    }
    return board;
}

void print_board(char difficulty, char** game_board)
{
    int x, y;
    int size = set_size(difficulty);
    system("cls");
    for(x = 0; x < size; x++)
        printf("%d ", x + 1);

    printf("\n\n");
        for(y = 0; y < size; y++)
    {
        for(x = 0; x < size; x++)
        {
            printf("%c ", game_board[y][x]);
        }
        printf(" %d ", y + 1);
        printf("\n");
    }
}

/* Print the full board showing mines */
void print_fullboard(char difficulty, char** board)
{
    int y, x;
    int size = set_size(difficulty);

    system("cls");
    for(x = 0; x < size; x++)
        printf("%d ", x + 1);

    printf("\n\n");
        for(y = 0; y < size; y++)
    {
        for(x = 0; x < size; x++)
        {
            printf("%c ", board[y][x]);
        }
        printf(" %d ", y + 1);
        printf("\n");
    }
}

/* Take user input for playing of the game */
int play_game(char difficulty, char** game_board, char** board, int lost)
{
    int xpostion = 0, yposition = 0,
        nearbymines = 0, nearbymines2 = 0,
        nearbymines3 = 0, nearbymines4 = 0,
        nearbymines5 = 0, nearbymines6 = 0,
        nearbymines7 = 0, nearbymines8 = 0,
        i = 0;

    int size = set_size(difficulty);


    //----------------------------------------------
    // Recieves data from the user, first the row
    // number, then the column number...
    // I could think of other ways to do it, but
    // this one seemed easiest.
    //----------------------------------------------
    do {
    printf("\nMake a selection (ie. y [ENTER] x): \n");
    printf("y--> ");
    scanf("%d", &yposition);
    printf("x--> ");
    scanf("%d", &xpostion);

    } while(yposition < 1 || yposition > size || xpostion < 1 || yposition > size);
    // ^ Checks for any invalid input statements from user.

    check_mine(yposition - 1, xpostion - 1, board);

    if(lost == 1)
        return -1;

    // Checks for nearby mines at every direction from user
    // input location. Assigns that location the number
    // of mines found nearby, updating the board.
    nearbymines = counthowmanymines(yposition - 1, xpostion - 1, difficulty, board);
    game_board[yposition - 1][xpostion - 1] = (char)( ((int)'0') + nearbymines );

    //--------------------------------------------------
    // The following checks for mines nearby elements
    // in the array with no mines.. it's a continuous
    // loop until either a mine is found, or we
    // reach the end of the array & it cannot be checked
    // any further. It's big, clunky... but it works..
    //

    // It also changes the game_board[] with '0' if no
    // mines are found. Very useful piece of code.
    // It checks all elements left, right, up, down,
    // and all diagonal directions.. by running a function
    // that checks these same directions. A bit much
    // to follow, though. I'm sure there's a much better
    // way. I just don't know it yet.
    //---------------------------------------------------
    if(nearbymines == 0)
    {
        if(xpostion != size)
        {
            i = 0;
            while(nearbymines == 0 && (xpostion - 1 + i) < size)
            {
                // This is checking elements to the right
                nearbymines = counthowmanymines(yposition - 1, (xpostion - 1 + i), difficulty, board);
                if(nearbymines != -1)
                {
                game_board[yposition - 1][(xpostion - 1) + i] = (char) ( ((int)'0') + nearbymines );
                i++;
                }
            }
                if(yposition != 1)
                {
                    i = 0;
                    while(nearbymines5 == 0 && (xpostion - 1 + i) < size && (yposition - 1 - i) > 0)
                    {
                        // This is checking elements to the diagonal-uright
                        nearbymines5 = counthowmanymines((yposition - 1 - i), (xpostion - 1 + i), difficulty, board);
                        if(nearbymines5 != -1)
                        {
                        game_board[(yposition - 1) - i][(xpostion - 1) + i] = (char) ( ((int)'0') + nearbymines5);
                        i++;
                        }
                    }
                }
                if(yposition != size)
                {
                    i = 0;
                    while(nearbymines6 == 0 && (yposition - 1 + i) < size && (xpostion - 1 + i) < size )
                    {
                        // This is checking elements to the diagonal-dright
                        nearbymines6 = counthowmanymines((yposition - 1 + i), (xpostion - 1 + i), difficulty, board);
                        if(nearbymines != -1)
                        {
                        game_board[(yposition - 1) + i][(xpostion - 1) + i] = (char) ( ((int)'0') + nearbymines6);
                        i++;
                        }
                    }
                }
        }

        if(yposition != size)
        {
            i = 0;
            while(nearbymines2 == 0 && (yposition - 1 + i) < size)
            {
                // This is checking elements heading down
                nearbymines2 = counthowmanymines((yposition - 1 + i), xpostion - 1, difficulty, board);
                if(nearbymines2 != -1)
                {
                game_board[(yposition - 1) + i][xpostion - 1] = (char) ( ((int)'0') + nearbymines2 );
                i++;
                }
            }

            if(xpostion != size)
            {
                i = 0;
                while(nearbymines7 == 0 && (yposition - 1 + i) < size && (xpostion - 1 - i) > 0)
                {
                    // This is checking elements to the diagonal-dleft
                    nearbymines7 = counthowmanymines((yposition - 1 + i), (xpostion - 1 - i), difficulty, board);
                    if(nearbymines != -1)
                    {
                    game_board[(yposition - 1) + i][(xpostion - 1) - i] = (char) ( ((int)'0') + nearbymines7);
                    i++;
                    }
                }
            }
        }

        if(yposition != 1)
        {
            i = 0;
            while(nearbymines3 == 0 && (yposition - i) > 0)
            {
                // This is checking elements heading up
                nearbymines3 = counthowmanymines((yposition - 1 - i), xpostion - 1, difficulty, board);
                if(nearbymines3 != -1)
                {
                game_board[(yposition - 1) - i][xpostion - 1] = (char) ( ((int)'0') + nearbymines3 );
                i++;
                }
            }
            if(xpostion != size)
            {
                while(nearbymines8 == 0 && (xpostion - 1 - i) > 0 && (yposition - 1 - i) > 0)
                {
                    // This is checking elements to the diagonal-uleft
                    nearbymines8 = counthowmanymines((yposition - 1 - i), (xpostion - 1 - i), difficulty, board);
                    if(nearbymines8 != -1)
                    {
                    game_board[(yposition - 1) - i][(xpostion - 1) - i] = (char) ( ((int)'0') + nearbymines8);
                    i++;
                    }
                }
            }
        }

        if(xpostion != 1)
        {
            i = 0;
            while(nearbymines4 == 0 && (xpostion - i) > 0)
            {
                // This is checking elements to the left
                nearbymines4 = counthowmanymines(yposition - 1, (xpostion - 1 - i), difficulty, board);
                if(nearbymines4 != -1)
                {
                game_board[yposition - 1][(xpostion - 1) - i] = (char) ( ((int)'0') + nearbymines4 );
                i++;
                }
            }
        }
    }



    // Handles a player winning.
    if(check_win(difficulty, board, game_board) == TRUE)
    {
        system("cls");
        print_fullboard(difficulty, board);
        printf("\n\nYou've won the game!! Congrats!!\n\n");
        replay(difficulty);
    }

    return 0;
}

/* Check whether user input has selected a mine */
int check_mine(int x, int y, char** board)
{
    int lost = 0;
    if(board[x][y] == '*')
    {
        printf("\nBomb!!! You just hit a mine! You lose!\n");
        getchar();
        lost = 1;
    }
    return lost;
}

// A function to find nearby mines.

int counthowmanymines(int x, int y, char difficulty, char** board)
{
    int size = set_size(difficulty);
    int howmanymines = 0;

    if(board[x][y] == '*')
    {
        return -1;
    }

    if(x < (size - 1) && y < (size - 1))
    {
        // Is there a mine to the right?
        if(board[x + 1][y] == '*')
            howmanymines++;
        // Is there a mine below?
        if(board[x][y + 1] == '*')
            howmanymines++;
        // Is there a mine to the south-east?
        if(board[x + 1][y + 1] == '*')
            howmanymines++;

        // Check whether the columns to the left can be checked
        if(x != 0)
        {
            // Check for mine diagonal-dleft
            if(board[x + 1][y - 1] == '*')
                howmanymines++;
            // Check for mine to the left
            if(board[x][y - 1] == '*')
                howmanymines++;
        }
        // Check whether the rows above can be checked
        if(y != 0)
        {
            // Check for mine above
            if(board[y - 1][x] == '*')
                howmanymines++;
            // Check for mine diagonal-uright
            if(board[y - 1][x + 1] == '*')
                howmanymines++;
            // Check whether columns to the left can be checked
            if(x != 0)
            {
                // Check for mine diagonal-uleft
                if(board[y - 1][x - 1] == '*')
                    howmanymines++;
            }
        }
    }
    // Check if selection is in last row
    if(y == (size - 1) && x != (size - 1))
    {
        // Check for mine above
            if(board[y - 1][x] == '*')
                howmanymines++;
        // Check for mine diagonal-uright
            if(board[y - 1][x + 1] == '*')
                howmanymines++;
    }
    // Check if selection is in last column
    if(x == (size - 1) && y != (size - 1))
    {
        // Check for mine to the left
            if(board[y][x - 1] == '*')
                howmanymines++;
        // Check for mine diagonal-dleft
            if(board[y + 1][x - 1] == '*')
                howmanymines++;
    }
    // Check whether selection is last in element
    if(y == (size - 1) && x == (size - 1))
    {
        // Check for mine to the left
            if(board[y][x - 1] == '*')
                howmanymines++;
        // Check for mine diagonal-dleft
            if(board[y - 1][x - 1] == '*')
                howmanymines++;
        // Check for mine above
            if(board[y - 1][x] == '*')
                howmanymines++;
    }

    return howmanymines;
}

/* Check if user has won game */
int check_win(char difficulty, char** game_board, char** board)
{
    int y, x;
    int size = set_size(difficulty);
    for(y = 0; y < size; y++)
        for(x = 0; x < size; x++)
        {
            if(game_board[y][x] == 'o' && board[y][x] != '*')
                return FALSE;
        }

    return TRUE;
}
// Ask user if they wish to play again.
void replay(char difficulty)
{
    char ans;

    printf("\n\nWould you like to play again? (y/n) --> ");
    scanf(" %c", &ans);

    if((ans == 'Y') || (ans == 'y'))
    {
        system("cls");
        begin(difficulty);
    }

    else
    {
        printf("\n\nSee you again. I hope you enjoyed.");
        (void) getchar();
        exit(EXIT_SUCCESS);
    }
}
