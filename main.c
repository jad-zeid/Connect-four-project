#include <stdio.h>
#include "connect4.h"


int main(){
    char board[6][7];
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
        drop(board, col,player, &count);
        if(player == 'A'){
            player = 'B';
        }
        else{
            player = 'A';
        }
    }
}
