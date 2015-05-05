#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#define MAX 25 // Maximum board size

struct board // Struct the hidden board, enabling the return of the board
{
    char mode;
    char gboard[MAX][MAX];
};

void welcome(); //Display the welcome message
int set_difficulty(); // Set difficulty and size
int set_minenum(int size); // Set mine number
struct board draw_hboard(int size, int minenum); // Draw board and plant mine. This board will not be seen to the player
struct board plant_mines(struct board h_board, int size, int minenum); // plant as many as mine_num on the hboard (invoked in the 'draw_hboard' function
struct board draw_pboard(int size, struct board p_board);
struct board ask_u_action(int size, struct board h_board, struct board p_board); // Ask user action, calls open_mine() or mark_mine() function.
struct board open_mine(int size, struct board h_board, struct board p_board); // When in open mine mode, tries opening a mine. While doing that, it calls 'check_mode_change()' function
int check_mode_change(char u_input[]); // Check if the input is for mode change, also check if there's unwanted alphabets.
struct board mark_mine(int size, struct board p_board);
int check_correct_range(int num, int size); // check if the user provided number is out of range
void print_board(struct board someboard, int size); // print h_board or p_board when necessary
char rechallenge(struct board p_board, int size); // Let's be polite. Lets care about if player wants to challenge again. returns 'y' or 'n'
void restart_game();

int main()
{
    int i, j;
    int size, mine_num; // Declare board size, and mine number
    struct board h_board, p_board; // Declare board, its gonna be never bigger than MAX.
    // hboard is hidden from user, u_board is what players see
    p_board.mode = 'o'; //'o' is for open mode 'm' is for mark mode, start with open mode
    welcome(); //Display the welcome message
    size = set_difficulty();
    mine_num = set_minenum(size);
    h_board = draw_hboard(size, mine_num); // Draw board and plant mine. This board will not be seen to the player
    // check hboard
    print_board(h_board, size);
    p_board = draw_pboard(size, p_board); // Draw the board for players and print it
    do
    {
        p_board = ask_u_action(size, h_board, p_board);
        if ( p_board.mode == 'l') { // if mode is l, it means player lost. check if the player is lost.
            if (rechallenge(p_board, size) == 'n'){ // Let's be polite. Lets care about if player wants to challenge again.
                break;
            } else if (rechallenge(p_board, size) == 'y') {
                restart_game();
            }
        } else if ( p_board.mode == 'w') { // if mode is w, it means player won. check if the player won.
            if (rechallenge(p_board, size) == 'n'){ // Let's be polite. Lets care about if player wants to challenge again.
                break;
            } else if (rechallenge(p_board, size) == 'y') {
                restart_game();
            }
        }
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

void print_board(struct board someboard, int size)
{
    int i, j;
    for(i = 0; i < size; i++) {
        for(j = 0; j < size; j++) {
            printf("%c ", someboard.gboard[i][j]);
        }
        printf("\n");
    }
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

struct board ask_u_action(int size, struct board h_board, struct board p_board)
{
    char u_input[20];
    struct board result;

    if (p_board.mode == 'o') {
        printf("Now you are in 'Open mode', which means you can open a location at your risk.\n");
        printf("First type the row number[0 ~ %i], enter, and then column number[0 ~ %i]\n", size-1, size-1);
        printf("If you want to change your mode to 'Mark mode', Type 'm'.\n");
        printf("That way, you will mark a place as mine with the actual risk of opening it.\n");
        result = open_mine(size, h_board, p_board);
        return result;
    } else if (p_board.mode == 'm') {
        printf("Now you are in 'Mark mode', which means you will mark a place as a mine!\n"
               "This doesn't guarantee that you were right in your guess, it is just for your reference!\n");
        printf("First type the row number[0 ~ %i], enter, and then column number[0 ~ %i]\n", size-1, size-1);
        printf("If you want to change your mode to 'Open mode', Type 'o'.\n");
        printf("That way, you can open a location you think is safe.\n");
        result = mark_mine(size, p_board);
        return result;
    }
}

struct board open_mine(int size, struct board h_board, struct board p_board)
{
    int row, col, ifvalid;
    char u_input[20];
    scanf("%19s", u_input); // Get input maximum of 20 characters.
    ifvalid = check_mode_change(u_input); // Check if the input is for mode change, also check if there's unwanted alphabets.
    // ifvalid = 1 means input is 'o', ifvalid = 2 means input is 'm'
    if (ifvalid == 1) {
        p_board.mode = 'o';
        return p_board;
    } else if (ifvalid == 2) {
        p_board.mode = 'm';
        return p_board;
    } else if (ifvalid == 0) { // ifvalid = 0 means there was an invalid input so just return mode, exiting function without doing anything.
        return p_board;
    }
    // Now we are sure we got some number input. Let's check if it is from 0 to (size-1). Because that is the legit range.
    row = atoi(u_input);
    if ( check_correct_range(row, size) == 0 ) // If the player gave a wrong range, just return, exiting function without doing anything.
        return p_board;
    // ask col number now
    printf("You selected row #%d, and now please select column number: ", row);
    scanf("%20d", &col);
    if ( check_correct_range(col, size) == 0 ) // If the player gave a wrong range, just return, exiting function without doing anything.
        return p_board;
    printf("You selected row #%d and column number #%d\n ", row, col);
    // check_dead_alive();
    if (h_board.gboard[row][col] == '.') {
        printf("\nBomb!!! Sorry, but you stepped on a mine!!!\nYou lost! But you are welcome to challenge again!\n\n\n");
        print_board(h_board, size);
        p_board.mode = 'l'; // mode l means, player lost the game.
        return p_board;
    } else if (p_board.gboard[row][col] == '0') {

    }
    return p_board;
}

int check_mode_change(char u_input[]) //struct board p_board) // Check if the input is for mode change, also check if there's unwanted alphabets.
{
    int i, input_len, ifvalid; // ifvalid = 0 means invalid input, ifvalid = 1 means input is 'o', ifvalid = 2 means input is 'm', ifvalid = 4 means input is numeric
    // change mode
    u_input[0] = tolower(u_input[0]); // change u input to lower case
    if ( strcmp(u_input, "o") == 0 ) {
        ifvalid = 1;
        return ifvalid; // change mode
    } else if ( strcmp(u_input, "m") == 0 ) {
        ifvalid = 2;
        return ifvalid;
    } else {
    // check if 'u_input' has non-numeric value and if it has, send an error message.
        input_len = strlen(u_input);
        for (i = 0; i < input_len; i++) {
            if (!isdigit(u_input[i])) {
                printf("\nWrong input! Enter only 'm', 'o', or row coordinate.\n");
                ifvalid = 0;
                return ifvalid;
            }
        }
    }
    ifvalid = 4;
    return ifvalid;
}

int check_correct_range(int num, int size) // check if the user provided number is out of range
{
    if ((num < 0) || (num > (size-1))) {
        printf("\nWrong input! Enter from 0 to %i only!\n", size-1);
        return 0; // Return 0. Because the player gave a wrong range.
    } else {
        return 1;
    }
}

struct board mark_mine(int size, struct board p_board)
{
    int row, col, ifvalid;
    char u_input[20];
    scanf("%19s", u_input); // Get input maximum of 20 characters.
    ifvalid = check_mode_change(u_input); // Check if the input is for mode change, also check if there's unwanted alphabets.
    // ifvalid = 1 means input is 'o', ifvalid = 2 means input is 'm'
    if (ifvalid == 1) {
        p_board.mode = 'o';
        return p_board;
    } else if (ifvalid == 2) {
        p_board.mode = 'm';
        return p_board;
    } else if (ifvalid == 0) { // ifvalid = 0 means there was an invalid input so just return mode, exiting function without doing anything.
        return p_board;
    }
    // Now we are sure we got some number input. Let's check if it is from 0 to (size-1). Because that is the legit range.
    row = atoi(u_input);
    if ( check_correct_range(row, size) == 0 ) // If the player gave a wrong range, just return mode, exiting function without doing anything.
        return p_board;
    // ask col number now
    printf("You selected row #%d, and now please select column number: ", row);
    scanf("%20d", &col);
    if ( check_correct_range(col, size) == 0 ) // If the player gave a wrong range, just return mode, exiting function without doing anything.
        return p_board;
    printf("You selected row #%d and column number #%d\n ", row, col);
    return p_board;
}

char rechallenge(struct board p_board, int size)
{
    char whatnow;
    if (p_board.mode == 'l') {
        printf("Well... What now? Do you wanna challenge again after healing yourself from the mine damage?\n"
               "Choose y for a re-challenge or n for quit like a loser.(y/n): ");
        do {
            scanf("%c", &whatnow);
            if (whatnow == 'n') {
                printf("Ok, whatever. Bye-bye quitter.\n");
                getchar();
                return whatnow;
            } else if (whatnow == 'y') {
                printf("Yesss! I knew you were strong. Let's show your ability this time for real!\n");
                getchar();
                return whatnow;
            } else {
                printf("Please select only y or n.(y/n): ");
            }
        } while (1);
    } else if (p_board.mode == 'w') {
        if (size == 10) {
            printf("Wow, you just won the normal lever of this game!!! CONGRATULATION!\n"
               "Do you want to play again? Maybe this time you could challenge the hard level.(y/n): ");
            do {
                scanf("%c", &whatnow);
                if (whatnow == 'n') {
                    printf("Ok. See you again!\n");
                    getchar();
                    return whatnow;
                } else if (whatnow == 'y') {
                    printf("Yesss!!! Let's sweep another mine field!\n");
                    getchar();
                    return whatnow;
                } else {
                    printf("Please select only y or n.(y/n): ");
                }
            } while (1);
        } else if (size == 15){
            printf("incredible!!! You actually just won the HARD level of this game!!! CONGRATULATION!\n"
               "Do you want to play again? (y/n): ");
            do {
                scanf("%c", &whatnow);
                if (whatnow == 'n') {
                    printf("Farewell my friend. If you ever wanna show off your intelligence, you are always welcome!\n");
                    getchar();
                    return whatnow;
                } else if (whatnow == 'y') {
                    printf("Yesss!!! Let's sweep another mine field!\n");
                    getchar();
                    return whatnow;
                } else {
                    printf("Please select only y or n.(y/n): ");
                }
            } while (1);
        }
    }
}

void restart_game()
{
    // copy main function here.
}
