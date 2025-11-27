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

// Score a group of 4 cells to check for winning patterns and threats
int scoreFourCells(char window[4], char player, char opponent) {
    int score = 0;
    int playerCount = 0;
    int opponentCount = 0;
    int emptyCount = 0;
    
    for (int c = 0; c < 4; c++) {
        if (window[c] == player) playerCount++;
        else if (window[c] == opponent) opponentCount++;
        else emptyCount++;
    }
    
    // Scoring based on piece count
    if (playerCount == 4) score += 100000; // winning
    else if (playerCount == 3 && emptyCount == 1) score += 100; // 3 in a row with open spot
    else if (playerCount == 2 && emptyCount == 2) score += 40; // 2 in a row with open spots
    
    // Penalize opponent threats
    if (opponentCount == 3 && emptyCount == 1) score -= 80; // block opponent 3 in a row
    
    return score;
}

// Score the entire board to determine who has the advantage
int scoreBoardPosition(char board[6][7], char player, char opponent) {
    int score = 0;
    char window[4];
    
    // Center column preference (col index 3 is center)
    for (int row = 0; row < 6; row++) {
        if (board[row][3] == player) score += 50;
    }
    
    // Next to center columns (col 2 and 4)
    for (int row = 0; row < 6; row++) {
        if (board[row][2] == player) score += 25;
        if (board[row][4] == player) score += 25;
    }
    
    // Horizontal scoring
    for (int row = 0; row < 6; row++) {
        for (int c = 0; c < 4; c++) {
            for (int i = 0; i < 4; i++) window[i] = board[row][c + i];
            score += scoreFourCells(window, player, opponent);
        }
    }
    
    // Vertical scoring
    for (int col = 0; col < 7; col++) {
        for (int row = 0; row < 3; row++) {
            for (int i = 0; i < 4; i++) window[i] = board[row + i][col];
            score += scoreFourCells(window, player, opponent);
        }
    }
    
    // Diagonal (bottom-left to top-right) scoring
    for (int row = 0; row < 3; row++) {
        for (int c = 0; c < 4; c++) {
            for (int i = 0; i < 4; i++) window[i] = board[row + i][c + i];
            score += scoreFourCells(window, player, opponent);
        }
    }
    
    // Diagonal (top-left to bottom-right) scoring
    for (int row = 3; row < 6; row++) {
        for (int c = 0; c < 4; c++) {
            for (int i = 0; i < 4; i++) window[i] = board[row - i][c + i];
            score += scoreFourCells(window, player, opponent);
        }
    }
    
    return score;
}

// Check if the board is full (draw)
int isBoardFull(char board[6][7]) {
    for (int col = 0; col < 7; col++) {
        if (board[5][col] != 'A' && board[5][col] != 'B') return 0;
    }
    return 1;
}

// Check if any player has won
int checkWinner(char board[6][7], char player) {
    // Check horizontal
    if (checkHorizontal(board, player)) return 1;
    // Check vertical
    if (checkVertical(board, player)) return 1;
    // Check all diagonals
    for (int row = 0; row < 6; row++) {
        for (int col = 0; col < 7; col++) {
            if (board[row][col] == player) {
                if (checkDiagonal(board, player, col, row)) return 1;
            }
        }
    }
    return 0;
}

// Minimax with Alpha-Beta Pruning
int minimax(char board[6][7], int depth, int alpha, int beta, int isMaximizing, char bot, char player) {
    // Terminal conditions
    if (checkWinner(board, bot)) return 100000 + depth; // Bot wins (prefer faster wins)
    if (checkWinner(board, player)) return -100000 - depth; // Player wins (avoid faster losses)
    if (isBoardFull(board) || depth == 0) return scoreBoardPosition(board, bot, player); // Draw or depth limit
    
    if (isMaximizing) {
        int bestScore = -10000000; // worst possible score for maximizer
        for (int col = 1; col <= 7; col++) {
            int countTemp = 0;
            int row = drop(board, col, bot, &countTemp);
            if (row == -1) continue; // column full, skip
            
            int score = minimax(board, depth - 1, alpha, beta, 0, bot, player);
            board[row][col - 1] = '.'; // undo move
            
            if (score > bestScore) bestScore = score;
            if (score > alpha) alpha = score;
            if (beta <= alpha) break; // Alpha-Beta pruning
        }
        return bestScore;
    } else {
        int bestScore = 10000000; // worst possible score for minimizer
        for (int col = 1; col <= 7; col++) {
            int countTemp = 0;
            int row = drop(board, col, player, &countTemp);
            if (row == -1) continue; // column full, skip
            
            int score = minimax(board, depth - 1, alpha, beta, 1, bot, player);
            board[row][col - 1] = '.'; // undo move
            
            if (score < bestScore) bestScore = score;
            if (score < beta) beta = score;
            if (beta <= alpha) break; // Alpha-Beta pruning
        }
        return bestScore;
    }
}

int hardbot(char board[6][7]){
    int bestCol = 4; // center column best way to start the game, higher chance of winning
    int bestScore = -10000000; // initializing to worst possible score since there is no neg infinity in C
    int alpha = -10000000; // alpha for alpha-beta pruning
    int beta = 10000000; // beta for alpha-beta pruning
    int depth = 6; // search depth for minimax (increase for stronger AI, decrease for faster)
    
    // First check for immediate winning move for hardbot
    for (int col = 1; col <= 7; col++) {
        int countTemp = 0;
        int row = drop(board, col, 'B', &countTemp);
        
        if (row == -1) continue; // column full, skip
        
        // checking winning move for hardbot
        if (checkHorizontal(board, 'B') || // check horizontal win
            checkVertical(board, 'B') || // check vertical win
            checkDiagonal(board, 'B', col - 1, row)) { // check diagonal win
            board[row][col - 1] = '.'; // undo
            return col; // return column since it is winning move
        }
        board[row][col - 1] = '.'; // undo
    }
    
    // Block player A's immediate winning move
    for (int col = 1; col <= 7; col++) {
        int countTemp = 0;
        int row = drop(board, col, 'A', &countTemp); // player A's move
        
        if (row == -1) continue; // column is full, skip
        
        // check win move for player A
        if (checkHorizontal(board, 'A') || // check horizontal win
            checkVertical(board, 'A') || // check vertical win
            checkDiagonal(board, 'A', col - 1, row)) { // check diagonal win
            board[row][col - 1] = '.'; // undo
            return col; // place in column and block player A
        }
        board[row][col - 1] = '.'; // undo
    }
    
    // Use minimax with alpha-beta pruning to find best move
    for (int col = 1; col <= 7; col++) {
        int countTemp = 0;
        int row = drop(board, col, 'B', &countTemp); // AI dropping piece in column
        
        if (row == -1) continue; // column full, skip
        
        // Run minimax to get score for this move
        int score = minimax(board, depth - 1, alpha, beta, 0, 'B', 'A');
        
        board[row][col - 1] = '.'; // undo
        
        if (score > bestScore) { // keep track of column with best score
            bestScore = score; // update bestscore
            bestCol = col; // update bestcol
        }
        
        // Update alpha for root level pruning
        if (score > alpha) alpha = score;
    }
    
    return bestCol; // place piece in column since it has highest score and highest chance of winning
}

/*
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

        for (int c = 0; c < 4; c++){ //check open 3s
            if (board[row][c]=='B' && board[row][c+1]=='B' && board[row][c+2]=='B' && board[row][c+3]=='.') //left to right
                score += 200;

            if (board[row][c]=='.' && board[row][c+1]=='B' && board[row][c+2]=='B' && board[row][c+3]=='B') //right to left
                score += 200;
        }

        //check vertical
        // 2 vertical potential, 1 piece under
        if (row <= 4 && board[row+1][col-1]=='B')
            score += 30;

        // 3 vertical potential, 2 pieces under
        if (row <= 3 && board[row+1][col-1]=='B' && board[row+2][col-1]=='B')
            score += 80;

        //check diagonal
        
        // diagonal bottom left to top right pattern
        if (col <= 6 && row <= 4 && board[row+1][col]=='B') 
            score += 40;

        if (col <= 5 && row <= 3 && board[row+1][col]=='B' && board[row+2][col+1]=='B')
            score += 60;

        // diagonal top right to bottom left pattern
        if (col >= 2 && row <= 4 && board[row+1][col-2]=='B')
            score += 40;

        if (col >= 3 && row <= 3 && board[row+1][col-2]=='B' && board[row+2][col-3]=='B')
            score += 60;

        if (row >= 1 && board[row-1][col-1] == 'A') //avoid any move that lets player A win
            score -= 100;

        //check any move that might give player A chance to win in next piece drop
        int lose = 0;
        for (int oppCol = 1; oppCol <= 7; oppCol++)
        {
            int x = 0;
            int oppRow = drop(board, oppCol, 'A', &x);

            if (oppRow != -1)
            {
                //very bad play if player A can win in next move
                if (checkHorizontal(board,'A') ||
                    checkVertical(board,'A')   ||
                    checkDiagonal(board,'A', oppCol-1, oppRow))
                    lose = 1;

                board[oppRow][oppCol-1] = '.'; // undo
            }
        }
        if (lose)
            score -= 500; //very bad move


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

           board[row][col-1]='.'; //undo
    }

    return bestCol; //place piece in column since it has highest column and highest chance of winning 
}
    */