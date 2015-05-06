/*********************************************************************************

                            Author WON SEOB SEO

Mine sweeper source code in C.
Including features...
0. Playtime display
1. Mine 'markmode'. When you mark as many as mines, the rest is automatically opens for you.
2. 'testrun' mode, make hidden board visible. Enables you to quickly test functions and see how this game work.
3. Difficulty selection(hard : 15X15, 25 mines, normal : 10X10, 10 mines)
4. When you opened a field with number '0', automatically opens its north, south, west, east field as well
5. No global variables. Only constant is "MAX", but easily replaceable.
   But actually used struct board as almost like a global variable because it is invoked from almost every function.
6. When you win or lose, it will politely(?) ask if you want to play it again.
7. When you win, the ending shows your progress. When you lose, the ending shows your progress,
   where you were hit(by 'X'), how it looks like when you win, and the original hidden board.
8. It took so much time to make it, but I really enjoyed making it and learned a lot from it.
9. Now, please enjoy the game! Play!

**********************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h> // need this for random seed
#define MAX 25 // Maximum board size

struct board // Struct the hidden board, enabling the return of the board
{
    char mode;
    int size, mine_num, remained_unopened_field, test_run, num_mark;
    char gboard[MAX][MAX];
    clock_t launch; // to calculate playtime, store start time
    clock_t done; // store end time
};

// the functions appears after the main function. They appear in the order of prototypes.
void start_game(struct board h_board, struct board p_board); // start or restart game from the beginning
void welcome(); //Display the welcome message
struct board set_difficulty(struct board some_board); // Set difficulty(normal or hard), size, test_run mode, mine_number(10 or 25)
struct board draw_hboard(struct board h_board); // Draw board and plant mine. This board will not be seen to the player
struct board plant_mines(struct board h_board); // plant as many as mine_num on the hboard (invoked in the 'draw_hboard' function
struct board draw_pboard(struct board p_board); // Draw the board for players
void print_board(struct board some_board); // print h_board or p_board when necessary
struct board ask_u_action(struct board h_board, struct board p_board); // Ask user action, calls open_mine() or mark_mine() function.
struct board open_mine(struct board h_board, struct board p_board); // When in open mine mode, tries opening a mine. While doing that, it calls 'check_mode_change()' function
int check_mode_change(char u_input[]); // Check if the input is for mode change, also check if there's unwanted alphabets.
struct board check_lost(int row, int col, struct board h_board, struct board p_board); // check if the player is lost
struct board mark_mine(struct board h_board, struct board p_board);
int check_correct_range(int num, struct board p_board); // check if the user provided number is out of range
char count_nearby_mines(int row, int col, struct board h_board);
struct board check_nearby_regions(int row, int col, struct board h_board, struct board p_board); // check nearby regions automatically and update the player board, called in open_mine() function.
struct board check_win(struct board p_board); // check if player won the game, counting numeric fields should test the winning condition.
char rechallenge(struct board p_board); // Let's be polite. Lets care about if player wants to challenge again. returns 'y' or 'n'
void dump_line(FILE * fp);

int main()
{
    int i, j;
    struct board h_board, p_board; // Declare board, its gonna be never bigger than MAX.
    // h_board is hidden from user, p_board is what the players see
    start_game(h_board, p_board);
    return 0;
}

void start_game(struct board h_board, struct board p_board) // start or restart game from the beginning
{
    char replay; // contains answer of the player, wanna replay or not
    welcome(); //Display the welcome message
    h_board = p_board = set_difficulty(p_board); // Set difficulty(normal or hard), size, test_run mode, mine_number(10 or 25)
    h_board = draw_hboard(h_board); // Draw board and plant mine. This board will not be seen to the player
    // check hboard
    print_board(h_board); // print hidden board for the convenience of the developer to test
    p_board = draw_pboard(p_board); // Draw the board for players
    p_board.remained_unopened_field = (p_board.size * p_board.size) - p_board.mine_num; //Initialize remained_unopened_field
    printf("This is where you are. The mine field\n\n");
    p_board.launch = clock(); // store the start time
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
struct board set_difficulty(struct board some_board)
{
    char difficulty[20];
    int i, input_len, size;
    do
    {
        printf("Select difficulty level(h for hard, n for normal(or th, tn for test run)> ");
        scanf("%19s", difficulty);
        input_len = strlen(difficulty);
        // change user input to lowercase
        for (i = 0; i < input_len; i++) {
            difficulty[i] = tolower(difficulty[i]);
        }
        if ((strcmp(difficulty, "h")) == 0) {
            some_board.size = 15;
            some_board.mine_num = 25;
            some_board.test_run = 0; // test run = 0 means it is a normal player mode
            printf("The board is now %iX%i size!\n", some_board.size, some_board.size);
            printf("You have total %i mines to find!\n\n", some_board.mine_num);
            return some_board;
        } else if ((strcmp(difficulty, "n")) == 0) {
            some_board.size = 10;
            some_board.mine_num = 10;
            some_board.test_run = 0; // test run = 0 means it is a normal player mode
            printf("The board is now %iX%i size!\n", some_board.size, some_board.size);
            printf("You have total %i mines to find!\n\n", some_board.mine_num);
            return some_board;
        } else if ((strcmp(difficulty, "th")) == 0) {
            some_board.size = 15;
            some_board.mine_num = 25;
            some_board.test_run = 1; // test run = 1 means it is developer mode where you can see where all the mines are
            printf("The board is now %iX%i size!\n", some_board.size, some_board.size);
            printf("You have total %i mines to find!\n\n", some_board.mine_num);
            return some_board;
        } else if ((strcmp(difficulty, "tn")) == 0) {
            some_board.size = 10;
            some_board.mine_num = 10;
            some_board.test_run = 1; // test run = 1 means it is developer mode where you can see where all the mines are
            printf("The board is now %iX%i size!\n", some_board.size, some_board.size);
            printf("You have total %i mines to find!\n\n", some_board.mine_num);
            return some_board;
        }
    } while (1);
}

/* Draw board and this board will not be seen to the player */

struct board draw_hboard(struct board h_board)
{
    int i, j;

    for(i = 0; i < h_board.size; i++)
        for(j = 0; j < h_board.size; j++)
            h_board.gboard[i][j] = '_';

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
        if (h_board.gboard[row_random][col_random] == '_') {
            h_board.gboard[row_random][col_random] = '.';
            i++;
        }
    }
    return h_board;
}

struct board draw_pboard(struct board p_board) // Draw the board for players and print it
{
    int i, j;

    // Some initialization of p_board
    p_board.mode = 'o'; //'o' is for open mode 'm' is for mark mode, start with open mode
    p_board.num_mark = 0; //'o' is for open mode 'm' is for mark mode, start with open mode

    for(i = 0; i < p_board.size; i++)
        for(j = 0; j < p_board.size; j++)
            p_board.gboard[i][j] = '_';

    return p_board;
}

void print_board(struct board some_board) // print h_board or p_board
{
    int i, j;
    printf("\n\n    ");
    for (i = 0; i < some_board.size; i++) {
        printf("%3i", i);
    }
    for(i = 0; i < some_board.size; i++) {
        printf("\n%3i ", i);
        for(j = 0; j < some_board.size; j++) {
            printf("  %c", some_board.gboard[i][j]);
        }
        printf("%4i ", i);
    }
    printf("\n    ");
    for (i = 0; i < some_board.size; i++) {
        printf("%3i", i);
    }
    printf("\n");
}

struct board ask_u_action(struct board h_board, struct board p_board)
{
    char u_input[20];
    struct board result;
    system("cls"); // Clearing the screen. Not sure if a machine operating on other than Windows OS is compatible
    print_board(p_board);
    clock_t elapsed_t = clock(); // store the ending time
    int playtime = (elapsed_t - p_board.launch) / CLOCKS_PER_SEC; // convert the time into the seconds
    printf("The total playtime is : %i seconds.\n\n", playtime); // show the playtime
    if (p_board.test_run == 1) {
        print_board(h_board); // for developer's convenience, show the hidden board.
    }
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
        printf("\n\nNow you marked %i fields so far. If you mark %i fields, the rest fields will be open automatically. Be careful!\n", p_board.num_mark, p_board.mine_num);
        result = mark_mine(h_board, p_board);
        return result;
    }
}

struct board open_mine(struct board h_board, struct board p_board)
{
    int row, col, ifvalid, i, j, original_zero_cnt=0, second_zero_cnt=0;
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
    p_board = check_lost(row, col, h_board, p_board);

    if (p_board.gboard[row][col] == '_') {
        p_board.gboard[row][col] = count_nearby_mines(row, col, h_board);
        // check the whole board, and if some fields have no mine at all nearby, check nearby regions of them automatically.
        do {
            original_zero_cnt = second_zero_cnt = 0;
            // first check nearby regions of the field with 0 mine nearby and count how many 0s were there.
            for (i = 0; i < p_board.size; i++) {
                for (j = 0; j < p_board.size; j++) {
                    if (p_board.gboard[i][j] == '0') {
                        p_board = check_nearby_regions(i, j, h_board, p_board); // check nearby regions and update the player board
                        original_zero_cnt++;
                    }
                }
            }
            // and then count how many mines there are after, if second counting is different from first counting do it again. If they are same, break the loop.
            for (i = 0; i < p_board.size; i++) {
                for (j = 0; j < p_board.size; j++) {
                    if (p_board.gboard[i][j] == '0') {
                        second_zero_cnt++;
                    }
                }
            }
        } while(original_zero_cnt != second_zero_cnt);

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

struct board check_lost(int row, int col, struct board h_board, struct board p_board)
{
    int i, j;
    if (h_board.gboard[row][col] == '.') {
        system("cls");
        printf("\nBomb!!! Sorry, but you stepped on a mine!!!(That mine is represented as a 'X'!)\nYou lost! But you are welcome to challenge again!\n\n");
        p_board.done = clock(); // store the ending time
        int playtime = (p_board.done - p_board.launch) / CLOCKS_PER_SEC; // convert the time into the seconds
        printf("The total playtime is : %i seconds.\n\n", playtime); // show the playtime
        // show player the progress by then and where he stepped mine
        printf("The progress you had at the time you lost.");
        p_board.gboard[row][col] = 'X'; // Show where is the mine that player stepped on it
        print_board(p_board);
        printf("\n");
        // show player where was the mines
        for (i = 0; i < p_board.size; i++) {
            for (j = 0; j < p_board.size; j++) {
                if (h_board.gboard[i][j] == '.') {
                    p_board.gboard[i][j] = '.';
                } else {
                    p_board.gboard[i][j] = count_nearby_mines(i, j, h_board);
                }
            }
        }
        printf("What looks like when you win.");
        print_board(p_board);
        printf("\n");
        printf("How the field really was when it was created(Hidden board).");
        print_board(h_board);
        printf("\n\n");
        p_board.mode = 'l'; // mode l means, player lost the game.
        return p_board;
    }
    return p_board; // if you don't return anything here, bad things bappen.
}

struct board mark_mine(struct board h_board, struct board p_board)
{
    int i, j, row, col, ifvalid;
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
    if (p_board.gboard[row][col] == '_') {// If the location is unmarked, mark there.
        p_board.gboard[row][col] = '!';
    } else if (p_board.gboard[row][col] == '!') {// If the location is marked, unmark there.
        p_board.gboard[row][col] = '_';
    }
    // count the mark numbers
    p_board.num_mark=0; // Initialize...
    for (i = 0; i < p_board.size; i++) {
        for (j = 0; j < p_board.size; j++) {
            if (p_board.gboard[i][j] == '!') {
                p_board.num_mark++;
            }
        }
    }
    // check if the user marked as many as the mines are, then open up all the rest part of the board
    if (p_board.num_mark >= p_board.mine_num) {
        for (i = 0; i < p_board.size; i++) {
            for (j = 0; j < p_board.size; j++) {
                if (p_board.gboard[i][j] != '!') {
                    p_board = check_lost(i, j, h_board, p_board);
                    if (p_board.mode == 'l') // player should die at the first mine hit, not last mine hit(it would be zombie-like, if player check all the mines after being hit
                        return p_board;     // This affects the 'X' location at the defeat screen
                    p_board.gboard[i][j] = count_nearby_mines(i, j, h_board);
                }
            }
        }
        p_board = check_win(p_board); // check if user actually won
    }
    return p_board;
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

char count_nearby_mines(int row, int col, struct board h_board)
{
    int i, how_many=0; // count with how_many and then turn it into char type 'mine_count'
    char mine_count; // mine_count will be returned as a char
    // check north west, north, north east mines
    if (h_board.gboard[row][col] != '.') {
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
}

struct board check_nearby_regions(int row, int col, struct board h_board, struct board p_board) // check nearby regions automatically and update the player board, called in open_mine() function.
{
    // check north
    if ((row - 1) >= 0) { // make sure you don't access to outside board
        p_board.gboard[row - 1][col] = count_nearby_mines(row - 1, col, h_board); // check north
        // if ((count_nearby_mines(row - 1, col, h_board)) == '0') // check there s no mine at all again
        //     p_board = check_nearby_regions(row - 1, col, h_board, p_board); // If no mine, call check_nearby_regions recursively. this should loop until it finds some mines nearby
    }
    // check south
    if ((row + 1) <= (h_board.size - 1)) { // make sure you don't access to outside board
        p_board.gboard[row + 1][col] = count_nearby_mines(row + 1, col, h_board); // check south
        // if ((count_nearby_mines(row + 1, col, h_board)) == '0') // check there s no mine at all again
        //     p_board = check_nearby_regions(row + 1, col, h_board, p_board); // If no mine, call check_nearby_regions recursively. this should loop until it finds some mines nearby
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
        printf("Well... What now? Do you wanna challenge again?\n"
               "Choose y for a re-challenge or n for quit like a loser.(y/n): ");
        do {
            dump_line(stdin);
            scanf("%c", &whatnow);
            if (whatnow == 'n') {
                printf("Ok, whatever. Bye-bye quitter.\n");
                getchar();
                getc(stdin);
                system("cls");
                return whatnow;
            } else if (whatnow == 'y') {
                printf("Yesss! I knew you were strong. Let's show your ability this time for real!\n");
                getchar();
                getc(stdin);
                system("cls");
                return whatnow;
            } else {
                printf("Please select only y or n.(y/n): ");
            }
        } while (1);
    } else if (p_board.mode == 'w') {
        if (p_board.size == 10) {
            system("cls");
            printf("Wow, you just won the normal level of this game!!! CONGRATULATION!\n");
            p_board.done = clock(); // store the ending time
            int playtime = (p_board.done - p_board.launch) / CLOCKS_PER_SEC; // convert the time into the seconds
            printf("The total playtime is : %i seconds.\n", playtime); // show the playtime
            print_board(p_board);
            printf("\nDo you want to play again? Maybe you could challenge the hard level.(y/n): ");
            do {
                dump_line(stdin);
                scanf("%c", &whatnow);
                if (whatnow == 'n') {
                    printf("Ok. See you again!\n");
                    getchar();
                    getc(stdin);
                    system("cls");
                    return whatnow;
                } else if (whatnow == 'y') {
                    printf("Yesss!!! Let's sweep another mine field!\n");
                    getchar();
                    getc(stdin);
                    system("cls");
                    return whatnow;
                } else {
                    printf("Please select only y or n.(y/n): ");
                }
            } while (1);
        } else if (p_board.size == 15){
            system("cls");
            printf("incredible!!! You actually just won the HARD level of this game!!!\nCONGRATULATION!!!\n\n");
            p_board.done = clock(); // store the ending time
            int playtime = (p_board.done - p_board.launch) / CLOCKS_PER_SEC; // convert the time into the seconds
            printf("The total playtime is : %i seconds.\n", playtime); // show the playtime
            print_board(p_board);
            printf("\nDo you want to play again? (y/n): ");
            do {
                dump_line(stdin);
                scanf("%c", &whatnow);
                if (whatnow == 'n') {
                    printf("Farewell my friend. If you ever wanna show off your intelligence, you are always welcome!\n");
                    getchar();
                    getc(stdin);
                    system("cls");
                    return whatnow;
                } else if (whatnow == 'y') {
                    printf("Yesss!!! Let's sweep another mine field!\n");
                    getchar();
                    getc(stdin);
                    system("cls");
                    return whatnow;
                } else {
                    printf("Please select only y or n.(y/n): ");
                }
            } while (1);
        }
    }
}

void dump_line(FILE * fp)
{
    int ch;
    while ((ch = fgetc(fp)) != EOF && ch != '\n') {
    /* null body */;
    }
}
