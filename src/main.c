#include <curses.h>
#include <stdlib.h>
#include "sweep.h"

void draw_board(tile* board, int width, int height, int c_x, int c_y)
{
	int y1, x1;
	getmaxyx(stdscr, y1, x1);
	y1 = y1 / 2 - height / 2 - 1;
	x1 = x1 / 2 - width - 1;
	mvwprintw(stdscr, y1, x1, "-");
	for (int i = 1; i < width * 2 + 1; i++)
		printw("-");
	y1 += height + 1;
	mvwprintw(stdscr, y1, x1, "-");
	for (int i = 1; i < width * 2 + 1; i++)
		printw("-");

	for (int y = 0; y < height; y++) {
		getmaxyx(stdscr, y1, x1);
		y1 = y1 / 2 - height / 2 + y;
		x1 = x1 / 2 - width - 1;
		mvwprintw(stdscr, y1, x1, "|");
		
		for (int x = 0; x < width; x++) {
			getmaxyx(stdscr, y1, x1);
			y1 = y1 / 2 - height / 2 + y;
			x1 = x1 / 2 - width + x * 2;

			tile t = board[y * width + x];
			int is_cursor = 0;
			if (x == c_x && y == c_y) {
				attron(COLOR_PAIR(1));
				is_cursor = 1;
			}

			if (t.is_flag) {
				if (!is_cursor)
					attron(COLOR_PAIR(2));
				mvwprintw(stdscr, y1, x1, "!");
				attroff(COLOR_PAIR(2));
				mvwprintw(stdscr, y1, x1 + 1, "|");
			}
			else if (!t.revealed) {
				mvwprintw(stdscr, y1, x1, " ");
				attroff(COLOR_PAIR(1));
				mvwprintw(stdscr, y1, x1 + 1, "|");
			}
			else if (board[y * width + x].neighbor_mines > 0) {
				mvwprintw(stdscr, y1, x1, "%d", board[y * width + x].neighbor_mines);
				attroff(COLOR_PAIR(1));
				mvwprintw(stdscr, y1, x1 + 1, "|");
			}
			else {
				if (!is_cursor)
					attron(COLOR_PAIR(3));
				mvwprintw(stdscr, y1, x1, " ");
				attroff(COLOR_PAIR(3));
				mvwprintw(stdscr, y1, x1 + 1, "|");
			}
		}
	}
}

void print_win(void)
{
	const char* msg = " Win ^_^ ";
	int x, y;
	getmaxyx(stdscr, y, x);
	y /= 2;
	x = x / 2 - 4;
	attron(COLOR_PAIR(4));
	mvwprintw(stdscr, y, x, "%s", msg);
	attroff(COLOR_PAIR(4));
}

void print_loss(void)
{
	const char* msg = " Loss -_- ";
	int x, y;
	getmaxyx(stdscr, y, x);
	y /= 2;
	x = x / 2 - 5;
	attron(COLOR_PAIR(5));
	mvwprintw(stdscr, y, x, "%s", msg);
	attroff(COLOR_PAIR(5));
}

int main(int argc, char** argv)
{
	int width = 9, height = 9;
	int mines = 10;
	if (argc > 3) {
		width = atoi(argv[1]);
		height = atoi(argv[2]);
		mines = atoi(argv[3]);
	}

	initscr();
	noecho();
	cbreak();
	keypad(stdscr, 1);
	curs_set(0);

	use_default_colors();
	start_color();
	init_pair(1, COLOR_BLACK, COLOR_WHITE);
	init_pair(2, COLOR_YELLOW, COLOR_RED);
	init_pair(3, COLOR_WHITE, COLOR_BLUE);
	init_pair(4, COLOR_BLACK, COLOR_GREEN);
	init_pair(5, COLOR_BLACK, COLOR_RED);

	tile* board = malloc(width * height * sizeof(tile));
	init_board(board, width, height, mines);
	int x = 0 , y = 0;
	int quit = 0;
	int win = 0;
	int loss = 0;
	int first_reveal = 1;
	while (!quit) {
		draw_board(board, width, height, x, y);

		if (win)
			print_win();
		if (loss)
			print_loss();
		
		switch (getch()) {
			case KEY_LEFT:
				if (x > 0) x--;
				break;
			case KEY_RIGHT:
				if (x < width - 1) x++;
				break;
			case KEY_UP:
				if (y > 0) y--;
				break;
			case KEY_DOWN:
				if (y < height - 1) y++;
				break;
			case 'q':
				quit = 1;
				break;
			case 'r':
				clear();
				win = 0;
				loss = 0;
				init_board(board, width, height, mines);
				first_reveal = 1;
				break;
			case 'z':
				if (first_reveal) {
					while (check_tile(board, width, height, x, y) != 0)
						init_board(board, width, height, mines);
					first_reveal = 0;
				}
				else {
					if (check_tile(board, width, height, x, y) < 0)
						loss = 1;
				}
				if (!loss && check_win(board, width, height, mines))
					win = 1;
				break;
			case 'x':
				board[y * width + x].is_flag ^= 1;
				break;
			default:
				break;
		}
	}

	free(board);
	endwin();
	return 0;
}
