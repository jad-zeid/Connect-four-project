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


int mediumbot(char board[6][7]) {
    char bot = 'B';
    char player = 'A';

    // Try to win
    for (int c = 1; c <= 7; c++) {
        if (board[5][c - 1] != 'A' && board[5][c - 1] != 'B') {
            int r;
            for (r = 0; r < 6; r++) {
                if (board[r][c - 1] == '.') {
                    board[r][c - 1] = bot;
                    break;
                }
            }

            if (checkHorizontal(board, bot) == bot ||
                checkVertical(board, bot) == bot ||
                checkDiagonal(board, bot, c - 1, r) == bot) {
                board[r][c - 1] = '.'; // undo
                return c; // play here to win
            }

            board[r][c - 1] = '.'; // undo
        }
    }

    // Try to block opponent’s win
    for (int c = 1; c <= 7; c++) {
        if (board[5][c - 1] != 'A' && board[5][c - 1] != 'B') {
            int r;
            for (r = 0; r < 6; r++) {
                if (board[r][c - 1] == '.') {
                    board[r][c - 1] = player;
                    break;
                }
            }

            if (checkHorizontal(board, player) == player ||
                checkVertical(board, player) == player ||
                checkDiagonal(board, player, c - 1, r) == player) {
                board[r][c - 1] = '.'; // undo
                return c; // block here
            }

            board[r][c - 1] = '.'; // undo
        }
    }

    // Prefer center column if possible
    if (board[5][3] == '.') {
        return 4;
    }

    // Otherwise pick a random valid column
    int validCols[7], count = 0;
    for (int c = 1; c <= 7; c++) {
        if (board[5][c - 1] == '.') {
            validCols[count++] = c;
        }
    }

    if (count == 0) return 1; // fallback
    return validCols[rand() % count];
}

int hardbot(char board[6][7]){
    int bestCol= 4; //center column best way to start the game, higher chance of winning 
    int bestScore= -10000000; //initiliazing to worst possible score since there is no neg infinity in C 

    //hardbot moves
    for(int col=1; col<=7; col++){  //loop through columns
        int countTemp=0; //not related to actual count
        int row= drop(board, col, 'B', &countTemp); //AI dropping piece in column
        
        if(row==-1) 
        continue; //column full, skip

        //checking winning move for hardbot
        if(checkHorizontal(board,'B') || //check horizontal win 
           checkVertical(board,'B') || //check vertical win
           checkDiagonal(board,'B', col-1, row)){ //check diagonal win 
            board[row][col-1]='.'; //undo
            return col; //return column since it is winning move
           }

           //scoring 
           int score=0; //initiliaze score for column 

           if (col==4) //center is strongest
           score+=50; //think of it as weight, the higher the better the move 

           if(col==3 || col==5) //next to center also strong, not as much as center
           score+=25;  

           for(int c=0; c<6; c++){ //check 2 in a row 
            if(board[row][c]=='B' && board[row][c+1]=='B')
            score+=40; 
           }

           for(int c=0; c<5; c++){ //check 3 in a row
            if(board[row][c]=='B' && board[row][c+1]=='B' && board[row][c+2]=='B')
            score+=100; 
           }

           board[row][col-1]='.'; //undo

           if(score>bestScore){ //keep track of column with best score
            bestScore=score; //update bestscore
            bestCol=col; //update bestcol
           }
    }

    //block player A moves
    for(int col=1; col<=7; col++){ //loop through columns 
        int countTemp=0;
        int row= drop(board, col, 'A', &countTemp); //player A's move

        if(row==-1)
        continue; //column is full, skip

        //check win move for player A
        if(checkHorizontal(board, 'A') || //check horizontal win
           checkVertical(board, 'A') || //check vertical win
           checkDiagonal(board, 'A', col-1, row)){  //check diagonal win

            board[row][col-1]='.'; //undo
            return col; //place in column and block player A
           }

           board[row][col-1]=='.'; //undo
    }

    return bestCol; //place piece in column since it has highest column and highest chance of winning 
}

