/* nqueens.c: (c) Arnold Meijster (a.meijster@rug.nl) */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

#define MAXQ 100

#define MAXgenerations 10000
#define MAXindividuals 100

#define FALSE 0
#define TRUE  1

#define ABS(a) ((a) < 0 ? (-(a)) : (a))

int nqueens;      /* number of queens: global variable */
int queens[MAXQ]; /* queen at (r,c) is represented by queens[r] == c */

void initializeRandomGenerator() {
  /* this routine initializes the random generator. You are not
   * supposed to understand this code. You can simply use it.
   */
  time_t t;
  srand((unsigned) time(&t));
}

/* Generate an initial position.
 * If flag == 0, then for each row, a queen is placed in the first column.
 * If flag == 1, then for each row, a queen is placed in a random column.
 */
void initiateQueens(int flag) {
  int q;
  for (q = 0; q < nqueens; q++) {
    queens[q] = (flag == 0? 0 : rand()%nqueens); 
  }
}

/* returns TRUE if position (row0,column0) is in 
 * conflict with (row1,column1), otherwise FALSE.
 */
int inConflict(int row0, int column0, int row1, int column1) {
  if (row0 == row1) return TRUE; /* on same row, */
  if (column0 == column1) return TRUE; /* column, */
  if (ABS(row0-row1) == ABS(column0-column1)) return TRUE;/* diagonal */
  return FALSE; /* no conflict */
}

/* returns TRUE if position (row,col) is in 
 * conflict with any other queen on the board, otherwise FALSE.
 */
int inConflictWithAnotherQueen(int row, int col) {
  int queen;
  for (queen=0; queen < nqueens; queen++) {
    if (inConflict(row, col, queen, queens[queen])) {
      if ((row != queen) || (col != queens[queen])) return TRUE;
    }
  }
  return FALSE;
}

/* print configuration on screen */
void printState() {
  int row, column; 
  printf("\n");
  for(row = 0; row < nqueens; row++) {
    for(column = 0; column < nqueens; column++) {
      if (queens[row] != column) {
        printf (".");
      } else {
        if (inConflictWithAnotherQueen(row, column)) {
          printf("Q");
        } else {
          printf("q");
        }
      }
    }
    printf("\n");
  }
}


/* move queen on row q to specified column, i.e. to (q,column) */
void moveQueen(int queen, int column) {
  if ((queen < 0) || (queen >= nqueens)) {
    fprintf(stderr, "Error in moveQueen: queen=%d "
      "(should be 0<=queen<%d)...Abort.\n", queen, nqueens);
    exit(-1);
  }
  if ((column < 0) || (column >= nqueens)) {
    fprintf(stderr, "Error in moveQueen: column=%d "
      "(should be 0<=column<%d)...Abort.\n", column, nqueens);
    exit(-1);
  }
  queens[queen] = column;
}

/* returns TRUE if queen can be moved to position
 * (queen,column). Note that this routine checks only that 
 * the values of queen and column are valid! It does not test
 * conflicts!
 */
int canMoveTo(int queen, int column) {
  if ((queen < 0) || (queen >= nqueens)) {
    fprintf(stderr, "Error in canMoveTo: queen=%d "
      "(should be 0<=queen<%d)...Abort.\n", queen, nqueens);
    exit(-1);
  }
  if(column < 0 || column >= nqueens) return FALSE;
  if (queens[queen] == column) return FALSE; /* queen already there */
  return TRUE;
}

/* returns the column number of the specified queen */
int columnOfQueen(int queen) {
  if ((queen < 0) || (queen >= nqueens)) {
    fprintf(stderr, "Error in columnOfQueen: queen=%d "
      "(should be 0<=queen<%d)...Abort.\n", queen, nqueens);
    exit(-1);
  }
  return queens[queen];
}

/* returns the number of pairs of queens that are in conflict */
int countConflicts() {
  int cnt = 0;
  int queen, other;
  for (queen=0; queen < nqueens; queen++) {
    for (other=queen+1; other < nqueens; other++) {
      if (inConflict(queen, queens[queen], other, queens[other])) {
        cnt++;
      }
    }
  }
  return cnt;
}

int countConflictsArg(int ind[MAXQ]) {
  int cnt = 0;
  int queen, other;
  for (queen=0; queen < nqueens; queen++) {
    for (other=queen+1; other < nqueens; other++) {
      if (inConflict(queen, ind[queen], other, ind[other])) {
        cnt++;
      }
    }
  }
  return cnt;
}

/* evaluation function. The maximal number of queens in conflict
 * can be 1 + 2 + 3 + 4 + .. + (nquees-1)=(nqueens-1)*nqueens/2.
 * Since we want to do ascending local searches, the evaluation
 * function returns (nqueens-1)*nqueens/2 - countConflicts().
 */
int evaluateState() {
  return (nqueens-1)*nqueens/2 - countConflicts();
}

int evaluateStateArg(int ind[MAXQ]) {
  return (nqueens-1)*nqueens/2 - countConflictsArg(ind);
}

int maxSlot(int array[MAXQ]){
	int i;
	int maxi = 0;
	for(i = 0; i < nqueens; i++){
		if(array[i] > array[maxi]) maxi=i;
	}
	return maxi;
}


/*************************************************************/

/* A very silly random search 'algorithm' */
#define MAXITER 1000
void randomSearch() {
  int queen, iter = 0;
  int optimum = (nqueens-1)*nqueens/2;

  while (evaluateState() != optimum) {
    printf("iteration %d: evaluation=%d\n", iter++, evaluateState());
    if (iter == MAXITER) break;  /* give up */
    /* generate a (new) random state: for each queen do ...*/
    for (queen=0; queen < nqueens; queen++) {
      int pos, newpos;
      /* position (=column) of queen */
      pos = columnOfQueen(queen);
      /* change in random new location */
      newpos = pos;
      while (newpos == pos) {
        newpos = rand() % nqueens;
      }
      moveQueen(queen, newpos);
    }
  }
  if (iter < MAXITER) {
    printf ("Solved puzzle. ");
  }
  printf ("Final state is");
  printState();
}

/*************************************************************/
int randomRestartChance(int treshold){
	if(random() % 1000 > treshold){
		
		return 1;
	}
	return 0;
	
}
void hillClimbing() {
  initiateQueens(1);
  int iter = 0;
  int optimum = (nqueens-1)*nqueens/2;
  printf("Do you want to use random restart? <Y|N>\n");
  int ranRest = 0;
  int restartCount=0;
  char c;
  do{
	  c = getchar();
  }while(c != 'y' && c != 'n' && c!= 'Y' && c!= 'N');
  if(c == 'Y' || c == 'y')
	ranRest = 1;
  while (evaluateState() != optimum) {
    printf("iteration %d: evaluation=%d\n", iter++, evaluateState());
    if (iter == MAXITER) break;  /* give up */
    if (ranRest && randomRestartChance(950)){
		restartCount++;
		initiateQueens(1); /*This places every queen on a random position*/
	}
    int newpos, queen;
    /**Could cycle through queens instead?*/
    queen = rand() % nqueens; /*Pick a random queen*/
    int newScores[MAXQ];
   
	for(newpos = 0; newpos < nqueens; newpos ++){
		moveQueen(queen, newpos);
		newScores[newpos] = evaluateState();
	}
	printf("Moved queen %d to %d\n", queen, maxSlot(newScores));
	moveQueen(queen, maxSlot(newScores));
  }
  if (iter < MAXITER) {
    printf ("Solved puzzle. ");
  }
  printf ("Final state is");
  printState();
  printf("Restarted %d times\n", restartCount);
}

int temperatureProbabilityTrue(float time, float dE, char *method){
	float thresh;
	float e;
	if(!strcmp(method, "log")){
		e = 1.15;
		thresh = pow(e,(dE/log(time)));
	}else{
		e = 70;
		thresh = pow(e,(dE/time));
	}
	
	printf("Theshold at %f\n", thresh);
	float randNum = (float)random() / RAND_MAX;
	printf("Rand %f\n", randNum);
	if(randNum <= thresh){
		return 0;
	} return 1;
	
}

/*************************************************************/

void simulatedAnnealing() {
  int iter = 0;
  int allowTemp = 0;
  int optimum = (nqueens-1)*nqueens/2;
  char method[4];
  do{
	printf("Give a method for calculating the possibility of randomness <log|lin>\n");
	scanf("%s", method);
  }while (strcmp(method, "lin") && strcmp(method, "log"));
  
  printf("Do you want to use random restart? <Y|N>\n");
  int ranRest = 0;
  int restartCount=0;
  char c;
  do{
	  c = getchar();
  }while(c != 'y' && c != 'n' && c!= 'Y' && c!= 'N');
  if(c == 'Y' || c == 'y')
	ranRest = 1;
	
  while (evaluateState() != optimum) {
    printf("iteration %d: evaluation=%d\n", iter++, evaluateState());
    if (iter == MAXITER*10) break;  /* give up */
    if(ranRest && randomRestartChance(990)){
		initiateQueens(1);
		restartCount++;
	}
    int newpos, pos, queen, prevEval = evaluateState();
    /**Could cycle through queens instead?*/
	queen = random() % nqueens; /*Pick a random queen*/
	pos = columnOfQueen(queen);
	newpos = random() % nqueens;
	printf("Trying queen %d to %d\n", queen, newpos);
	moveQueen(queen, newpos);
	if(evaluateState() < prevEval){
		if(!temperatureProbabilityTrue(iter, (float)(evaluateState() - prevEval), method)){
			/*If the temparturechance says to not move, put back to original position*/
			moveQueen(queen, pos);
		}else{
			allowTemp++;
			printf("Allowed through temperature, %dth time\n", allowTemp);
		}
	}else{
		/*Found dE > 0, let the queen stay*/
	}
  }
  if (iter < MAXITER*10) {
    printf ("Solved puzzle. ");
  }
  printf ("Final state is");
  printState();
  printf("Allowed a total of %d through temperature\n", allowTemp);
  printf("Restarted a total of %d times\n", restartCount);
}
//////////////////////////////////////////////////////////////////////

int pickRandom(){
	for (int i = 0; i<MAXindividuals; i++){
		if ((MAXindividuals - i) / 3 > (rand()%MAXindividuals)/2){
			return i;
		}
	}
	return MAXindividuals;
}

void newIndividual(int gen[MAXindividuals][MAXQ], int son1[MAXQ], int son2[MAXQ]){
	////////crossover - cutting the parents at a random point and sticking together///////////////
	int father = pickRandom();
	int mother;
	do{
		mother = pickRandom();
	}while(mother ==father);
	//printf("father is %d mother is %d\n", father, mother);
	
	int pos;
	for (pos = 0; pos<father; pos++){
		son1[pos] = gen[father][pos];
		son2[pos] = gen[mother][pos];
	}
	for (pos; pos<MAXQ; pos++){
		son1[pos] = gen[mother][pos];
		son2[pos] = gen[father][pos];
	}
	///////mutation - swapping a the column for one row///////////////
	son1[rand()%nqueens] = rand()%nqueens;
	son2[rand()%nqueens] = rand()%nqueens;

}

void insertInOrder(int evaluations[MAXindividuals], int gen[MAXindividuals][MAXQ], int ind[MAXQ], int size){
	int eval = evaluateStateArg(ind);
	int i;
	for (i = 0; i<size; i++){
		if(eval > evaluations [i]){
			for (int h = size; h >= i; h--){		// found place, shift everything by 1;
				evaluations[h] = evaluations[h-1];
				for (int j = 0; j < nqueens; j++){
					gen[h][j] = gen [h-1][j];
				}
			}
			break;
		}
	}
	
	for(int x = 0; x<nqueens; x++){
		gen[i][x] = ind[x];
	}
	evaluations[i] = eval;
	return;
}

void geneticAlgorithm(){
	int generations = 0;
	int optimum = (nqueens-1)*nqueens/2;
	
	printf("optimum is %d\n", optimum);
	
	int newGen[MAXindividuals][MAXQ];
	int currentGen[MAXindividuals][MAXQ];
	int evaluations[MAXindividuals];
	int son1[MAXQ];
	int son2[MAXQ];
	
	while (evaluateState() != optimum) {
		printf("generation %d: evaluation=%d\n", generations++, evaluateState());
		if (generations >= MAXgenerations) break;  /* give up */
		
		
		for (int g = 0; g < MAXindividuals; g+=2){
			newIndividual(currentGen, son1, son2);
			insertInOrder(evaluations, newGen, son1, g);
			insertInOrder(evaluations, newGen, son2, g+1);
		}
		for (int i = 0; i < nqueens; i++){
			queens[i] = newGen[0][i];		// copy the best solution to the main solution array for checking and printing
		}		
		for (int ind = 0; ind<MAXindividuals; ind++){	//updating the current generation to the new one
			for (int pos = 0; pos < nqueens; pos++){
				currentGen[ind][pos] = newGen[ind][pos];
			}
		}
		
	  }
	  if (generations < MAXgenerations) {
		printf ("Solved puzzle. ");
	  }
	  printf ("Final state is");
	  printState();

}

int main(int argc, char *argv[]) {
  int algorithm;
  
  do {
    printf ("Number of queens (1<=nqueens<%d): ", MAXQ);
    scanf ("%d", &nqueens);
  } while ((nqueens < 1) || (nqueens > MAXQ));

  do {
    printf ("Algorithm: (1) Random search  (2) Hill climbing  ");
    printf ("(3) Simulated Annealing (4) Genetic Algorithm\n");
    scanf ("%d", &algorithm);
  } while ((algorithm < 1) || (algorithm > 4));
  
  initializeRandomGenerator();

  initiateQueens(1);
  
  printf("\nInitial state:");
  printState();

  switch (algorithm) {
  case 1: randomSearch();       break;
  case 2: hillClimbing();       break;
  case 3: simulatedAnnealing(); break;
  case 4: geneticAlgorithm();	break;
  }

  return 0;  
}
