#include <stdio.h>
#include <stdlib.h>
#include "connect4.h"

//display the board of the game
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

//drop piece
int drop(char board[6][7], int col, char player, int* count){
    col = col-1;
    for(int r=0;r<6;r++){
        char place = board[r][col];
        if(place !='A' && place != 'B'){
            board[r][col]=player;
            printboard(board);
            (*count)++;
            return r;
        }
    }
    return -1;
}

int checkHorizontal(char board[6][7], char player){
    for(int r=0; r<6; r++){ //loop through all the rows
        for(int c=0; c<=3; c++){ //checks horizontal from column 0-3, if c=0 then it checks (0,1,2,3) thats why c<=3 since c=3 checks (3,4,5,6) hence covering all possible 4s in a row
            if(board[r][c] == player && //current location the player is at
               board[r][c+1] == player && //next column 
               board[r][c+2] == player && //2 columns next
               board[r][c+3] == player){ //3 columns next
                return player; //if all 4 are true then the player achieved 4 in a row and the system returns the player who won(A or B)
               }
        }
    }
    return '\0'; //no horizontal 4 in a row achieved 
}

int checkVertical(char board[6][7], char player){
    for (int c = 0; c < 7; c++) { //loop through all the columns 
        for (int r = 0; r <= 2; r++) { //check vertical from row 0-2, r=0 checks (0,1,2,3) the next 4rows therefore r<=2 since r=3 is invalid(checks 3,4,5,6->invalid)
            if (board[r][c] == player && //current location of the player (last checker played)
                board[r+1][c] == player && //next row
                board[r+2][c] == player && // 2rows next
                board[r+3][c] == player) { //3rows next
                return player; //player achieved vertical consecutive 4 and the system returns the player that won (A or B)
            }
        }
    }
    return '\0'; // no vertical consecutive 4 achieved 
}

    int checkDiagonal(char board[6][7],char player,int col,int row){
    int count = 1;
    

    // Check \ diagonal (top-left to bottom-right)

    // Up-left
    int r = row - 1;
    int c = col - 1;
    while (r >= 0 && c >= 0 && board[r][c] == player) {
        count++;
        r--;
        c--;
    }

    // Down-right
    r = row + 1;
    c = col + 1;
    while (r < 6 && c < 7 && board[r][c] == player) {
        count++;
        r++;
        c++;
    }

    if (count >= 4) {
        return player;
    }

    // Reset count for / diagonal
    count = 1;

    // Up-right
    r = row - 1;
    c = col + 1;
    while (r >= 0 && c < 7 && board[r][c] == player) {
        count++;
        r--;
        c++;
    }

    // Down-left
    r = row + 1;
    c = col - 1;
    while (r < 6 && c >= 0 && board[r][c] == player) {
        count++;
        r++;
        c--;
    }

    if (count >= 4) {
        return player;
    }

    return '\0';
}

// make the bot choose a random column that isnt full
int easybot(char board[6][7]){
    int col;
    int valid =0;
    do{
        col =(rand() % 7)+1;
        if (board[5][col-1]!= 'A' && board[5][col-1]!= 'B' ){
            valid=1;
        }
    }while (valid == 0);
    return col;
}


