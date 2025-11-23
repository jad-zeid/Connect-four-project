#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "connect4.h"

#define ROWS 6
#define COLS 7

// Initialize board
void initializeBoard(char board[ROWS][COLS]) {
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            board[r][c] = '.';
        }
    }
}

// Switch player turn
char switchPlayer(char current) {
    return (current == 'A') ? 'B' : 'A';
}

// Get user column input safely
int getPlayerMove(char player) {
    int col;
    printf("Player %c, choose a column (1-7): ", player);
    while (scanf("%d", &col) != 1 || col < 1 || col > 7) {
        while (getchar() != '\n'); // clear buffer
        printf("Invalid input. Enter a number between 1-7: ");
    }
    return col;
}

int main() {
    char board[ROWS][COLS];
    initializeBoard(board);

    int mode;
    printf("Welcome to Connect Four!\n");
    printf("Select mode:\n");
    printf("1 → Player vs Player\n");
    printf("2 → Player vs Bot (Easy)\n");
    printf("3 → Player vs Bot (Medium)\n");
    printf("4 → Player vs Bot (Hard)\n");

    while (scanf("%d", &mode) != 1 || mode < 1 || mode > 4) {
        while (getchar() != '\n'); // flush invalid input
        printf("Invalid choice. Enter 1, 2, 3 or 4: ");
    }

    srand(time(NULL));

    printf("\nPlayer A: A");
    if (mode == 1) printf("\nPlayer B: B");
    else printf("\nPlayer B: Bot");
    printf("\n\n");

    printboard(board);

    char player = 'A';
    int count = 0;
    while (count < ROWS * COLS) {
        int col;

        // Bot moves
        if ((mode == 2 || mode == 3 || mode == 4) && player == 'B') {
            if (mode == 2)
                col = easybot(board);
            else if (mode == 3)
                col = mediumbot(board);
                else 
                col = hardbot(board);

            printf("\nBot chose column %d\n", col);
        } 
        // Human move
        else {
            col = getPlayerMove(player);
        }

        int row = drop(board, col, player, &count);
        while (row == -1) { // handle full column
            if ((mode == 2 || mode == 3 || mode == 4) && player == 'B') {
                 if (mode == 2)
            col = easybot(board);
        else if (mode == 3)
            col = mediumbot(board);
        else
            col = hardbot(board);

                printf("Column full, bot chose column %d instead\n", col);

                row = drop(board, col, player, &count);
                
            } else {
                printf("Column full! Choose another column: ");
                col = getPlayerMove(player);
                row = drop(board, col, player, &count);
            }
        }

        printf("\n");
        printboard(board);
        fflush(stdout); // fixes display delay in Alpine/TTY

        // Check for winner
        char winner = checkHorizontal(board, player);
        if (!winner) winner = checkVertical(board, player);
        if (!winner) winner = checkDiagonal(board, player, col - 1, row);

        if (winner) {
            printf("\n Player %c wins!\n", winner);
            break;
        }

        player = switchPlayer(player);
    }

    if (count == ROWS * COLS) {
        printf("\nIt's a draw!\n");
    }


    return 0;
}
