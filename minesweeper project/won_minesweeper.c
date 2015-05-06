#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h> // need this for random seed
#define MAX 25 // Maximum board size

struct board // Struct the hidden board, enabling the return of the board
{
    char mode;
    int size, mine_num, remained_unopened_field;
    char gboard[MAX][MAX];
};

void start_game(struct board h_board, struct board p_board); // start or restart game from the beginning
void welcome(); //Display the welcome message
int set_difficulty(); // Set difficulty and size
int set_minenum(struct board p_board); // Set mine number
struct board draw_hboard(struct board h_board); // Draw board and plant mine. This board will not be seen to the player
struct board plant_mines(struct board h_board); // plant as many as mine_num on the hboard (invoked in the 'draw_hboard' function
struct board draw_pboard(struct board p_board); // Draw the board for players
struct board ask_u_action(struct board h_board, struct board p_board); // Ask user action, calls open_mine() or mark_mine() function.
struct board open_mine(struct board h_board, struct board p_board); // When in open mine mode, tries opening a mine. While doing that, it calls 'check_mode_change()' function
int check_mode_change(char u_input[]); // Check if the input is for mode change, also check if there's unwanted alphabets.
struct board mark_mine(struct board p_board);
int check_correct_range(int num, struct board p_board); // check if the user provided number is out of range
void print_board(struct board someboard); // print h_board or p_board when necessary
char count_nearby_mines(int row, int col, struct board h_board);
struct board check_nearby_regions(int row, int col, struct board h_board, struct board p_board); // check nearby regions automatically and update the player board, called in open_mine() function.
struct board check_win(struct board p_board); // check if player won the game, counting numeric fields should test the winning condition.
char rechallenge(struct board p_board); // Let's be polite. Lets care about if player wants to challenge again. returns 'y' or 'n'

int main()
{
    int i, j;
    struct board h_board, p_board; // Declare board, its gonna be never bigger than MAX.
    // hboard is hidden from user, u_board is what players see
    start_game(h_board, p_board);
    /*
    p_board.mode = 'o'; //'o' is for open mode 'm' is for mark mode, start with open mode
    welcome(); //Display the welcome message
    h_board.size = p_board.size = set_difficulty();
    h_board.mine_num = p_board.mine_num = set_minenum(p_board);
    h_board = draw_hboard(h_board); // Draw board and plant mine. This board will not be seen to the player
    // check hboard
    print_board(h_board);
    p_board = draw_pboard(p_board); // Draw the board for players
    printf("\nThis is where you are. The mine field\n\n");
    print_board(p_board);
    do
    {
        p_board = ask_u_action(h_board, p_board);
        if ( p_board.mode == 'l') { // if mode is l, it means player lost. check if the player is lost.
            if (rechallenge(p_board) == 'n'){ // Let's be polite. Lets care about if player wants to challenge again.
                break;
            } else if (rechallenge(p_board) == 'y') {
                start_game();
            }
        } else if ( p_board.mode == 'w') { // if mode is w, it means player won. check if the player won.
            if (rechallenge(p_board) == 'n'){ // Let's be polite. Lets care about if player wants to challenge again.
                break;
            } else if (rechallenge(p_board) == 'y') {
                start_game();
            }
        }
    } while (1);
    */
    return 0;
}

void start_game(struct board h_board, struct board p_board) // start or restart game from the beginning
{
    char replay; // contains answer of the player, wanna replay or not
    p_board.mode = 'o'; //'o' is for open mode 'm' is for mark mode, start with open mode
    welcome(); //Display the welcome message
    h_board.size = p_board.size = set_difficulty(); // set difficulty(normal or hard)
    h_board.mine_num = p_board.mine_num = set_minenum(p_board); //set the number of mines 10 or 25
    h_board = draw_hboard(h_board); // Draw board and plant mine. This board will not be seen to the player
    // check hboard
    print_board(h_board); // print hidden board for the convenience of the developer to test
    p_board = draw_pboard(p_board); // Draw the board for players
    p_board.remained_unopened_field = (p_board.size * p_board.size) - p_board.mine_num; //Initialize remained_unopened_field
    printf("This is where you are. The mine field\n\n");
    do
    {
        p_board = ask_u_action(h_board, p_board);
        if ( p_board.mode == 'l') { // if mode is l, it means player lost. check if the player is lost.
            replay = rechallenge(p_board);
            if (replay == 'n'){ // Let's be polite. Lets care about if player wants to challenge again.
                break;
            } else if (replay == 'y') {
                start_game(h_board, p_board); // when the player wants to play it again, it calls 'start_game()' function recursively
            }
        } else if ( p_board.mode == 'w') { // if mode is w, it means player won. check if the player won.
            replay = rechallenge(p_board);
            if (replay == 'n'){ // Let's be polite. Lets care about if player wants to challenge again.
                break;
            } else if (replay == 'y') {
                start_game(h_board, p_board); // when the player wants to play it again, it calls 'start_game()' function recursively
            }
        }
    } while (1);
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
            printf("The board is now %iX%i size!\n", size, size);
            return size;
        } else if (difficulty == 'n') {
            size = 10;
            printf("The board is now %iX%i size!\n", size, size);
            return size;
        }
    } while (1);
}

int set_minenum(struct board p_board)
{
    int mine_num;
    do
    {
        if (p_board.size == 15 ) {
            mine_num = 25;
            printf("You have total %i mines to find!\n", mine_num);
            return mine_num;
        } else if ( p_board.size == 10) {
            mine_num = 10;
            printf("You have total %i mines to find!\n\n", mine_num);
            return mine_num;
        }
    } while (1);
}

/* Draw board and this board will not be seen to the player */

struct board draw_hboard(struct board h_board)
{
    int i, j;

    for(i = 0; i < h_board.size; i++)
        for(j = 0; j < h_board.size; j++)
            h_board.gboard[i][j] = 'X';

    // plant hidden mines
    h_board = plant_mines(h_board);
    return h_board;
}

struct board plant_mines(struct board h_board)
{
    int i, row_random, col_random;

    // Seeding srand() with time(0) so that
    // mine locations aren't the same every
    // time the game is played.
    srand(time(0));

    i = 0;
    while  (i < h_board.mine_num)
    {
        row_random = rand() % (h_board.size);
        col_random = rand() % (h_board.size);
        // avoid planting mine at the same place, place mine only when the coordinate is empty
        if (h_board.gboard[row_random][col_random] == 'X') {
            h_board.gboard[row_random][col_random] = '.';
            i++;
        }
    }
    return h_board;
}

void print_board(struct board someboard)
{
    int i, j;
    printf("\n\n");
    for(i = 0; i < someboard.size; i++) {
        for(j = 0; j < someboard.size; j++) {
            printf("%c ", someboard.gboard[i][j]);
        }
        printf("\n");
    }
}

struct board draw_pboard(struct board p_board) // Draw the board for players and print it
{
    int i, j;

    for(i = 0; i < p_board.size; i++)
        for(j = 0; j < p_board.size; j++)
            p_board.gboard[i][j] = 'X';

    return p_board;
}

struct board ask_u_action(struct board h_board, struct board p_board)
{
    char u_input[20];
    struct board result;
    system("cls"); // Clearing the screen. Not sure if a machine operating on other than Windows OS is compatible
    print_board(h_board); // for developer's convenience, I show the hidden board.
    print_board(p_board);
    printf("\n%i unopened field(s) until your victory! Wish your luck!\n", p_board.remained_unopened_field); // Tell the player how many fields are left to be opened

    if (p_board.mode == 'o') {
        printf("\nNow you are in 'Open mode', which means you can open a location at your risk.\n");
        printf("First type the row number[0 ~ %i], enter, and then column number[0 ~ %i]\n", p_board.size-1, p_board.size-1);
        printf("If you want to change your mode to 'Mark mode', Type 'm'.\n");
        printf("With the 'Mark mode', you can mark a place as mine with the actual risk of opening it.\n");
        result = open_mine(h_board, p_board);
        return result;
    } else if (p_board.mode == 'm') {
        printf("\nNow you are in 'Mark mode', which means you will mark a place as a mine!\n"
               "This doesn't guarantee that you were right in your guess, it is just for your reference!\n"
               "If you mark, the location will be seen as '-', if you want to un-mark there, \n"
               "simply enter the same location again.");
        printf("First type the row number[0 ~ %i], enter, and then column number[0 ~ %i]\n", p_board.size-1, p_board.size-1);
        printf("If you want to change your mode to 'Open mode', Type 'o'.\n");
        printf("With the 'Open mode', you can open a location you think is safe.\n");
        result = mark_mine(p_board);
        return result;
    }
}

struct board open_mine(struct board h_board, struct board p_board)
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
    } else if (ifvalid == 0) { // ifvalid = 0 means there was an invalid input so just return, exiting function without doing anything.
        return p_board;
    }
    // Now we are sure we got some number input. Let's check if it is from 0 to (size-1). Because that is the legit range.
    row = atoi(u_input);
    if ( check_correct_range(row, p_board) == 0 ) // If the player gave a wrong range, just return, exiting function without doing anything.
        return p_board;
    // ask col number now
    printf("You selected row #%d, and now please select column number: ", row);
    scanf("%20d", &col);
    if ( check_correct_range(col, p_board) == 0 ) // If the player gave a wrong range, just return, exiting function without doing anything.
        return p_board;
    printf("You selected row #%d and column number #%d\n ", row, col);

    // check_dead_alive();
    if (h_board.gboard[row][col] == '.') {
        printf("\nBomb!!! Sorry, but you stepped on a mine!!!\nYou lost! But you are welcome to challenge again!\n\n\n");
        print_board(h_board);
        p_board.mode = 'l'; // mode l means, player lost the game.
        return p_board;
    } else if (p_board.gboard[row][col] == 'X') {
        p_board.gboard[row][col] = count_nearby_mines(row, col, h_board);
        if (count_nearby_mines(row, col, h_board) == '0') { //check if there was no mine at all nearby, because in that case, we need to check nearby regions automatically.
            p_board = check_nearby_regions(row, col, h_board, p_board); // check nearby regions and update the player board
        }

        // Here comes a long complex code, this part implements that, if you have 0 for the checked nearby regions, check nearby regions of there automatically.
        // This should repeat until You find a region that there are nearby mines.
        /*
        // check south west, south, south east regions
    if ((row + 1) <= (h_board.size - 1)) { // make sure you don't access to outside board
        if ((col - 1) >= 0) { // make sure you don't access to outside board
            p_board.gboard[row + 1][col - 1] = count_nearby_mines(row + 1, col - 1, h_board); // check south west
            // if ((count_nearby_mines(row + 1, col - 1, h_board)) == '0') // check there s no mine at all again
            //     p_board = check_nearby_regions(row + 1, col - 1, h_board, p_board); // If no mine, call check_nearby_regions recursively. this should loop until it finds some mines nearby
        }
        p_board.gboard[row + 1][col] = count_nearby_mines(row + 1, col, h_board); // check south
        // if ((count_nearby_mines(row + 1, col, h_board)) == '0') // check there s no mine at all again
        //     p_board = check_nearby_regions(row + 1, col, h_board, p_board); // If no mine, call check_nearby_regions recursively. this should loop until it finds some mines nearby
        if ((col + 1) <= (h_board.size - 1)) { // make sure you don't access to outside board
            p_board.gboard[row + 1][col + 1] = count_nearby_mines(row + 1, col + 1, h_board); // check south east
        //     if ((count_nearby_mines(row + 1, col + 1, h_board)) == '0') // check there s no mine at all again
        //         p_board = check_nearby_regions(row + 1, col + 1, h_board, p_board); // If no mine, call check_nearby_regions recursively. this should loop until it finds some mines nearby
        }
    }
    // check nearby regions of due west and due east
    // check due west, due east mines
    if ((col - 1) >= 0) { // make sure you don't access to outside board
        p_board.gboard[row][col - 1] = count_nearby_mines(row, col - 1, h_board); // check due west
        // if ((count_nearby_mines(row, col - 1, h_board)) == '0') // check there s no mine at all again
        //     p_board = check_nearby_regions(row, col - 1, h_board, p_board); // If no mine, call check_nearby_regions recursively. this should loop until it finds some mines nearby
    }
    if ((col + 1) <= (h_board.size - 1)) { // make sure you don't access to outside board
        p_board.gboard[row][col + 1] = count_nearby_mines(row, col + 1, h_board); // check due east
        // if ((count_nearby_mines(row, col + 1, h_board)) == '0') // check there s no mine at all again
        //     p_board = check_nearby_regions(row, col + 1, h_board, p_board); // If no mine, call check_nearby_regions recursively. this should loop until it finds some mines nearby
    }
    return p_board;
    */
        p_board = check_win(p_board);
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

int check_correct_range(int num, struct board p_board) // check if the user provided number is out of range
{
    if ((num < 0) || (num > (p_board.size-1))) {
        printf("\nWrong input! Enter from 0 to %i only!\n", p_board.size-1);
        return 0; // Return 0. Because the player gave a wrong range.
    } else {
        return 1;
    }
}

struct board mark_mine(struct board p_board)
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
    if ( check_correct_range(row, p_board) == 0 ) // If the player gave a wrong range, just return mode, exiting function without doing anything.
        return p_board;
    // ask col number now
    printf("You selected row #%d, and now please select column number: ", row);
    scanf("%20d", &col);
    if ( check_correct_range(col, p_board) == 0 ) // If the player gave a wrong range, just return mode, exiting function without doing anything.
        return p_board;
    printf("You selected row #%d and column number #%d\n ", row, col);
    if (p_board.gboard[row][col] == 'X') {// If the location is unmarked, mark there.
        p_board.gboard[row][col] = '-';
    } else if (p_board.gboard[row][col] == '-') {// If the location is marked, unmark there.
        p_board.gboard[row][col] = 'X';
    }
    return p_board;
}

char count_nearby_mines(int row, int col, struct board h_board)
{
    int i, how_many=0; // count with how_many and then turn it into char type 'mine_count'
    char mine_count; // mine_count will be returned as a char
    // check north west, north, north east mines
    if ((row - 1) >= 0) { // make sure you don't access to outside board
        if ((col - 1) >= 0) { // make sure you don't access to outside board
            if ((h_board.gboard[row - 1][col - 1]) == '.') // check north west
                how_many++; // mine count increases
        }
        if ((h_board.gboard[row - 1][col]) == '.') // check north
            how_many++; // mine count increases
        if ((col + 1) <= (h_board.size - 1)) { // make sure you don't access to outside board
            if ((h_board.gboard[row - 1][col + 1]) == '.') // check north east
                how_many++; // mine count increases
        }
    }
    // check south west, south, south east mines
    if ((row + 1) <= (h_board.size - 1)) { // make sure you don't access to outside board
        if ((col - 1) >= 0) { // make sure you don't access to outside board
            if ((h_board.gboard[row + 1][col - 1]) == '.') // check south west
                how_many++; // mine count increases
        }
        if ((h_board.gboard[row + 1][col]) == '.') // check south
            how_many++; // mine count increases
        if ((col + 1) <= (h_board.size - 1)) { // make sure you don't access to outside board
            if ((h_board.gboard[row + 1][col + 1]) == '.') // check north east
                how_many++; // mine count increases
        }
    }
    // check due west, due east mines
    if ((col - 1) >= 0) { // make sure you don't access to outside board
        if ((h_board.gboard[row][col - 1]) == '.') // check due west
            how_many++; // mine count increases
    }
    if ((col + 1) <= (h_board.size - 1)) { // make sure you don't access to outside board
        if ((h_board.gboard[row][col + 1]) == '.') // check due east
            how_many++; // mine count increases
    }
    mine_count = how_many + '0'; // convert int 'how_many' into char 'mine_count'
    return mine_count;
}

struct board check_nearby_regions(int row, int col, struct board h_board, struct board p_board) // check nearby regions automatically and update the player board, called in open_mine() function.
{
    // check north west, north, north east regions
    if ((row - 1) >= 0) { // make sure you don't access to outside board
        if ((col - 1) >= 0) { // make sure you don't access to outside board
            p_board.gboard[row - 1][col - 1] = count_nearby_mines(row - 1, col - 1, h_board); // check north west
            // if ((count_nearby_mines(row - 1, col - 1, h_board)) == '0') // check there s no mine at all again
            //     p_board = check_nearby_regions(row - 1, col - 1, h_board, p_board); // If no mine, call check_nearby_regions recursively. this should loop until it finds some mines nearby
        }
        p_board.gboard[row - 1][col] = count_nearby_mines(row - 1, col, h_board); // check north
        // if ((count_nearby_mines(row - 1, col, h_board)) == '0') // check there s no mine at all again
        //     p_board = check_nearby_regions(row - 1, col, h_board, p_board); // If no mine, call check_nearby_regions recursively. this should loop until it finds some mines nearby
        if ((col + 1) <= (h_board.size - 1)) { // make sure you don't access to outside board
            p_board.gboard[row - 1][col + 1] = count_nearby_mines(row - 1, col + 1, h_board); // check north east
            // if ((count_nearby_mines(row - 1, col + 1, h_board)) == '0') // check there s no mine at all again
            //     p_board = check_nearby_regions(row - 1, col + 1, h_board, p_board); // If no mine, call check_nearby_regions recursively. this should loop until it finds some mines nearby
        }
    }
    // check south west, south, south east regions
    if ((row + 1) <= (h_board.size - 1)) { // make sure you don't access to outside board
        if ((col - 1) >= 0) { // make sure you don't access to outside board
            p_board.gboard[row + 1][col - 1] = count_nearby_mines(row + 1, col - 1, h_board); // check south west
            // if ((count_nearby_mines(row + 1, col - 1, h_board)) == '0') // check there s no mine at all again
            //     p_board = check_nearby_regions(row + 1, col - 1, h_board, p_board); // If no mine, call check_nearby_regions recursively. this should loop until it finds some mines nearby
        }
        p_board.gboard[row + 1][col] = count_nearby_mines(row + 1, col, h_board); // check south
        // if ((count_nearby_mines(row + 1, col, h_board)) == '0') // check there s no mine at all again
        //     p_board = check_nearby_regions(row + 1, col, h_board, p_board); // If no mine, call check_nearby_regions recursively. this should loop until it finds some mines nearby
        if ((col + 1) <= (h_board.size - 1)) { // make sure you don't access to outside board
            p_board.gboard[row + 1][col + 1] = count_nearby_mines(row + 1, col + 1, h_board); // check south east
        //     if ((count_nearby_mines(row + 1, col + 1, h_board)) == '0') // check there s no mine at all again
        //         p_board = check_nearby_regions(row + 1, col + 1, h_board, p_board); // If no mine, call check_nearby_regions recursively. this should loop until it finds some mines nearby
        }
    }
    // check due west, due east mines
    if ((col - 1) >= 0) { // make sure you don't access to outside board
        p_board.gboard[row][col - 1] = count_nearby_mines(row, col - 1, h_board); // check due west
        // if ((count_nearby_mines(row, col - 1, h_board)) == '0') // check there s no mine at all again
        //     p_board = check_nearby_regions(row, col - 1, h_board, p_board); // If no mine, call check_nearby_regions recursively. this should loop until it finds some mines nearby
    }
    if ((col + 1) <= (h_board.size - 1)) { // make sure you don't access to outside board
        p_board.gboard[row][col + 1] = count_nearby_mines(row, col + 1, h_board); // check due east
        // if ((count_nearby_mines(row, col + 1, h_board)) == '0') // check there s no mine at all again
        //     p_board = check_nearby_regions(row, col + 1, h_board, p_board); // If no mine, call check_nearby_regions recursively. this should loop until it finds some mines nearby
    }
    return p_board;
}

struct board check_win(struct board p_board) // check if player won the game, counting numeric fields should test the winning condition.
{
    int i, j, open_field_cnt = 0;
    int num_no_mine_field = (p_board.size * p_board.size) - p_board.mine_num;

    for (i = 0; i < p_board.size; i++) {
        for (j = 0; j < p_board.size; j++) {
            if (isdigit(p_board.gboard[i][j])) { // Opened field should have numeric value from 0 to 8, check the field if it has a value for a opened field
                open_field_cnt++;
            }
        }
    }

    p_board.remained_unopened_field = num_no_mine_field - open_field_cnt; // set how many fields without mines are unopened

    if (p_board.remained_unopened_field == 0) // Check if player opened all the field with no mine.
        p_board.mode = 'w'; // If so, we will return the p_board with 'Win mode'

    return p_board;
}

char rechallenge(struct board p_board)
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
                getc(stdin);
                return whatnow;
            } else if (whatnow == 'y') {
                printf("Yesss! I knew you were strong. Let's show your ability this time for real!\n");
                getchar();
                getc(stdin);
                return whatnow;
            } else {
                printf("Please select only y or n.(y/n): ");
            }
        } while (1);
    } else if (p_board.mode == 'w') {
        if (p_board.size == 10) {
            printf("Wow, you just won the normal lever of this game!!! CONGRATULATION!\n"
               "Do you want to play again? Maybe this time you could challenge the hard level.(y/n): ");
            do {
                scanf("%c", &whatnow);
                if (whatnow == 'n') {
                    printf("Ok. See you again!\n");
                    getchar();
                    getc(stdin);
                    return whatnow;
                } else if (whatnow == 'y') {
                    printf("Yesss!!! Let's sweep another mine field!\n");
                    getchar();
                    getc(stdin);
                    return whatnow;
                } else {
                    printf("Please select only y or n.(y/n): ");
                }
            } while (1);
        } else if (p_board.size == 15){
            printf("incredible!!! You actually just won the HARD level of this game!!! CONGRATULATION!\n"
               "Do you want to play again? (y/n): ");
            do {
                scanf("%c", &whatnow);
                if (whatnow == 'n') {
                    printf("Farewell my friend. If you ever wanna show off your intelligence, you are always welcome!\n");
                    getchar();
                    getc(stdin);
                    return whatnow;
                } else if (whatnow == 'y') {
                    printf("Yesss!!! Let's sweep another mine field!\n");
                    getchar();
                    getc(stdin);
                    return whatnow;
                } else {
                    printf("Please select only y or n.(y/n): ");
                }
            } while (1);
        }
    }
}
