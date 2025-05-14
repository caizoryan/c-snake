void print_menu(WINDOW *menu_win, int highlight)
{
	int x, y, i;	
	x = 2;
	y = 2;

	box(menu_win, 0, 0);

	for(i = 0; i < n_choices; ++i) {	
		if(highlight == i + 1) /* High light the present choice */
		{	
			wattron(menu_win, A_UNDERLINE); 
			mvwprintw(menu_win, y, x, "%s", choices[i]);
			wattroff(menu_win, A_UNDERLINE);
		}
		else
			mvwprintw(menu_win, y, x, "%s", choices[i]);
		++y;
	}

	wrefresh(menu_win);
}

int whatever(){
	WINDOW *menu_win;
	WINDOW *another;
	int highlight = 1;
	int choice = 0;
	int c;

	initscr();
	clear();
	noecho();
	cbreak();	/* Line buffering disabled. pass on everything */

	getmaxyx(stdscr, Y,X);

	startx = X - WIDTH;
	starty = Y - HEIGHT;
		
	menu_win = newwin(HEIGHT, WIDTH, starty, startx);

	keypad(menu_win, TRUE);
	nodelay(menu_win, 1);

	mvprintw(0, 0, "Use arrow keys to go up and down, Press enter to select a choice");
	mvprintw(1, 0, "X: %d, Y: %d", X, Y);
	refresh();

	print_menu(menu_win, highlight);
	while(1) {
		c = wgetch(menu_win);
		sleep_ms(10);
		mvprintw(5, 0, "count: %d", count);
		refresh();
		print_menu(menu_win, highlight);
		if(choice != 0)	/* User did a choice come out of the infinite loop */
			break;
	}	

	clear();
	mvprintw(23, 0, "You chose choice %d with choice string %s\n", choice, choices[choice - 1]);
	refresh();
	c = wgetch(menu_win);
	endwin();
	return 0;
}
