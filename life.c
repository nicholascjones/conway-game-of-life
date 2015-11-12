// life.c
// write a program that recreate's Conway's Game of Life given certain parameters

#include <stdio.h>
#include <ctype.h> //for scan type
#include <unistd.h> //usleep function
#define SIZE 40 //size of board
#define DT 100000 //timestep

//preprocessing

int inputCoord(FILE*); //gives input based on presence or absence of a file
char userinput(FILE*,char); //reads user input
int Check(int); //checks number immediately and assigns T/F value
void CheckSurroundings(int[][SIZE], int[][SIZE]); //counts surrounding number of live cells
void dead(int*); //kills alive cell or maintains dead cell
void alive(int*); // revives dead cell or maintains alive cell
void printboard(int[][SIZE]); //prints board
void update(int[][SIZE], int[][SIZE]); // proceeds to next step

int main(int argc, char *argv[2])
{
	//initialization
	FILE *fPtr; //input file pointer
	int BoardNow[SIZE][SIZE] = {0}, BoardNext[SIZE][SIZE]; //current and next board settings
	int p1,p2; //coordinate points as input
	int c; //user input
	int PlayOn = 0; //variable that plays game permanently

	fPtr = fopen( argv[1], "r"); //reads file as input
		//initiation switch
		while ((c != 'q') && (PlayOn == 0)) { //runs as long as 'q' or 'p' are not selected	
 	        	c = userinput(fPtr,c); //c as input to prevent repeating prompt question
			switch (c) {
				case 'a': p1 = inputCoord(fPtr); //addition case
					  p2 = inputCoord(fPtr);
				  	  BoardNow[p1][p2] = 1; //makes alive
				 	  break;
				case 'r': p1 = inputCoord(fPtr); //removal case
					  p2 = inputCoord(fPtr);
					  BoardNow[p1][p2] = 0; //kills
				 	  break;	  
				case 'n': CheckSurroundings(BoardNow, BoardNext);
					  update(BoardNow, BoardNext); //moves to next step, makes the "next board" and makes it current
				  	  break;
				case 'q': return 0; //quit case
					  break;
				case 'p': PlayOn = 1; //plays continuously and exits loop
					  break; //plays permanently
				//default: printf("Sorry, that input is not valid\n"); 
					break;
			} //ends switch
		} //ends initiation while loop
	 
	//continuous play
	while (PlayOn == 1) { //will go on forever
		CheckSurroundings(BoardNow, BoardNext);
		update(BoardNow, BoardNext);
		usleep(DT); // waits for one second
		system("clear"); //clears board
	} //end loop

} //ends main function execution                                    
	

	// functions

	//re-draws board and changes
	void update(int BoardNow[SIZE][SIZE], int BoardNext[SIZE][SIZE])
	{
		int i,j; //counters

		for (i = 0; i < SIZE; i++) {
			for (j = 0; j < SIZE; j++) {
				BoardNow[i][j] = BoardNext[i][j]; //allows for printing 
			}
		}		

                printboard(BoardNow);
	}

	//checks number of surroundings of a given cell and acts accordingly
	void CheckSurroundings(int BoardNow[][SIZE], int BoardNext[][SIZE])
	{
		//initialization
		int i,j; //counters
		int num; //number of surrounding cells
		int surround = 0; //number of LIVING surrounding cellls
		int rv; //1 or 0 boolean for a particular surrounding cell

		//loop check
		for (i = 0; i < SIZE; i++) { //row loop
			for (j = 0; j < SIZE; j++) { //column loop


				// surrounding characters
				
			if ((j-1) >= 0) {
				if ((i+1) < SIZE) {
					rv = Check(BoardNow[i+1][j-1]);
        	                        surround += rv; 
				}

				if ((i-1) >= 0) {
					rv = Check(BoardNow[i-1][j-1]);
					surround += rv;
                                }
					rv = Check(BoardNow[i][j-1]);
                                	surround += rv;
			}
				if ((i-1) >= 0) {
					rv = Check(BoardNow[i-1][j]);
					surround += rv;
				}
				if ((i+1) < SIZE) {
                                	rv = Check(BoardNow[i+1][j]);
                                	surround += rv;
				}
			if ((j+1) < SIZE) {                         
			        if ((i-1) >= 0) {
					rv = Check(BoardNow[i-1][j+1]);
                                	surround += rv;
                                }
				rv = Check(BoardNow[i][j+1]);
                                surround += rv;
                                if ((i+1) < SIZE) {
					rv = Check(BoardNow[i+1][j+1]);
                                	surround += rv;
				}
			}
				//cell determination
				if (BoardNow[i][j] == 1) {
					if  (surround == 2 || surround == 3)
						alive(&BoardNext[i][j]);
					else dead(&BoardNext[i][j]);
				}
				else  {
				 	if (surround == 3)
						alive(&BoardNext[i][j]);
					else dead(&BoardNext[i][j]);
				}
					

				surround = 0; //resets value for next character
				rv = 0;
			}
		} //end of for loops
	} //end of function 												
	

int Check(int comp) //checks number immediately and assigns value
{
		int rv = 0; //return value		

		if (comp == 1) {
			rv = 1;
		}
		else {rv = 0;}
		return rv; //returns value
} //end function
		
	//makes a cell alive with a pseudo-boolean int value of 1
	void alive(int* location) {
		*location = 1;
	}

	//makes a cell dead with a pseudo-boolean int value of 0
	void dead(int* location) {
		*location = 0;
	}

void printboard(int board[SIZE][SIZE]) //prints board
{
	int i,j; //counters
	
	for (i = 0; i<SIZE; i++) {
		for (j = 0; j < SIZE; j++) {
			if (board[i][j] == 1) {
				printf("X "); //x and space
			} else {
				printf("  "); //two spaces
			}
			
			if (((j+1) % 40) == 0){
				printf(" \n"); //new line
			}
		}
	} //ends loop
} //ends function 

//receives input
char userinput(FILE* fPtr, char cp)
{
	char c; //user's input
	
	if (fPtr == NULL) {
		if (!isalpha(cp))
			printf("Enter a Command\n"); //prompts user
		scanf("%c", &c);
	}
	else {
			fscanf(fPtr, "%c", &c); //scans pointer
			fPtr++;
	}
	 //increments pointer
	return c;
}		

//receives coordinate input
int inputCoord(FILE* fPtr)
{
        int p = 100; //user's input; given value of 100 such that 
	
        if (fPtr == NULL) { //in case of no file mode
		while (p >= SIZE) { //prevents for error 
                	printf("Enter a Coordinate\n"); //prompts user
                	scanf("%d", &p);
			if (p >= SIZE)
				printf("Sorry, that input is not valid. Pick a coordinate from 0 to 39.\n");
        	}
	}
        else {
                fscanf(fPtr, "%d", &p);
        	fPtr++; //increments pointer
	}
        	return p;
}
