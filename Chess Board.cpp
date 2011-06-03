#include <iostream>
using namespace std;

#define MAX_MOVE_NUM 8

struct ValidMoves
{
	int count;
	int outOfBounds;
	int position;
	int newPositions[MAX_MOVE_NUM]; //The knight can move a maximum number of 8 squares
};

void printArray(const int width, const int height, int position, int *board)
{
	printf("\n");
	for(int i=0; i<height; i++)
	{
		printf("\n");
		for(int j=0; j<width; j++)
		{
			printf("[%02d]",board[j+i*width]);
		}
	}	
}

ValidMoves countValidMoves(const int width, const int height, int position, int *board)
{
	int dx[]={-2,-1,+1,+2,+2,+1,-1,-2};
	int dy[]={+1,+2,+2,+1,-1,-2,-2,-1};
	int count=0;
	int x=position%height;
	int y=position/width;
	
	ValidMoves ret;
	ret.count=0;
	ret.outOfBounds=0;
	ret.position=position;
	int temp=0;

	for(int i=0; i<MAX_MOVE_NUM; i++)
	{
		if( (x+dx[i])<0 || (x+dx[i])>=width || (y+dy[i])<0 || (y+dy[i])>=height )
		{
			//Out of bounds! Increment the OOB counter
			ret.outOfBounds++;
			ret.newPositions[i]=-1; //Assume that the move is not valid, mark it with a negative
		}else{
			//We'll convert the x/y coord system back to a 1 dimensional position and grab that value in the array
			temp=(x+dx[i])+(y+dy[i])*width;
			if(board[temp]==0)
			{
				//They haven't moved here, increment the counter
				ret.count++;
				ret.newPositions[i]=temp;
			}
		}
	}
	return ret;
}

int *startArray(const int width, const int height, const int initialValue, int startPosition)
{
	

	int *ret=new int[width*height];

	for (int i=0; i<height; i++)
	{
		for (int j=0; j<width; j++)
		{
			ret[i*width+j]=initialValue;
		}
	}
	ret[startPosition]=1;
	return ret;
}

int *solveArray(const int width, const int height, int position, int *board,int count)
{
	int *ret=board;
	ValidMoves checkMoves=countValidMoves(width,height,position,board);
	ValidMoves nextMove;
	ValidMoves probe;

	//Initialize nextMove's count value to one more than max move num (meaning ANY move is preferred over this one)
	nextMove.count=MAX_MOVE_NUM+1;
	//We have counted the valid moves and logged the valid moves from this square. For the algorithm to work, we must now also grab the counts for all the moves from this square
	bool foundMove=false;

	for(int i=0; i<MAX_MOVE_NUM; i++)
	{
		if(checkMoves.newPositions[i]>-1)
		{
			probe=countValidMoves(width,height,checkMoves.newPositions[i],board);
			if(probe.count>0 && probe.count<nextMove.count)
			{
				nextMove=probe;
				foundMove=true;
			}else if(probe.count>0 && probe.count==nextMove.count && probe.outOfBounds>nextMove.outOfBounds){
				nextMove=probe;
				foundMove=true;
			}
		}
	}

	//Add a catch for the final square. Since our previous for loop only allows movement to a square with at least one more valid move
	//We'll just grab the first open square (since it should be our last one, in this case)
	if(foundMove==false)
	{
		for(int i=0; i<MAX_MOVE_NUM; i++)
		{
			if(checkMoves.newPositions[i]>-1)
			{
				nextMove=countValidMoves(width,height,checkMoves.newPositions[i],board);
				foundMove=true;
				break;
			}
		}
	}

	//We've checked all the valid moves and the best move should be held by nextMove now.
	if(foundMove)
	{
		ret[nextMove.position]=count; //Visited
		ret=solveArray(width,height,nextMove.position,ret,count+1);
	}
	
	return ret;
}

int main(int argc, char *argv[])
{
	const int width=8;
	const int height=8;
	const int initialValue=0;
	int position=0; //Position was chosen arbitrarily, can be read from the command line at any rate

	if(argc>1)
		position=atoi(argv[1]);

	printf("Running chess board problem...");
	printf("\nUsing starting location as %d...",position);
	printf("\nInitializing the board array...");
	int *board=startArray(width,height,initialValue,position);
	printf("Done.");
	printf("\nAnalyzing problem...");
	int *solution=solveArray(width,height,position,board,2); //Our count starts at 2 since the squares are marked BEFORE the count is incremented, and the function start array sets the starting square's value to 1
	printf("Done.");
	printf("\nChess board solved! Press enter to exit.");

	printArray(width, height, position, solution);

	getchar();

	return 0;
}