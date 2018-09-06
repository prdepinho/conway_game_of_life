
#ifndef __LIFE_H__
#define __LIFE_H__

#include <stdarg.h> /* For variable args handling. */
#include <stdlib.h>	/*dynamic memory allocation api*/
#include <stdio.h>	/*io streams api*/
#include <unistd.h>	/*to use the sleep function*/
#include <string.h>	/*to handle the board files*/
#include <ncurses.h>	/*gui for the game*/
#include <time.h>   /* time function */
#include "stack.h"	/*to use a home-brew stack structure*/

/*
 * These represents the state of the cells; simple as dead and alive,
 * there's no much thrill about a cell's life.
 */ 
#define ALIVE 	1
#define DEAD 	0

/*
 * Constants that states the fate of the cells
 */
#define OVERCROWD	4
#define LONELINESS	1
#define REVIVAL		3

/*
 * defines the default bounds of the board so that it is 
 * easily visible from the shell.
 */
#define BOARD_X		20
#define BOARD_Y		35

/*
 * Colors.
 */
#define RED_BLACK	1
#define BLUE_BLACK	2
#define BLUE_WHITE	3
#define RED_WHITE	4

/*
 * The board structure, it has the heigh, width and the
 * cell matrix itself.
 * The matrix are of characters, since a whole integer wasn't
 * necessary to store only 0 or 1.
 */
struct Board{
	int heigh;	/*the heigh of the board*/
	int width;	/*the width of the board*/
	char **matrix;	/*the matrix of cells*/
	int exit_x;	/* The axis of the exit. */
	int exit_y;	/* The axis of the exit. */
	int time_limit;
};

/*
 * The player character.
 */
struct Player{
	int x;		/* His position in axis X on the board. */
	int y;		/* His position in axis Y on the board. */
	int alive;	/* Weather he is dead or alive. */
};

/*
 * Global variables. The gDeathStack (or global stack of death) is
 * where the cells that die from overcrowd or loneliness go; the
 * gLifeStack (or global stack of life) is where dead cells that
 * will once again live go.
 * Since the deaths and revival of the cells takes place at the same
 * time, there must be a buffer to store the fated cells while browsing
 * the board checking for all them. When all are checked, the cells
 * stored in the stack die (or raise), to the next generation.
 */
Stack *gDeath_Stack;
Stack *gLife_Stack;

/*
 * Takes care of the main loop.
 */
int gLoop;

/*
 * Initializes a board saved in the specified filename.
 * If the filename is null or the file doesn't exists, a random board
 * is generated instead.
 */
struct Board* Init_Board(char *filename, struct Player* player);

/*
 * Creates a random board, with the default size, 50% of the cells
 * start living.
 */
int Random_Board(struct Board *board);

/*
 * Creates a board from the specified file.
 */
int Pattern_Board(struct Board *board, struct Player* player, FILE *file);

/*
 * Sets random living cells to the board. This function
 * is only used if the board is not using a pre-defined pattern
 * it returns -1 if the board is too small.
 */
int Init_Life(struct Board *board);

/*
 * prints the board to shell screen
 */
void Print_Board(struct Board *board, struct Player* player);

/*
 * Updates the cells conditions according to the rules. Checks
 * the cells that may be given death or life, it is here that
 * the global stacks of life and death are used, any cell
 * that was pushed in these stacks attend to their fate here.
 */
int Update_Cells(struct Board *board);

/*
 * Checks if the cell at the co-ordinates x and y is lonely or
 * crowded and pushes it to the global stack of death for further
 * sentence. If the cell was dead and can be reanimated, it
 * is sent to the global stack of life, instead.
 */
void Check_Cell(struct Board *board, int x, int y);

/*
 * This function is used by the Pattern_Board function to read the header
 * of the board file, to set the board width and heigh.
 */
int Read_Header(struct Board *board, FILE *file);

/*
 * This function is used by the Pattern_Board function to read the body
 * of the board file and set the state of the cells.
 */
int Read_Body(struct Board *board, struct Player* player, FILE *file);

/*
 * Inits ncurses.
 */
int Init_Gui(void);

/*
 * Ends ncurses.
 */
int End_Gui(void);

/*
 * Gets input from the user.
 */
void Get_Input(struct Board* board, struct Player* player);

#endif /*__LIFE_H__*/

