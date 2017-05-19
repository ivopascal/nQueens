#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 0
#define MIN 1

#define INFINITY 9999999

typedef struct Choice {
	int move;
	int value;
}Choice;

int minValue(int state); /* forward declaration: mutual recursion */

int maxValue(int state) {
  int move, max = -INFINITY;
  /* terminal state ? */
  if (state == 1) {
    return -1; /* Min wins if max is in a terminal state */
  }
  /* non-terminal state */
  for (move = 1; move <= 3; move++) {
    if (state - move > 0) { /* legal move */
      int m = minValue(state - move);
      if (m > max) max = m;
    }
  }
  return max;
}

int minValue(int state) {
  int move, min = INFINITY;
  /* terminal state ? */
  if (state == 1) {
    return 1; /* Max wins if min is in a terminal state */
  } 
  /* non-terminal state */  
  for (move = 1; move <= 3; move++) {
    if (state - move > 0) { /* legal move */
      int m = maxValue(state - move);
      if (m < min) min = m;
    }
  }
  return min;
}

int minimaxDecision(int state, int turn) {
  int move, bestmove, max, min;
  if (turn == MAX) {
    max = -INFINITY;
    for (move = 1; move <= 3; move++) {
      if (state - move > 0) { /* legal move */
        int m = minValue(state - move);
        if (m > max) {
          max = m;
          bestmove = move;
        }
      }
    }
    return bestmove;
  }
  /* turn == MIN */
  min = INFINITY;
  for (move = 1; move <= 3; move++) {
    if (state - move > 0) { /* legal move */
      int m = maxValue(state - move);
      if (m < min) {
        min = m;
        bestmove = move;
      }
    }
  }
  return bestmove;
}

void initTable(Choice t[101][2]){
	for (int i = 0; i<101; i++){
		for (int j = 0; j<2; j++){
			t[i][j].move = 0;
		}
	}
}

int preCalculated(Choice transTable[101][2], int turn, int state){
	return (transTable[state][turn == -1].move);
}

Choice negaMax(int state, int turn, Choice transTable[101][2]){
	Choice c;
	if(state == 1){
		c.value = -turn; 
		c.move = 1;
		return c;
	}
	if(!preCalculated(transTable, turn, state)){		//calculate only if the value is unknown
		int move, bestmove, ext;
		ext = -INFINITY;
		int m = 0;
		for (move = 3; move >= 1; move--) {		//turned around to start from the biggest move for shorter sequence
		  if (state - move > 0) { /* legal move */
			m = negaMax(state - move, -turn, transTable).value;
			if (turn * m > ext) {
			  ext = m;
			  bestmove = move;
			}
			if (m == turn){						//no point in searching further if a winning value has been found
				break;
			}
		  }
		 }
		 transTable[state][turn == -1].move = bestmove;	//adding calculated values to the transposition table
		 transTable[state][turn == -1].value = ext;
	}
    c.move = transTable[state][turn == -1].move;
    c.value = transTable[state][turn == -1].value;
    
    return c;
}

void playNim(int state) {
  int turn = 0;
  while (state != 1) {
    int action = minimaxDecision(state, turn);
    printf("%d: %s takes %d\n", state, 
           (turn==MAX ? "Max" : "Min"), action);
    state = state - action;
    turn = 1 - turn;
  }
  printf("1: %s looses\n", (turn==MAX ? "Max" : "Min"));
}

void PlayNegaMax(int state, Choice transTable[101][2]){
  int turn = 1;
  while (state != 1) {
    Choice action = negaMax(state, turn, transTable);
    printf("%d: %s takes %d for value %d\n", state, 
           (turn==1 ? "Max" : "Min"), action.move, action.value);
    state = state - action.move;
    turn = -turn;
  }
  printf("1: %s looses\n", (turn==1 ? "Max" : "Min"));
}

int main(int argc, char *argv[]) {
  char method[10];
  Choice transTable[101][2];		//transposition table for best choices for Min and Max
  initTable(transTable);
  if ((argc != 2) || (atoi(argv[1]) < 3)) {
    fprintf(stderr, "Usage: %s <number of sticks>, where ", argv[0]);
    fprintf(stderr, "<number of sticks> must be at least 3!\n");
    return -1;
  }
  do{
	printf("choose a method: Classic|Nega\n");
	scanf("%s", method);
  }while(strcmp(method, "Classic")&&strcmp(method, "Nega"));
	
  if(!strcmp(method, "Classic")){	
	playNim(atoi(argv[1]));
  }else{
	PlayNegaMax(atoi(argv[1]), transTable);
  }

  return 0;
}
