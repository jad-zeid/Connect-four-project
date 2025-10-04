#include <stdio.h>
#include "connect4.h"


int main(){
    char board[6][7];
    for (int r = 0; r < 6; r++) { //board initialization
        for (int c = 0; c < 7; c++) {
            board[r][c] = '.';
        }
    }

    printf("Welcome to Connect Four!\n");
    printf("Player A: A\n");
    printf("Player B: B\n");
    printboard(board);
    char player = 'A';
    int col;
    int count=0;
    while(count<42){
        printf("Player %c, choose a column (1-7): ", player);
        scanf("%d", &col);
        while(col<1 || col>7){
            printf("Enter a number between 1 and 7: ");
            scanf("%d", &col);
        }

         int row = drop(board, col,player, &count);
          while (row == -1) { // column full
            printf("Column full! Choose another column: ");
            scanf("%d", &col);
            row = drop(board, col, player, &count);
        }

        //check for winner 
        char winner = checkHorizontal(board, player); //or checkVertical both work but then we change if statements
        if (!winner) { //if no horizontal 4 consecutively achieved then check vertical
            winner = checkVertical(board, player);
        }
        if(!winner){ //if neither horizontal nor vertical, check diagonal
            winner = checkDiagonal(board, player, col-1, row);
        }

        if (winner) { //4 consecutively achieved and we have a winner 
            printf("Player %c wins!\n", winner);
            break;
        }

        if(player == 'A'){
            player = 'B';
        }
        else{
            player = 'A';
        }
    }
    if(count==42){ //all slots were taken and no one achived a 4 horizontally, vertically, or diagonally
        printf("No player wins. Draw!");
    }
}
