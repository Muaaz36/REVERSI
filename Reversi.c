#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <stdbool.h>

void printBoard(char board[][26], int n);
void initializeBoard(char board[][26], int n);
void computers_move(char board[][26], int n, char colour);
void humans_move(char board[][26], int n, char colour, char move[]);
char findOpposite(char colour);
bool moveIsValid(char board[][26], int n, char colour, int row, int col);
bool checkLegalInDirection(char board[][26], int n, int row, int col, char colour, int deltaRow, int deltaCol);
void checkValidAndFlip(char board[][26], int n, int row, int col, char colour);
void flipInDirection(char board[][26], int n, int row, int col, char colour, int deltaRow, int deltaCol);
bool positionInBounds(int n, int row, int col);
void declareWinner(int c_score, int h_score, char computer_colour);
int countScore(char board[][26], int n, char colour);
bool gameIsNotOver(char board[][26], int n, char colour, int c, int h);
bool moveAvailable(char board[][26] ,int n, char colour);

int main(){
  int n, computer_score, human_score;
  char computer_colour;
  char board[26][26] = {0};
  char move[3] = {0};
  printf("Enter the board dimension: ");
  scanf("%d", &n);
  printf("Computer plays (B/W): ");
  scanf(" %c", &computer_colour);
  initializeBoard(board, n);
  printBoard(board, n);
  char turn = 'B';
  bool game_is_not_over = true;
  bool valid_move = true;
  while(game_is_not_over && valid_move){
    if(turn == computer_colour){
      computers_move(board, n, computer_colour);
    }
    else{
      printf("Enter move for colour %c (RowCol): ", turn);
      scanf("%s", move);
      if(moveIsValid(board, n, turn, move[0]-97, move[1]-97)){
        humans_move(board, n, turn, move);
      }
      else{
        printf("Invalid move.\n");
        valid_move = false;
      }
    }
    if(game_is_not_over){
      if(moveAvailable(board, n, findOpposite(turn))){
        turn = findOpposite(turn);
      }
      else if(moveAvailable(board, n, turn)){
        turn = turn;
        printf("%c player has no valid move.\n", findOpposite(turn));
      }
      else{
        game_is_not_over = false;
        computer_score = countScore(board, n, computer_colour);
        human_score = countScore(board, n, findOpposite(computer_colour));
        declareWinner(computer_score, human_score, computer_colour);
      }
    }
  }
  if(valid_move == false){
    printf("%c player wins.", computer_colour);
  }
}
char findOpposite(char colour){
    if(colour == (char)'W')
        return (char)'B';
    else
    return (char)'W';
}
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
void initializeBoard(char board[][26], int n){
  for(int i = 0; i < n; i++){
    for(int j = 0; j < n; j++){
      if((i == (n/2-1) && j == (n/2-1)) || (i == n/2 && j == n/2)){
        board[i][j] = 'W';
      }
      else if((i == (n/2-1) && j == n/2) || (i == n/2 && j == (n/2-1))){
        board[i][j] = 'B';
      }
      else{
        board[i][j] = 'U';
      }
    }
  }
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
void computers_move(char board[][26], int n, char colour){
  for(int i = 0; i < n; i++){
    for(int j = 0; j < n; j++){
      if(moveIsValid(board, n, colour, i, j)){
        checkValidAndFlip(board, n, i, j, colour);
        printf("Computer places %c at %c%c.\n", colour, (char)(i+97), (char)(j+97));
        printBoard(board, n);
        return;
      }
    }
  }
}
void humans_move(char board[][26], int n, char colour, char move[]){
  if(moveIsValid(board, n, colour, move[0]-97, move[1]-97)){
    checkValidAndFlip(board, n, move[0]-97, move[1]-97, colour);
    printBoard(board, n);
  }
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
bool positionInBounds(int n, int row, int col){
    if(row < 0 || row >= n)
        return false;
    else if(col < 0 || col >= n)
        return false;
    else
        return true;
}
void declareWinner(int c_score, int h_score, char colour){
  if(c_score > h_score){
    printf("%c player wins.", colour);
  }
  else if(c_score < h_score){
    printf("%c player wins.", findOpposite(colour));
  }
  else{
    printf("Draw!");
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
bool gameIsNotOver(char board[][26], int n, char colour, int c, int h){
  if(c+h == 16){
    return false;
  }
  else if(moveAvailable(board, n, colour) == false && moveAvailable(board, n, findOpposite(colour) == false)){
    return false;
  }
  else{
    return true;
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
