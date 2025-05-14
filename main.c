#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "cJSON.h"

int SCREEN_WIDTH = 0;
int SCREEN_HEIGHT = 0;
int WINDOW_POSX = 0;
int WINDOW_POSY = 0;

int WINDOW_SIZEX = 50;
int WINDOW_SIZEY = 25;

#define true 1
#define false 0

enum DIRECTION {TOP, RIGHT, DOWN, LEFT};

char snakestr[] = "[sexy<<<snake>>>--((xx))--game--&&--cool<->monkey]]]";

typedef struct position {
	int x;
	int y;
} position;

struct snake {
	position pos[4096]; 
	char icon;
	int len;
};

void init();
void sleep_ms(int ms);
void draw_snake(WINDOW* window, struct snake* snake);
void draw_food(WINDOW* window, struct snake* snake);
void update_position(struct snake* snake, int direction);
void createfood();
void checkifate(struct snake* snake);

int direction = TOP;
position food;

void sleep_ms(int ms){
	usleep(ms * 1000);
}

void mainloop();
void endgame();

int main()
{

	init();
	FILE* file;
	file = fopen("./data.json", "r");
	char data[4096];
	fgets(data, 4096, file);

	/* cJSON *json = cJSON_Parse(data); */
	/* char *string = cJSON_Print(json); */

	fclose(file);
	mvprintw(3, 3, "%s", data);

	createfood();
	mainloop();
	endgame();

	return 0;
}

void mainloop(){
	int should_exit = false;
	int c = 0;
	struct snake snake = {
		.pos = { (position) {.x = WINDOW_SIZEX/2, .y = WINDOW_SIZEY} },
		.icon = '0',
		.len = 5,
	};

	// make a window
	WINDOW *main_window;
	WINDOW_POSX = (SCREEN_WIDTH - WINDOW_SIZEX)/2;
	WINDOW_POSY = (SCREEN_HEIGHT - WINDOW_SIZEY)/2;

	main_window = newwin(25, 50, WINDOW_POSY, WINDOW_POSX);
	keypad(main_window, true);
	nodelay(main_window, true);
	refresh();

	while(!should_exit){
		// get char from a window
		int t = wgetch(main_window);
		if (t != ERR) c = t; 

		sleep_ms(110);

		werase(main_window);
		box(main_window,0,0);

		switch (c) {
			case 'q':
				should_exit = true;
				break;

			case KEY_DOWN:
				mvwprintw(main_window, 0,2, "PRESSED: (v)");
				direction = DOWN;
				break;

			case KEY_UP:
				mvwprintw(main_window, 0,2, "PRESSED: (^)");
				direction = TOP;
				break;

			case KEY_RIGHT:
				mvwprintw(main_window, 0,2, "PRESSED: (>)");
				direction = RIGHT;
				break;

			case KEY_LEFT:
				mvwprintw(main_window, 0,2, "PRESSED: (<)");
				direction = LEFT;
				break;

			default:
				mvprintw(WINDOW_POSY, WINDOW_POSX+2, "PRESSED: %d", c);
				break;
		}


		checkifate(&snake);
		update_position(&snake, direction);
		draw_snake(main_window, &snake);
		draw_food(main_window, &snake);

		wrefresh(main_window);
		//refresh();
	}
}

void endgame(){
	// clear and return
	clear();
	endwin();
}

void createfood(){
	food.x = (rand() % (WINDOW_SIZEX));
	food.y = (rand() % (WINDOW_SIZEY));
};

void checkifate(struct snake* snake){
	// compare head and food
	if (snake->pos[0].x == food.x && snake->pos[0].y == food.y) {
		createfood();
		snake->len++;
	}

};

void draw_food(WINDOW* window, struct snake* snake){
	wattron(window, COLOR_PAIR(2));
	mvwprintw(window, food.y, food.x, "%c", snakestr[snake->len%strlen(snakestr)]);
	wattroff(window, COLOR_PAIR(2));
}

void draw_snake(WINDOW* window, struct snake* snake){
	wattron(window, COLOR_PAIR(1));
	position head = snake->pos[0];
	mvwprintw(window, 0, 15, "x: %d, y: %d", head.x, head.y);

	for(int i = 0; i < snake->len; i++){
		position part = snake->pos[i];
		mvwprintw(window, part.y, part.x, "%c", snakestr[i%strlen(snakestr)]);
	}
	wattroff(window, COLOR_PAIR(1));
}

void update_position(struct snake* snake, int direction){
	// go thru len and pass on pos
	for (int i = snake->len - 1; i > 0; i--){
		position last_pos = snake->pos[i-1];
		snake->pos[i] = last_pos;
	}

	position* head = &snake->pos[0];
	switch (direction) {
		case TOP:
			head->y--;
			break;

		case DOWN:
			head->y++;
			break;

		case LEFT:
			head->x--;
			break;

		case RIGHT:
			head->x++;
			break;
	}

}

void init(){
	initscr();
	curs_set(0);
	noecho();
	cbreak();
	start_color();			/* Start color 			*/

	init_pair(1, COLOR_BLACK, COLOR_WHITE);
	init_pair(2, COLOR_GREEN, COLOR_RED);

	clear();

	// Get screen width and height
	getmaxyx(stdscr, SCREEN_HEIGHT, SCREEN_WIDTH);
}
