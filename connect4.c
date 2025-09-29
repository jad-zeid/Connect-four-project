#include <stdio.h>
#include "connect4.h"

void printboard(char board[6][7]){
    for(int r=5;r>=0;r--){
        for(int c=0;c<7;c++){
            char cur = board[r][c];
            char place;
            if(cur =='A'||cur=='B'){
                place = cur;
            }
            else{
                place='.';
            }
            printf(" %c ", place);
        }
        printf("\n");
    }
    for(int c=1; c<=7;c++){
        printf(" %d ", c);
    }
    printf("\n");
}

void drop(char board[6][7], int col, char player, int* count){
    col = col-1;
    for(int r=0;r<6;r++){
        char place = board[r][col];
        if(place !='A' && place != 'B'){
            board[r][col]=player;
            printboard(board);
            (*count)++;
            return;
        }
    }
}