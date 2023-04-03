#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <stdbool.h>

char findOpposite(char colour);
void copyBoard(char board[][26], char bd[][26], int n);
int countScore(char board[][26], int n, char colour);
bool moveIsValid(char board[][26], int n, char colour, int row, int col);
void checkValidAndFlip(char board[][26], int n, int row, int col, char colour);
void flipInDirection(char board[][26], int n, int row, int col, char colour, int deltaRow, int deltaCol);
int minimax(char bd[][26], int n, char turn, int num);
int lastMove(char bd[][26], int n, char turn);
void firstAvailableMove(char board[][26], int n, char colour, int h_row, int h_col);
bool moveAvailable(char board[][26] ,int n, char colour);
bool xSquares(int row, int col, int n);

/*****************************************************/

void printBoard(char board[][26], int n) {
  char ch = 97;
  printf("  ");
  for(char ch = 97; ch < 97+n; ch++){
    printf("%c" ,ch);
  }
  printf("\n");
  for(int i = 0; i < n; i++){
    printf("%c " ,ch+i);
    for(int j = 0; j < n; j++){
      printf("%c" ,board[i][j]);
    }
    printf("\n");
  }
}

bool positionInBounds(int n, int row, int col) {
  if(row < 0 || row >= n)
      return false;
  else if(col < 0 || col >= n)
      return false;
  else
      return true;
}

bool checkLegalInDirection(char board[][26], int n, int row, int col, char colour, int deltaRow, int deltaCol){
  if(board[row+deltaRow][col+deltaCol] != findOpposite(colour)){
      return false;
  }
  else if(positionInBounds(n, (row+deltaRow), (col+deltaCol)) == false){
      return false;
  }
  else{
      for(int x = 1; x < n; x++){
          if(board[row+deltaRow*x][col+deltaCol*x] == 'U'){
              return false;
          }
          else if(positionInBounds(n, (row+deltaRow*x), (col+deltaCol*x)) == false){
              return false;
          }
          else if(board[row+deltaRow*x][col+deltaCol*x] == (char)colour){
              return true;
          }
      }
    return false;
  }
}

int makeMove(const char board[][26], int n, char turn, int *row, int *col) {
  int highest_rank = 0, h_row, h_col, rank;
  char bd[26][26] = {0};
  for(int i = 0; i < n; i++){
    for(int j = 0; j < n; j++){
      if(moveIsValid(board, n, turn, i, j)){
        copyBoard(board, bd, n);
        int before = countScore(bd, n, turn);
        checkValidAndFlip(bd, n, i, j, turn);
        int after = countScore(bd, n, turn);
        int tiles_flipped = after - before - 1;
        rank = tiles_flipped - minimax(bd, n, findOpposite(turn), 5);
        if((i == 0 && j == 0) || (i == 0 && j == n-1) || (i == n-1 && j == 0) || (i == n-1 && j == n-1)){
          rank = rank + 4;
        }
        if(xSquares(i, j, n)){
          rank = rank - 2;
        }
        if(i == 0 || i == n-1){
          rank = rank + 2;
        }
        if(j == 0 || j == n-1){
          rank = rank + 2;
        }
        if((i == (n/2-1) && j == (n/2-1)) || (i == n/2 && j == n/2)){
          rank++;
        }
        if((i == (n/2-1) && j == n/2) || (i == n/2 && j == (n/2-1))){
          rank++;
        }
        if(rank >= highest_rank){
          highest_rank = rank;
          h_row = i;
          h_col = j;
        }
      }
    }
  }
  if( rank < 0){
    firstAvailableMove(board, n, turn, h_row, h_col);
    *row = h_row;
    *col = h_col;
  }
  else{
    *row = h_row;
    *col = h_col;
  }
}

int minimax(char bd[][26], int n, char turn, int num){
  int highest_score = 0, h_row, h_col, rank;
  char bdt[26][26] = {0};
  if(num == 1){
    lastMove(bdt, n, findOpposite(turn));
  }
  else{
    if(moveAvailable(bd, n, turn)){
      for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
          if(moveIsValid(bd, n, turn, i, j)){
            copyBoard(bd, bdt, n);
            int before = countScore(bdt, n, turn);
            checkValidAndFlip(bdt, n, i, j, turn);
            int after = countScore(bdt, n, turn);
            int tiles_flipped = after - before - 1;
            if(num%2 == 0){
              rank = tiles_flipped + minimax(bdt, n, findOpposite(turn), num-1);
            }
            else{
              rank = tiles_flipped - minimax(bdt, n, findOpposite(turn), num-1);
            }
            if((i == 0 && j == 0) || (i == 0 && j == n-1) || (i == n-1 && j == 0) || (i == n-1 && j == n-1)){
              rank = rank + 2;
            }
            if(xSquares(i, j, n)){
              rank = rank - 2;
            }
            if(i == 0 || i == n-1){
              rank++;
            }
            if(j == 0 || j == n-1){
              rank++;
            }
            if(i == 1 || i == n-2){
              rank--;
            }
            if(j == 1 || j == n-2){
              rank--;
            }
            if((i == (n/2-1) && j == (n/2-1)) || (i == n/2 && j == n/2)){
              rank++;
            }
            if((i == (n/2-1) && j == n/2) || (i == n/2 && j == (n/2-1))){
              rank++;
            }
            if(rank >= highest_score){
              highest_score = rank;
            }
          }
        }
      }
      return highest_score;
    }
    else{
      return 0;
    }
  }
}

int lastMove(char bd[][26], int n, char turn){
  int highest_score = 0, h_row, h_col;
  char bdt[26][26] = {0};
  if(moveAvailable(bd, n, turn)){
    for(int i = 0; i < n; i++){
      for(int j = 0; j < n; j++){
        if(moveIsValid(bd, n, turn, i, j)){
          copyBoard(bd, bdt, n);
          int before = countScore(bdt, n, turn);
          checkValidAndFlip(bdt, n, i, j, turn);
          int after = countScore(bdt, n, turn);
          int tiles_flipped = after - before - 1;
          if((i == 0 && j == 0) || (i == 0 && j == n-1) || (i == n-1 && j == 0) || (i == n-1 && j == n-1)){
            tiles_flipped = tiles_flipped + 2;
          }
          if(xSquares(i, j, n)){
            tiles_flipped = tiles_flipped - 2;
          }
          if(i == 0 || i == n-1){
            tiles_flipped++;
          }
          if(j == 0 || j == n-1){
            tiles_flipped++;
          }
          if(i == 1 || i == n-2){
            tiles_flipped--;
          }
          if(j == 1 || j == n-2){
            tiles_flipped--;
          }
          if((i == (n/2-1) && j == (n/2-1)) || (i == n/2 && j == n/2)){
            tiles_flipped++;
          }
          if((i == (n/2-1) && j == n/2) || (i == n/2 && j == (n/2-1))){
            tiles_flipped++;
          }
          if(tiles_flipped >= highest_score){
            highest_score = tiles_flipped;
          }
        }
      }
    }
    return highest_score;
  }
  else{
    return 0;
  }
}

char findOpposite(char colour){
  if(colour == (char)'W')
      return (char)'B';
  else
  return (char)'W';
}

void copyBoard(char board[][26], char bd[][26], int n){
  for(int i = 0; i < n; i++){
    for(int j = 0; j < n; j++){
      bd[i][j] = board[i][j];
    }
  }
}

int countScore(char board[][26], int n, char colour){
  int score = 0;
  for(int i = 0; i < n; i++){
    for(int j = 0; j < n; j++){
      if(board[i][j] == colour){
        score++;
      }
    }
  }
  return score;
}

bool moveIsValid(char board[][26], int n, char colour, int row, int col){
  bool check = false;
  if(board[row][col] == 'U'){
      if(checkLegalInDirection(board, n, row, col, colour, 1, 0) && check == false){
          check = true;
          return true;
      }
      else if((checkLegalInDirection(board, n, row, col, colour, 1, -1) && check == false)){
          check = true;
          return true;
      }
      else if((checkLegalInDirection(board, n, row, col, colour, 0, -1) && check == false)){
          check = true;
          return true;
      }
      else if((checkLegalInDirection(board, n, row, col, colour, -1, -1) && check == false)){
          check = true;
          return true;
      }
      else if((checkLegalInDirection(board, n, row, col, colour, -1, 0) && check == false)){
          check = true;
          return true;
      }
      else if((checkLegalInDirection(board, n, row, col, colour, -1, 1) && check == false)){
          check = true;
          return true;
      }
      else if((checkLegalInDirection(board, n, row, col, colour, 0, 1) && check == false)){
          check = true;
          return true;
      }
      else if((checkLegalInDirection(board, n, row, col, colour, 1, 1) && check == false)){
          check = true;
          return true;
      }
    }
    return false;
}

void checkValidAndFlip(char board[][26], int n, int row, int col, char colour){
  board[row][col] = colour;
  while(checkLegalInDirection(board, n, row, col, colour, 1, 0)){
    flipInDirection(board, n, row, col, colour, 1, 0);
  }
  while(checkLegalInDirection(board, n, row, col, colour, 1, -1)){
    flipInDirection(board, n, row, col, colour, 1, -1);
  }
  while(checkLegalInDirection(board, n, row, col, colour, 0, -1)){
    flipInDirection(board, n, row, col, colour, 0, -1);
  }
  while(checkLegalInDirection(board, n, row, col, colour, -1, -1)){
    flipInDirection(board, n, row, col, colour, -1, -1);
  }
  while(checkLegalInDirection(board, n, row, col, colour, -1, 0)){
    flipInDirection(board, n, row, col, colour, -1, 0);
  }
  while(checkLegalInDirection(board, n, row, col, colour, -1, 1)){
    flipInDirection(board, n, row, col, colour, -1, 1);
  }
  while(checkLegalInDirection(board, n, row, col, colour, 0, 1)){
    flipInDirection(board, n, row, col, colour, 0, 1);
  }
  while(checkLegalInDirection(board, n, row, col, colour, 1, 1)){
    flipInDirection(board, n, row, col, colour, 1, 1);
  }
}

void flipInDirection(char board[][26], int n, int row, int col, char colour, int deltaRow, int deltaCol){
  for(int x = 1; x < n; x++){
    if(board[row+deltaRow*x][col+deltaCol*x] != colour){
      board[row+deltaRow*x][col+deltaCol*x] = colour;
    }
    else{
      return;
    }
  }
}

void firstAvailableMove(char board[][26], int n, char colour, int h_row, int h_col){
  for(int i = 0; i < n; i++){
    for(int j = 0; j < n; j++){
      if(moveIsValid(board, n, colour, i , j)){
        h_row = i;
        h_col = j;
        return;
      }
    }
  }
}

bool moveAvailable(char board[][26] ,int n, char colour){
  for(int i = 0; i < n; i++){
    for(int j = 0; j < n; j++){
      if(moveIsValid(board, n, colour, i, j)){
        return true;
      }
    }
  }
  return false;
}

bool xSquares(int row, int col, int n){
  if((row == 0 && col == 1) || (row == 1 && col == 1) || (row == 1 && col == 0)){
    return true;
  }
  if((row == n-2 && col == 0) || (row == n-2 && col == 1) || (row == n-1 && col == 1)){
    return true;
  }
  if((row == 0 && col == n-2) || (row == 1 && col == n-2) || (row == 1 && col == n-1)){
    return true;
  }
  if((row == n-2 && col == n-1) || (row == n-2 && col == n-2) || (row == n-1 && col == n-2)){
    return true;
  }
  return false;
}