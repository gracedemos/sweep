#ifndef SWEEP_H
#define SWEEP_H

typedef struct {
	int revealed;
	int is_mine;
	int is_flag;
	int neighbor_mines;
} tile;

void init_board(tile* board, int width, int height, int mines);

void find_neighbors(tile* board, int width, int height);

int check_win(tile* board, int width, int height, int mines);

int check_tile(tile* board, int width, int height, int x, int y);

#endif
