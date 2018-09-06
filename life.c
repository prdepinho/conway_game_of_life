/*
 * This little program is to test the game of life.
 *
 *
 */

#include "life.h"


int main(int argc, char* argv[])
{
	struct Board *board;	/*the board of the game*/
	int gen_count = 0;	/*counts the generations*/
	gLoop = 1;		/*one for true*/
	
	struct Player player;
	player.x = 2;
	player.y = 2;
	player.alive = 1;

	srand(time(0));
	gDeath_Stack = Stack_Create();
	if(gDeath_Stack == NULL){
		printf("Cannot initialize gDeath_Stack\n");
		return -1;
	}

	gLife_Stack = Stack_Create();
	if(gLife_Stack == NULL){
		printf("Cannot initialize gLife_Stack\n");
		return -1;
	}
	
	board = Init_Board(argv[1], &player);
	if(board == NULL){
		printf("Could not allocate much memmory\n");
		return -1;
	}

	Init_Gui();


	Print_Board(board, &player);

	while(gLoop){
		mvprintw(board->heigh, 0, "Time limit: %d\n", 
				(board->time_limit)--);

		Get_Input(board, &player);
		Update_Cells(board);

		if(board->matrix[player.y][player.x] == ALIVE){
			player.alive = 0;
		}

		Print_Board(board, &player);

		if(!player.alive){
			mvprintw(board->heigh, 0, "The player dies    ");
			break;
		}else{
			if(player.x == board->exit_x && player.y == board->exit_y){
				mvprintw(board->heigh, 0, "You Win! Congrats.    ");
				break;
			}else{
				if(board->time_limit < 0){
					mvprintw(board->heigh, 0, "Time's Up    ");
					break;
				}
			}
		}
	}

	getch();
	End_Gui();
	return 0;
}
	

struct Board* Init_Board(char *filename, struct Player* player)
{
	FILE *file;
	struct Board *board;
	
    board = malloc(sizeof(struct Board));
	if(board == NULL)
		return NULL;

	file = fopen(filename, "r");
	if(file == NULL){
		if(Random_Board(board)){
			free(board);
			board = NULL;
		}
	}else{
		if(Pattern_Board(board, player, file)){
			free(board);
			board = NULL;
		}
	}
	if(file != NULL)
		fclose(file);

	return board;
}

int Random_Board(struct Board *board)
{
	board->width = BOARD_Y;
	board->heigh = BOARD_X;
	int a;
	
	board->matrix = calloc(sizeof(char*), board->heigh);
	if(board->matrix == NULL)
		return -1;
	
	for(a = 0; a < board->heigh; a++){
		board->matrix[a] = calloc(sizeof(char), board->width);
		if(board->matrix[a] == NULL)
			return -1;
	}

	if(Init_Life(board)){
		printf("This board is too small\n");
		return -1;
	}
	board->exit_x = (rand() % (board->width -2)) +1;
	board->exit_y = (rand() % (board->heigh -2)) +1;

	board->time_limit = (rand() % (board->width +board->heigh)) 
		+(board->exit_x +board->exit_y);
	return 0;
}

int Pattern_Board(struct Board *board, struct Player* player, FILE *file)
{
	int a;
	
	if(Read_Header(board, file)){
		return -1;
	}

	board->matrix = calloc(sizeof(char*), board->heigh);
	if(board->matrix == NULL)
		return -1;
	
	for(a = 0; a < board->heigh; a++){
		board->matrix[a] = calloc(sizeof(char), board->width);
		if(board->matrix[a] == NULL)
			return -1;
	}

	if(Read_Body(board, player, file)){
		return -1;
	}
	return 0;
}

int Init_Life(struct Board *board)
{
	int x, y;
	
	for(y = 1; y < board->heigh - 1; y++){
		for(x = 1; x < board->width - 1; x++){
			if(rand() % 2)
				board->matrix[y][x] = ALIVE;
		}
	}
	return 0;
}

void Print_Board(struct Board *board, struct Player* player)
{
	int y, x;

	for(y = 0; y < board->heigh; y++){
		for(x = 0; x < board->width; x++){
			if(board->exit_x == x && board->exit_y == y){
				if(board->matrix[y][x] == ALIVE){
					mvaddch(y, x *2, '&' | COLOR_PAIR(BLUE_WHITE));
				}else{
					mvaddch(y, x *2, '.' | COLOR_PAIR(RED_WHITE));
				}
			}else{
				if(board->matrix[y][x] == ALIVE){
					mvaddch(y, x *2, '&' | COLOR_PAIR(BLUE_BLACK));
				}else{
					mvaddch(y, x *2, '.' | COLOR_PAIR(RED_BLACK));
				}
			}
		}
	}
	if(player->alive){
		mvaddch(player->y, player->x *2, '@');
	}else{
		flash();
		mvaddch(player->y, player->x *2, 'X' | COLOR_PAIR(RED_BLACK));
	}
	refresh();
}

int Update_Cells(struct Board *board)
{
	int x, y;
	for(y = 1; y < board->heigh - 1; y++){
		for(x = 1; x < board->width - 1; x++){
			Check_Cell(board, x, y);
		}
	}
	while(Stack_Size(gDeath_Stack))
		*(char*)Stack_Pop(gDeath_Stack) = DEAD;

	while(Stack_Size(gLife_Stack))
		*(char*)Stack_Pop(gLife_Stack) = ALIVE;
	
	return 0;	
}

void Check_Cell(struct Board *board, int x, int y)
{
	char living = 0;	/*the number of living neighbours*/	
	char *cell = &board->matrix[y][x];
	int a, b;

	for(a = -1; a <= 1; a++){
		for(b = -1; b <= 1; b++){
			living += board->matrix[y + b][x + a];
		}
	}

	if(*cell == ALIVE){
		living--;	/*the own cell's value was added as well*/
		if(living <= LONELINESS)
			Stack_Push(gDeath_Stack, cell);
		else if(living >= OVERCROWD)
			Stack_Push(gDeath_Stack, cell);
	}else{
		if(living == REVIVAL)
			Stack_Push(gLife_Stack, cell);
	}
}

int Read_Header(struct Board *board, FILE *file)
{
	char buffer[4];	

	fgets(buffer, 4, file);
	buffer[strlen(buffer) - 1] = '\0';
	
	board->width = atoi(buffer);
	if(board->width <= 0){
		printf("Header: Illegal number\n");
		return -1;
	}
	
	fgets(buffer, 4, file);
	buffer[strlen(buffer) - 1] = '\0';
	
	board->heigh = atoi(buffer);
	if(board->heigh <= 0){
		printf("Header: Illegal number\n");
		return -1;
	}
	
	fgets(buffer, 4, file);
	buffer[strlen(buffer) - 1] = '\0';

	board->time_limit = atoi(buffer);
	if(board->time_limit <= 0){
		printf("Time limit %d is too short", board->time_limit);
		return -1;
	}
	
	return 0;
}

int Read_Body(struct Board *board, struct Player* player, FILE *file)
{	
	int x = 0;
	int y = 0;
	char c;
	
	while(!feof(file)){
		c = getc(file);
		
		switch(c){
		case '&':
			board->matrix[y][x] = ALIVE;
			x++;
			break;
		case '.':
			board->matrix[y][x] = DEAD;
			x++;
			break;
		case 'X':
			board->matrix[y][x] = DEAD;
			board->exit_x = x;
			board->exit_y = y;
			x++;
			break;
		case '@':
			board->matrix[y][x] = DEAD;
			player->x = x;
			player->y = y;
			x++;
			break;
		case '\n':
			y++;
			break;
		default:
			break;
		}
		
		if(x == board->width)
			x = 0;
	}
	return 0;

}

int Init_Gui(void)
{
	initscr();	/*inits ncurses*/
	raw();		/*dont need to press ENTER to input a char*/
	noecho();	/*dont print back the input char*/
	start_color();	/*set color handling*/
	init_pair(RED_BLACK, COLOR_RED, COLOR_BLACK);
	init_pair(BLUE_BLACK, COLOR_BLUE, COLOR_BLACK);
	init_pair(BLUE_WHITE, COLOR_BLUE, COLOR_WHITE);
	init_pair(RED_WHITE, COLOR_RED, COLOR_WHITE);
	keypad(stdscr, TRUE);	/*enables most keys of the keyboard*/
	curs_set(0); 	/*invisible cursor*/
	return 0;
}

int End_Gui(void)
{
	endwin();								/* end ncurses */
	return 0;
}

void Get_Input(struct Board* board, struct Player* player)
{
	int ch = getch();
	switch(ch){
	case 'q':
		mvprintw(board->heigh, 0, "Bye...             ");
		gLoop = 0;
		break;
	case 'h': case KEY_LEFT:
		if(player->x > 1)
			player->x--;
		break;
	case 'j': case KEY_DOWN:
		if(player->y < board->heigh -2)
			player->y++;
		break;
	case 'k': case KEY_UP:
		if(player->y > 1)
			player->y--;
		break;
	case 'l': case KEY_RIGHT:
		if(player->x < board->width -2)
			player->x++;
		break;
	}
}

