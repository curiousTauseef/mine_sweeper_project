/*********************************************************************************

1403724, WON SEOB SEO
    To-do list:
    1. Create a function where user can select levels {easy, medium, hard}

    2. Create a better procedure for finding nearby mines,
        preferably, one less clunky and difficult to read and follow,
        and debug, and ... just about everything else.. something
        simple, yet, efficient. Elegant.

G E N E R A L 	 I N F O R M A T I O N

All	games are text based games,	no need for	any	graphics

(of course other than ASCII	graphics, so for example
if you build minesweeper or	battleship game	it is not enough
to have	it completely text based, at least some	printouts
made with characters that represent	the	current	situation should be	used.)

No need	to build functionality to use mouse, so	player selecting
square from	minesweeper game can be	asked to input the square
by typing in for example ¡°A1¡± and press	enter.

Project	must include: Design part (MS Visio	drawing	or something similar)
and	the	source code	that is	well commented and clear to	read.

Completed projects must be returned to tuubi!

Projects must include the source code and design documentation!

Deadline is	6th of May 2015.

It is adamant	that the code is well indented, there are clear	comments and code is divided to	sections
(such as if	you	are	using functions	they are grouped to	a clearly understandable way for example at	the	end	of
the	source code	and	their location is clearly documented in	the	top	of the file).

Minesweeper PROJECT	TASK

Build a minesweeper	game (http://en.wikipedia.org/wiki/Minesweeper_%28video_game%29).
At least 10	x 10 grid and possibility to mark found mines. Test the game to see how it works and do
your best to implement the same functionality in your project.

**********************************************************************************/

/* Include files */
#include <stdio.h>
#include <stdlib.h> // The stdlib .h header defines four variable types, several macros and various
                    // functions for performing general functions.
#include <ctype.h>  // The ctype.h header file of the C Standard Library provides
                    // declares several functions useful for testing and mapping characters.
#include <time.h> // The time.h header defines four variable types, two macro and various functions
                  // for manipulating date and time.

/* Definitions */
#define BOARD_SIZE 6
#define TRUE 0
#define FALSE 1

/* Global Variables */
char board[BOARD_SIZE][BOARD_SIZE];
char game_board[BOARD_SIZE][BOARD_SIZE];
int lost = 0;

/* Function prototypes */
void star_message();
void create_board();
void create_gboard();
void put_mines();
void print_board();
void print_fullboard();
void begin();
int play();
void replay();
int check_win();
void check_mine(int, int);
int check_near_mines(int, int);

// Main function
int main ()
{
    display_welcome();
    printf("\nAny time you're ready... Just press ENTER. :)");

    // Get rid of welcome screen
    getchar();
    system("cls"); // sending "cls" command to the operating system to clear the screen
                    // Alternatively, clrscr();

    begin();

    return 0;

}

/* Build board used for created random mines */
void create_board()
{

    int i, j;

    //--------------------------------
    // Assign char for board elements
    //--------------------------------
    for(i = 0; i < BOARD_SIZE; i++)
        for(j = 0; j < BOARD_SIZE; j++)
            board[i][j] = 'o';

    // Put mines in this board
    put_mines();
}

/* Build game board for user input */
void build_gboard()
{

    int i, j;
    int row, col, cur = 4;
    //----------------------------------------
    // Assign char 'o' for all board elements
    //----------------------------------------
    for(i = 0; i < BOARD_SIZE; i++)
        for(j = 0; j < BOARD_SIZE; j++)
            game_board[i][j] = 'o';

    //--------------------------------
    // Print board
    //--------------------------------
    printf("  ")
    for(col = 0; col < BOARD_SIZE; col++)
        printf("%d ", col + 1);

    printf("\n\n");

    for(row = 0; row < BOARD_SIZE; row++)
    {
        printf("%d \n", row + 1);
        for(col = 0; col < BOARD_SIZE; col++)
        {
            printf("%c ", game_board[row][col]);
        }
    }
}

/* Randomly create mines in the array*/
void put_mines()
{
    int i, random;
    time_t t;

    /* Intializes random number generator
       so that mine locations aren't the same every*/
    srand((unsigned) time(&t));


    for (i = 0; i < BOARD_SIZE; i++)
    {
        coordinate = rand() % (BOARD_SIZE);
        board[coordinate][coordinate] = '*';
    }

}

/* Print the game board */
void print_board()
{
    int row, col;

    system("cls");
    for(col = 0; col < BOARD_SIZE; col++)
        printf("%d ", col + 1);

    printf("\n\n");
        for(row = 0; row < BOARD_SIZE; row++)
    {
        for(col = 0; col < BOARD_SIZE; col++)
        {
            printf("%c ", game_board[row][col]);
        }
        printf(" %d ", row + 1);
        printf("\n");
    }
}

/* Print the full board showing mines */
void print_fullboard()
{
    int row, col;

    system("cls");
    for(col = 0; col < BOARD_SIZE; col++)
        printf("%d ", col + 1);

    printf("\n\n");
        for(row = 0; row < BOARD_SIZE; row++)
    {
        for(col = 0; col < BOARD_SIZE; col++)
        {
            printf("%c ", board[row][col]);
        }
        printf(" %d ", row + 1);
        printf("\n");
    }
}

/* Take user input for playing of the game */
int play_game()
{
    int r_selection = 0, c_selection = 0,
        nearbymines = 0, nearbymines2 = 0,
        nearbymines3 = 0, nearbymines4 = 0,
        nearbymines5 = 0, nearbymines6 = 0,
        nearbymines7 = 0, nearbymines8 = 0,
        i = 0;

    //----------------------------------------------
    // Recieves data from the user, first the row
    // number, then the column number...
    // I could think of other ways to do it, but
    // this one seemed easiest.
    //----------------------------------------------
    do {
    printf("\nMake a selection (ie. row [ENTER] col): \n");
    printf("Row--> ");
    scanf("%d", &r_selection);
    printf("Col--> ");
    scanf("%d", &c_selection);

    } while(r_selection < 1 || r_selection > BOARD_SIZE || c_selection < 1 || r_selection > BOARD_SIZE);
    // ^ Checks for any invalid input statements from user.

    check_for_mine(r_selection - 1, c_selection - 1);

    if(lost == 1)
        return -1;

    // Checks for nearby mines at every direction from user
    // input location. Assigns that location the number
    // of mines found nearby, updating the board.
    nearbymines = check_for_nearby_mines(r_selection - 1, c_selection - 1);
    game_board[r_selection - 1][c_selection - 1] = (char)( ((int)'0') + nearbymines );

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
        if(c_selection != BOARD_SIZE)
        {
            i = 0;
            while(nearbymines == 0 && (c_selection - 1 + i) < BOARD_SIZE)
            {
                // This is checking elements to the right
                nearbymines = check_for_nearby_mines(r_selection - 1, (c_selection - 1 + i));
                if(nearbymines != -1)
                {
                game_board[r_selection - 1][(c_selection - 1) + i] = (char) ( ((int)'0') + nearbymines );
                i++;
                }
            }
                if(r_selection != 1)
                {
                    i = 0;
                    while(nearbymines5 == 0 && (c_selection - 1 + i) < BOARD_SIZE && (r_selection - 1 - i) > 0)
                    {
                        // This is checking elements to the diagonal-uright
                        nearbymines5 = check_for_nearby_mines((r_selection - 1 - i), (c_selection - 1 + i));
                        if(nearbymines5 != -1)
                        {
                        game_board[(r_selection - 1) - i][(c_selection - 1) + i] = (char) ( ((int)'0') + nearbymines5);
                        i++;
                        }
                    }
                }
                if(r_selection != BOARD_SIZE)
                {
                    i = 0;
                    while(nearbymines6 == 0 && (r_selection - 1 + i) < BOARD_SIZE && (c_selection - 1 + i) < BOARD_SIZE )
                    {
                        // This is checking elements to the diagonal-dright
                        nearbymines6 = check_for_nearby_mines((r_selection - 1 + i), (c_selection - 1 + i));
                        if(nearbymines != -1)
                        {
                        game_board[(r_selection - 1) + i][(c_selection - 1) + i] = (char) ( ((int)'0') + nearbymines6);
                        i++;
                        }
                    }
                }
        }

        if(r_selection != BOARD_SIZE)
        {
            i = 0;
            while(nearbymines2 == 0 && (r_selection - 1 + i) < BOARD_SIZE)
            {
                // This is checking elements heading down
                nearbymines2 = check_for_nearby_mines((r_selection - 1 + i), c_selection - 1);
                if(nearbymines2 != -1)
                {
                game_board[(r_selection - 1) + i][c_selection - 1] = (char) ( ((int)'0') + nearbymines2 );
                i++;
                }
            }

            if(c_selection != BOARD_SIZE)
            {
                i = 0;
                while(nearbymines7 == 0 && (r_selection - 1 + i) < BOARD_SIZE && (c_selection - 1 - i) > 0)
                {
                    // This is checking elements to the diagonal-dleft
                    nearbymines7 = check_for_nearby_mines((r_selection - 1 + i), (c_selection - 1 - i));
                    if(nearbymines != -1)
                    {
                    game_board[(r_selection - 1) + i][(c_selection - 1) - i] = (char) ( ((int)'0') + nearbymines7);
                    i++;
                    }
                }
            }
        }

        if(r_selection != 1)
        {
            i = 0;
            while(nearbymines3 == 0 && (r_selection - i) > 0)
            {
                // This is checking elements heading up
                nearbymines3 = check_for_nearby_mines((r_selection - 1 - i), c_selection - 1);
                if(nearbymines3 != -1)
                {
                game_board[(r_selection - 1) - i][c_selection - 1] = (char) ( ((int)'0') + nearbymines3 );
                i++;
                }
            }
            if(c_selection != BOARD_SIZE)
            {
                while(nearbymines8 == 0 && (c_selection - 1 - i) > 0 && (r_selection - 1 - i) > 0)
                {
                    // This is checking elements to the diagonal-uleft
                    nearbymines8 = check_for_nearby_mines((r_selection - 1 - i), (c_selection - 1 - i));
                    if(nearbymines8 != -1)
                    {
                    game_board[(r_selection - 1) - i][(c_selection - 1) - i] = (char) ( ((int)'0') + nearbymines8);
                    i++;
                    }
                }
            }
        }

        if(c_selection != 1)
        {
            i = 0;
            while(nearbymines4 == 0 && (c_selection - i) > 0)
            {
                // This is checking elements to the left
                nearbymines4 = check_for_nearby_mines(r_selection - 1, (c_selection - 1 - i));
                if(nearbymines4 != -1)
                {
                game_board[r_selection - 1][(c_selection - 1) - i] = (char) ( ((int)'0') + nearbymines4 );
                i++;
                }
            }
        }
    }



    // Handles a player winning.
    if(check_win_game() == TRUE)
    {
        system("cls");
        print_fullboard();
        printf("\n\nYou've won the game!! Congrats!!\n\n");
        play_again();
    }

    return 0;
}

/* Check whether user input has selected a mine */
void check_for_mine(int r_select, int c_select)
{
    if(board[r_select][c_select] == '*')
    {
        printf("\nYou've hit a mine! You lose!\n");
        getchar(); getchar();
        lost = 1;
    }
}

/* Another ridiculous function to find nearby mines.
 * I know, I know...it's messy, and needs a rewrite. */
int check_for_nearby_mines(int r_select, int c_select)
{
    int nearby_mine_count = 0;

    if(board[r_select][c_select] == '*')
    {
        return -1;
    }
    // Check for mines below and to the right.
    if(r_select < (BOARD_SIZE - 1) && c_select < (BOARD_SIZE - 1))
    {
        // Check for mine below
        if(board[r_select + 1][c_select] == '*')
            nearby_mine_count++;
        // Check for mine to the right.
        if(board[r_select][c_select + 1] == '*')
            nearby_mine_count++;
        // Check for mine diagonal-dright.
        if(board[r_select + 1][c_select + 1] == '*')
            nearby_mine_count++;

        // Check whether the columns to the left can be checked
        if(c_select != 0)
        {
            // Check for mine diagonal-dleft
            if(board[r_select + 1][c_select - 1] == '*')
                nearby_mine_count++;
            // Check for mine to the left
            if(board[r_select][c_select - 1] == '*')
                nearby_mine_count++;
        }
        // Check whether the rows above can be checked
        if(r_select != 0)
        {
            // Check for mine above
            if(board[r_select - 1][c_select] == '*')
                nearby_mine_count++;
            // Check for mine diagonal-uright
            if(board[r_select - 1][c_select + 1] == '*')
                nearby_mine_count++;
            // Check whether columns to the left can be checked
            if(c_select != 0)
            {
                // Check for mine diagonal-uleft
                if(board[r_select - 1][c_select - 1] == '*')
                    nearby_mine_count++;
            }
        }
    }
    // Check if selection is in last row
    if(r_select == (BOARD_SIZE - 1) && c_select != (BOARD_SIZE - 1))
    {
        // Check for mine above
            if(board[r_select - 1][c_select] == '*')
                nearby_mine_count++;
        // Check for mine diagonal-uright
            if(board[r_select - 1][c_select + 1] == '*')
                nearby_mine_count++;
    }
    // Check if selection is in last column
    if(c_select == (BOARD_SIZE - 1) && r_select != (BOARD_SIZE - 1))
    {
        // Check for mine to the left
            if(board[r_select][c_select - 1] == '*')
                nearby_mine_count++;
        // Check for mine diagonal-dleft
            if(board[r_select + 1][c_select - 1] == '*')
                nearby_mine_count++;
    }
    // Check whether selection is last in element
    if(r_select == (BOARD_SIZE - 1) && c_select == (BOARD_SIZE - 1))
    {
        // Check for mine to the left
            if(board[r_select][c_select - 1] == '*')
                nearby_mine_count++;
        // Check for mine diagonal-dleft
            if(board[r_select - 1][c_select - 1] == '*')
                nearby_mine_count++;
        // Check for mine above
            if(board[r_select - 1][c_select] == '*')
                nearby_mine_count++;
    }

    return nearby_mine_count;
}

/* Check if user has won game */
int check_win_game()
{
    int row, col;

    for(row = 0; row < BOARD_SIZE; row++)
        for(col = 0; col < BOARD_SIZE; col++)
        {
            if(game_board[row][col] == 'o' && board[row][col] != '*')
                return FALSE;
        }

    return TRUE;
}
// Ask user if they wish to play again.
void play_again()
{
    char ans;

    printf("\n\nWould you like to play again? (y/n) --> ");
    scanf(" %c", &ans);

    if(toupper(ans) == 'Y')
    {
        system("cls");
        start();
    }

    else
    {
        printf("\n\nThanks for playing! Bye.");
        (void) getchar();
        exit(EXIT_SUCCESS);
    }
}
// Displays the welcome message, and the GNU License
void display_welcome()
{
    puts("-----------------------Welcome to Minesweeper!---------------------------");
    puts("Version: 1.01");
    puts("\n");
    puts("Copyright (C) <2012>  <Samuel Joseph>\n");
    printf("This program is free software: you can redistribute it and/or modify\n"
    "it under the terms of the GNU General Public License as published by\n"
    "the Free Software Foundation, either version 3 of the License, or\n"
    "(at your option) any later version.");
    puts("\n\n");
}

void start()
{
    lost = 0;    // User hasn't lost yet
    // Build both game boards (one for the user to see,
    // and the one with the mines).
    build_board();
    build_gboard();

    // Start playing game
    do
    {
    play_game();
    print_board();
    } while(lost != 1);    // While the user hasn't lost, loop.

    // Once user is lost, print the board with all the mines.
    print_fullboard();

    // Play again?
    play_again();
}
