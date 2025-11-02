#ifndef CONNECT4_H
#define CONNECT4_H
void printboard(char board[6][7]);
int drop(char board[6][7], int col, char player, int* count);
int getPositionAfterDrop(char board[6][7], char player);
int checkHorizontal(char board[6][7], char player);
int checkVertical(char board[6][7], char player);
int checkDiagonal(char board[6][7],char player,int col ,int row);
int easybot(char board[6][7]);
int mediumbot(char board[6][7]);
#endif

