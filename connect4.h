#ifndef CONNECT4_H
#define CONNECT4_H
void printboard(char board[6][7]);
void drop(char board[6][7], int col, char player, int* count);
int checkHorizontal(char board[6][7], char player);
int checkVertical(char board[6][7], char player);
#endif

