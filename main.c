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
        drop(board, col,player, &count);
        if(player == 'A'){
            player = 'B';
        }
        else{
            player = 'A';
        }
    }
}
