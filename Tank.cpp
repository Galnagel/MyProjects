//                                            Tank
//                                 ---------------------
//
//
// Progremer      : Gal Nagel
// Student Number : 331635243
// Date           : 3/12/2025 - 13/12/2025
// 
//-------------------------------------------------------------------------------------------------
#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h> 
//-------------------------------------------------------------------------------------------------
//                                    Global variables and define words
#define wall   '#'
#define empty  '2' 
#define bricks 'X'
#define tanks  '0'
#define win     1
#define success 0
#define lose   -1
// Board size
int size = 0;
// derictions array
int derictions[9][2] =
{
	{ 0,-1},   //1. UP
	{ 1, 0},   // 2. RIGHT
	{0,  1},   // 3. DOWN
	{ -1,0},   // 4. LEFT

	{1, -1}, // 5. UP-RIGHT
	{ 1, 1}, // 6. DOWN-RIGHT 
	{ -1,1}, // 7. DOWN-LEFT 
	{-1,-1}, // 8. UP-LEFT 
	{0,  0} // self
};
//-------------------------------------------------------------------------------------------------
//                                            Function
//                                           ----------
void printBoard(char** Board);
char** creatGameBoard();
int shot(char** Board, int x, int y, int deriction);
int bomb(char** Board, int tankX, int tankY, int BombX, int BombY);
int shot_With_Change(char** Board, int* x, int* y, int x1, int y1, int deriction, int countWall);
//-------------------------------------------------------------------------------------------------
//                                              mov
//                                           ----------
// 
// General : The function move player locition in Board
//
// Parameters : 1. char** Board - Board game
//              2. int *  x - player x location 
//              3. int *  y - player y location
//              4. int    steps - how many steps to go
//              5. int    deriction - where to go :
//                                                  1. UP 
//                                                  2. RIGHT
//                                                  3. DOWN
//                                                  4. LEFT
//  
//
// Return Value : int:  
//                1. success (0) -> we finsh moving   
//                2. win     (1) -> we won 
//
//-------------------------------------------------------------------------------------------------
int mov(char** Board, int *x,int *y, int deriction, int steps) 
{
	// new derictions
	int newX = *x + derictions[deriction ][0];
	int newY = *y + derictions[deriction ][1];

	// finsh steps and not won
	if (steps == 0) return success; 

	// cant mov
	if (Board[newY][newX] == wall || Board[newX][newY] == bricks) return success;

	// win
	if (Board[newY][newX] == tanks)  return win; 

	// mov
	else {
		// clean current lociton
		Board[*y][*x] = empty;
	
		// update x, y 
		*x = newX;
		*y = newY;
		// put tank in new lociton
		Board[*y][*x] = tanks;

		// calling mov - > step = step - 1;
		return mov(Board, x, y, deriction, steps - 1);
	}

}
//-------------------------------------------------------------------------------------------------
//                                         creatGameBoard
//                                        -----------------
// 
// General     : The function create Bord -> char matrix [size + 2] [size + 2] 
//
// Parameters  : NULL
//  
//
// Return Value : char **  (Game Board)
//-------------------------------------------------------------------------------------------------
char ** creatGameBoard() 
{
	// start row for tanks lociton
	int StartRow;
	char** Board;

	// create board and wall
	size    += 2;
	// start row for players  =  mid of the Board
	StartRow = (size / 2); 

	// crate char pointers  array 
	Board = (char**)malloc(size * sizeof(char*));

	// check not null
	if (Board == NULL)
	{
		printf("Memory allocation failed\n");
		return NULL;
	}

	// create board
	for (int i = 0; i < size; i++) 
	{
		Board[i] = (char*)malloc(size * sizeof(char));
		// check not null
		if (Board == NULL)
		{
			printf("Memory allocation failed\n");
			return NULL;
		}
	}

	// filling Board
	for (int i = 0; i < size; i++) 
	{
		for (int j = 0; j < size; j++) 
		{
			// bricks wall
			if (i % 2 == 0 && j % 2 == 0) Board[i][j] = bricks;
			// empty space
			else Board[i][j] = empty;
		}
	}


	// create wall
	for (int i = 0; i < size; i++) Board[i][0] = wall;
	for (int i = 0; i < size; i++) Board[i][size - 1] = wall;
	for (int i = 0; i < size; i++) Board[0][i] = wall;
	for (int i = 0; i < size; i++) Board[size - 1][i] = wall;

	// put tanks in board
	Board[StartRow][1]        = tanks;
	Board[StartRow][size - 2] = tanks;

	return Board;

}
//-------------------------------------------------------------------------------------------------
//                                          printBoard
//                                         -------------
// 
// General     : The function print the Game Board without wall
//
// Parameters  : NULL
//  
//
// Return Value : void
//-------------------------------------------------------------------------------------------------
void printBoard(char ** Board)
{
	// print Board without wall
	for (int i = 1; i < size - 1; i++)
	{
		for (int j = 1; j < size - 1; j++)
		{
			printf("%4c", Board[i][j]);
		}
		printf("\n");
	}

	printf("\n");

}
//-------------------------------------------------------------------------------------------------
//                                              bomb
//                                           ----------
// 
// General : The function move player locition in Board
//
// Parameters : 1. char** Board - Board game
//              2. int   tankX - player x location 
//              3. int   tankY - player y location
//              4. int   BombX - Bomb x location
//              5. int   BombY   Bomb y location
// Return Value : int:  
//                1. success (0) ->    
//                2. win     (1) -> we won 
//                3. lose   (-1) -> we hit ourself
//
//-------------------------------------------------------------------------------------------------
int bomb(char** Board, int tankX, int tankY,int BombX, int BombY)
{
	// for geting 
	srand(time(NULL));
	int randomNumber;

	// for al 8 derictions in derictions array
	for (int i = 0; i < 9; i++)
	{
		BombX += derictions[i][0];
		BombY += derictions[i][1];

		// get random numbers (1 or 0)
		randomNumber = rand() % 2;

		// if we need to bomb a place check if its mot a wall or empty
		if (randomNumber == 1 && Board[BombY][BombX] != wall && Board[BombY][BombX] != empty)
		{
			// hit himself
			if (BombX == tankX && BombY == tankY) return lose;
			// 
			else if (Board[BombY][BombX] = tanks) return win;

			Board[BombY][BombX] = empty;
			printBoard(Board);
		}
		// get back original location 
		BombX -= derictions[i][0];
		BombY -= derictions[i][1];

	}

	return success;

}
//-------------------------------------------------------------------------------------------------
//                                              shot
//                                           ----------
// 
// General : The function shot in the direction the user selected.

//
// Parameters : 1. char** Board - Board game
//              2. int    x - player x location 
//              3. int    y - player y location
//              4. int    deriction - where to shot:
//                                                  1. UP 
//                                                  2. RIGHT
//                                                  3. DOWN
//                                                  4. LEFT
// 
// Return Value : int:  
//                1. success (0) ->    
//                2. win     (1) -> we won 
//
//-------------------------------------------------------------------------------------------------
int shot(char** Board, int x, int y, int deriction)
{
	// where to shot next
	int newX = x + derictions[deriction][0];
	int newY = y + derictions[deriction][1];

	// we hit wall
	if (Board[newY][newX] == wall ) return success;

	// we hit briks 
	if(Board[newY][newX] == bricks)
	{
		Board[newY][newX] = empty;
		return success;
	}

	// win
	if (Board[newY][newX] == tanks) return win;

	// shot next 
	else 
	{
		return shot(Board, newX, newY, deriction);
	}

}
//-------------------------------------------------------------------------------------------------
//                                         shot_With_Change
//                                        -------------------
// 
// General : The function shot in the direction the user selected. if we hit wall we will change direction
//
// Parameters : 1. char** Board - Board game
//              2. int *  x -  player x location 
//              3. int *  y -  player y location
//              4. int    x1 - player x location - for start shotting  
//              5. int    y1 - player y location - for start shotting  
//              6. int    countWall - how many changing direction we made
//              7. int    deriction - where to shot:
//                                                  1. UP 
//                                                  2. RIGHT
//                                                  3. DOWN
//                                                  4. LEFT
// 
// Return Value : int:  
//                1. success (0) ->    
//                2. win     (1) -> we won 
//                3. lose   (-1) -> we hit ourself
//-------------------------------------------------------------------------------------------------
int shot_With_Change(char** Board, int* x, int* y, int x1, int y1, int deriction, int countWall) 
{

	// new location
	int newX = x1 + derictions[deriction][0];
	int newY = y1 + derictions[deriction][1];

	
	// finsh function
	if (countWall == 5)  return success ;

	// hit wall
	if (Board[newY][newX] == wall)
	{
		// change deriction and countWall parameter
		deriction = (deriction + 1) % 4;  

		return shot_With_Change(Board, x, y, x1, y1, deriction , countWall + 1);
	}
	// hit briks
	if (Board[newY][newX] == bricks)
	{
		Board[newY][newX] = empty;
		return success;
	}

	// we hit tanks
	if (Board[newY][newX] == tanks)
	{
		// hit ourself
		if (newX == *x && newY == *y) return lose;
		// win
		else return win;
	}
	// else continue
	else return shot_With_Change(Board, x, y, newX, newY, deriction, countWall);

}
//-------------------------------------------------------------------------------------------------
//                                             startGame
//                                        -------------------
// 
// General      : The function start a new game
//
// Parameters   : NULL
// 
// Return Value : void 
//-------------------------------------------------------------------------------------------------
void startGame()
{
	// who playes ? 1 or -1;
	int turn = 1;
	// win = 1, lose = -1, else 0
	int checkWin = 0;
	// tanks lociton in Board
	int x1, y1, x2, y2, x, y;

	// what step to do
	int step = 0;
	// for functions
	int deriction;
	int steps;
	int BombY, BombX;

	// Get board size
	printf("enter Board Size\n");
	scanf("%d", &size);

	// create board
	char** Board = creatGameBoard();
	printBoard(Board);

	// starting locaiton
	y1 = y2 = (size / 2);
	// rigth player 
	x1 = 1;
	// left player
	x2 = size - 2;

	// left player start the game
	while (checkWin == success && step != 999)
	{
		turn == 1 ? printf("Left side turn") : printf("rigth side turn");
		// what step to do?
		printf("enter step:\n1.Mov\n2.Shot\n3.Bomb\n4.Shot with change\n");
		// get step
		scanf("%d", &step);

		// cases
		switch (step)
		{
		// case 1 :  move 
		case(1):
		{
			// get deriction, steps
			printf("enter deriction to mov\n1. UP\n2. RIGHT\n3. DOWN\n4. LEFT\n");

			scanf("%d", &deriction);

			printf("enter num of steps to mov\n");

			scanf("%d", &steps);

			// call mov function
			if (turn == 1) checkWin = mov(Board, &x1, &y1, deriction - 1, steps);

			else checkWin = mov(Board, &x2, &y2, deriction - 1, steps);

			// print Board
			printBoard(Board);

			break;
		}
		// case 2 :  shot 
		case(2):
		{
			// get deriction
			printf("enter deriction to shot\n1. UP\n2. RIGHT\n3. DOWN\n4. LEFT\n");
			scanf("%d", &deriction);

			// call shot function
			if (turn == 1) checkWin = shot(Board, x1, y1, deriction - 1);

			else checkWin = shot(Board, x2, y2, deriction - 1);

			// print Board
			printBoard(Board);

			break;
		}
		// case 3 :  bomb 
		case(3):
		{
			// get Bomb location
			printf("enter x and y for boaming\n");
			scanf("%d %d", &BombX, &BombY);

			// call bomb function
			if (turn == 1) checkWin = bomb(Board, x1, y1, BombX, BombY);

			else checkWin = bomb(Board, x2, y2, BombX, BombY);

			// print Board
			printBoard(Board);

			break;
		}		
		// case 4 :  shot_With_Change 
		case (4):
		{
			
			printf("enter deriction to shot\n1. UP\n2. RIGHT\n3. DOWN\n4. LEFT\n");
			// get deriction
			scanf("%d", &deriction);

			// call shot_With_Change function
			if (turn == 1) checkWin = shot_With_Change(Board, &x1, &y1, x1, y1, deriction - 1, 0);

			else checkWin = shot_With_Change(Board, &x2, &y2, x2, y2, deriction - 1, 0);

			// print Board
			printBoard(Board);

			break;

		}
		default:
			break;
		}

		turn *= -1;
	}

	
	if (checkWin == win)
	{
		if (turn == -1) printf("The player that started in the left side won\n");

		else printf("The player that started in the rigth side won\n");

	}

	else 
	{
		if (checkWin == lose)
		{
			if (turn == 1) printf("The player that started in the left side won\n");

			else printf("The player that started in the rigth side won\n");
		}
		else printf("Tie \n");
	}
	

}


void main() {
	 startGame();
	 
}