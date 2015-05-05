#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#define MAX 25 // Maximum board size

struct board // Struct the hidden board, enabling the return of the board
{
    char gboard[MAX][MAX];
};

void welcome(); //Display the welcome message
int set_difficulty(); // Set difficulty and size
int set_minenum(int size); // Set mine number
struct board draw_hboard(int size, int minenum); // Draw board and plant mine. This board will not be seen to the player
struct board plant_mines(struct board h_board, int size, int minenum); // plant as many as mine_num on the hboard (invoked in the 'draw_hboard' function
struct board draw_pboard(int size, struct board p_board);
char ask_u_action(char mode, int size); // Ask user action, calls open_mine() or mark_mine() function.
char open_mine(); //
char mark_mine();

int main()
{
    char mode = 'o'; //'o' is for open mode 'm' is for mark mode, start with open mode
    int i, j;
    int size, mine_num; // Declare board size, and mine number
    struct board h_board, p_board; // Declare board, its gonna be never bigger than MAX.
    // hboard is hidden from user, u_board is what players see
    welcome(); //Display the welcome message
    size = set_difficulty();
    mine_num = set_minenum(size);
    h_board = draw_hboard(size, mine_num); // Draw board and plant mine. This board will not be seen to the player
    // check hboard
    for(i = 0; i < size; i++) {
        for(j = 0; j < size; j++) {
            printf("%c ", h_board.gboard[i][j]);
        }
        printf("\n");
    }
    p_board = draw_pboard(size, p_board); // Draw the board for players and print it
    do
    {
        mode = ask_u_action(mode, size);
    } while (1);

    return 0;
}

void welcome() //Display the welcome message
{
    char c;
    FILE *fp;
    fp = fopen("welcome.txt", "r");
    if(fp == NULL) {
      perror("Error in opening welcome message!");
      return;
    }

    while ((c = fgetc(fp)) != EOF) {
        printf("%c", c);
    }
    printf("\n\n");
    getchar(); // Waiting for a user input
    fclose(fp);
}

/* Function to set the game difficulty */
int set_difficulty()
{
    char difficulty;
    int size;
    do
    {
        printf("Select difficulty level(h for hard, n for normal>");
        difficulty = getchar();
        difficulty = tolower(difficulty);
        // tolower(difficulty);
        if (difficulty == 'h') {
            size = 15;
            printf("The board is %iX%i size!\n", size, size);
            return size;
        } else if (difficulty == 'n') {
            size = 10;
            printf("The board is %iX%i size!\n", size, size);
            return size;
        }
    } while (1);
}

int set_minenum(int size)
{
    int mine_num;
    do
    {
        if ( size == 15 ) {
            mine_num = 25;
            printf("You have total %i mines to find!\n", mine_num);
            return mine_num;
        } else if ( size == 10) {
            mine_num = 10;
            printf("You have total %i mines to find!\n\n", mine_num);
            return mine_num;
        }
    } while (1);
}

/* Draw board and this board will not be seen to the player */

struct board draw_hboard(int size, int mine_num)
{
    int i, j;
    struct board h_board;

    for(i = 0; i < size; i++)
        for(j = 0; j < size; j++)
            h_board.gboard[i][j] = '0';

    // plant hidden mines
    h_board = plant_mines(h_board, size, mine_num);
    return h_board;
}

struct board plant_mines(struct board h_board, int size, int mine_num)
{
    int i, row_random, col_random;

    // Seeding srand() with time(0) so that
    // mine locations aren't the same every
    // time the game is played.
    srand(time(0));

    i = 0;
    while  (i < mine_num)
    {
        row_random = rand() % (size);
        col_random = rand() % (size);
        // avoid planting mine at the same place, place mine only when the coordinate is empty
        if (h_board.gboard[row_random][col_random] == '0') {
            h_board.gboard[row_random][col_random] = '.';
            i++;
        }
    }
    return h_board;
}

struct board draw_pboard(int size, struct board p_board) // Draw the board for players and print it
{
    int i, j;

    for(i = 0; i < size; i++)
        for(j = 0; j < size; j++)
            p_board.gboard[i][j] = '0';

    // print player board
    printf("\nThis is where you are. The mine field\n\n");
    for(i = 0; i < size; i++) {
        for(j = 0; j < size; j++) {
            printf("%c ", p_board.gboard[i][j]);
        }
        printf("\n");
    }
    return p_board;
}

char ask_u_action(char mode, int size)
{
    char u_input[20], choice;
    if (mode == 'o') {
        printf("Now you are in 'Open mode', which means you can open a location at your risk.\n");
        printf("First type the row number[0 ~ %i], enter, and then column number[0 ~ %i]\n", size);
        printf("If you want to change your mode to 'Mark mode', Type 'm'.\n");
        printf("That way, you will mark a place as mine with the actual risk of opening it.\n");
        choice = open_mine();
        return choice;
    } else if (mode == 'm') {
        printf("Now you are in 'Mark mode', which means you will mark a place as a mine!\n"
               "This doesn't guarantee that you were right in your guess, it is just for your reference!\n");
        printf("First type the row number[0 ~ %i], enter, and then column number[0 ~ %i]\n", size);
        printf("If you want to change your mode to 'Open mode', Type 'o'.\n");
        printf("That way, you can open a location you think is safe.\n");
        choice = mark_mine();
        return choice;
    }
    /*
    do
    {
        scanf("%19s", u_input); // Get input maximum of 20 characters.
        // change mode
        //if ((strcmp(u_input, "o\0")) || (strcmp(u_input, "O\0")) || (strcmp(u_input, "m\0")) || (strcmp(u_input, "M\0")))
        u_input[0] = tolower(u_input[0]); // change u input to lower case
        if ((u_input[0] == 'o') || (u_input[0] == 'm')) {
            return u_input[0];
        } else {
            printf("Wrong input Enter 'm', 'o', or row coordinate\n");
        }
    } while (1);
    */
}

char open_mine(struct board h_board, struct board p_board)
{
    char u_input[20];
    do
    {
        scanf("%19s", u_input); // Get input maximum of 20 characters.
        // change mode
        //if ((strcmp(u_input, "o\0")) || (strcmp(u_input, "O\0")) || (strcmp(u_input, "m\0")) || (strcmp(u_input, "M\0")))
        u_input[0] = tolower(u_input[0]); // change u input to lower case
        if ((strcmp(u_input, "o") == 0) || (strcmp(u_input, "m") == 0)) {
            return u_input[0];
        } else {
            printf("Wrong input! Enter 'm', 'o', or row coordinate.\n");
        }
    } while (1);
}

char mark_mine(struct board h_board, struct board p_board)
{
    char u_input[20];
    do
    {
        scanf("%19s", u_input); // Get input maximum of 20 characters.
        // change mode
        //if ((strcmp(u_input, "o\0")) || (strcmp(u_input, "O\0")) || (strcmp(u_input, "m\0")) || (strcmp(u_input, "M\0")))
        u_input[0] = tolower(u_input[0]); // change u input to lower case
        if ((strcmp(u_input, "o") == 0) || (strcmp(u_input, "m") == 0)) {
            return u_input[0];
        } else {
            printf("Wrong input! Enter 'm', 'o', or row coordinate.\n");
        }
    } while (1);
}
