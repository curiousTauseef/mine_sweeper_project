/* 2 way of returning board, passing pointer or returning the struct ! preserving all of it ! */

#include<stdio.h>
#include<stdlib.h>
#define MAX 25 // Maximum board size

void welcome(); //Display the welcome message
int set_difficulty(); // Set difficulty and size
int set_minenum(int size); // Set mine number
//struct hboard draw_board(int size); // Draw board and this board will not be seen to the player
void draw_board(int size, char[][MAX]); // Draw board and this board will not be seen to the player

struct hboard // Struct the hidden board, enabling the return of the board
{
    char hboard[MAX][MAX];
};

int main()

{
    int i, j;
    int size, mine_num; // Declare board size, and mine number
    // struct hboard board; // Declare board, its gonna be never bigger than MAX
    char board[MAX][MAX]; // Declare board, its gonna be never bigger than MAX
    welcome(); //Display the welcome message
    size = set_difficulty();
    mine_num = set_minenum(size);
    // board = draw_board(size);
    draw_board(size, board);
    /* for(i = 0; i < size; i++)
        for(j = 0; j < size; j++)
            printf("%c", board.hboard[i][j]);
    */
    for(i = 0; i < size; i++)
        for(j = 0; j < size; j++)
            printf("%c", board[i][j]);

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
            printf("You have total %i mines to find!\n", mine_num);
            return mine_num;
        }
    } while (1);
}

/* Draw board and this board will not be seen to the player */
/*
struct hboard draw_board(int size)
{

    int i, j;
    struct hboard board;

    for(i = 0; i < size; i++)
        for(j = 0; j < size; j++)
            board.hboard[i][j] = 'o';

    return board;
    // Place mines in this board, it remains
    // hidden from user until the game has
    // finished.
    // create_mines();
} */

void draw_board(int size, char *board)
{
    int i, j;

    for(i = 0; i < size; i++)
        for(j = 0; j < size; j++)
            board[i][j] = 'o';

    // Place mines in this board, it remains
    // hidden from user until the game has
    // finished.
    // create_mines();
}
