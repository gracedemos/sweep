#include "sweep.h"
#include <stdlib.h>
#include <string.h>

void init_board(tile* board, int width, int height, int mines)
{
	int board_size = width * height;
	memset(board, 0, board_size * sizeof(tile));
	for (int i = 0; i < mines; i++) {
		int index = rand() % board_size;
		while (board[index].is_mine)
			index = rand() % board_size;
		board[index].is_mine = 1;
	}

	find_neighbors(board, width, height);
}

int check_win(tile* board, int width, int height, int mines)
{
	int total = 0;
	for (int i = 0; i < width * height; i++)
		total += board[i].revealed;

	if (total == width * height - mines)
		return 1;
	return 0;
}

int find_neighbors_inner(tile* board, int width, int height, int x, int y)
{
	if (x < 0 || x >= width || y < 0 || y >= height)
		return 0;
	return board[y * width + x].is_mine;
}

void find_neighbors(tile* board, int width, int height)
{
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			board[y * width + x].neighbor_mines += find_neighbors_inner(board, width, height, x - 1, y - 1);
			board[y * width + x].neighbor_mines += find_neighbors_inner(board, width, height, x, y - 1);
			board[y * width + x].neighbor_mines += find_neighbors_inner(board, width, height, x + 1, y - 1);
			board[y * width + x].neighbor_mines += find_neighbors_inner(board, width, height, x - 1, y);
			board[y * width + x].neighbor_mines += find_neighbors_inner(board, width, height, x + 1, y);
			board[y * width + x].neighbor_mines += find_neighbors_inner(board, width, height, x - 1, y + 1);
			board[y * width + x].neighbor_mines += find_neighbors_inner(board, width, height, x, y + 1);
			board[y * width + x].neighbor_mines += find_neighbors_inner(board, width, height, x + 1, y + 1);
		}
	}
}

int check_tile(tile* board, int width, int height, int x, int y)
{
	if (x < 0 || x >= width || y < 0 || y >= height)
		return -1;

	int index = y * width + x;
	if (board[index].revealed)
		return board[index].neighbor_mines;
	if (board[index].is_mine)
		return -1;
	board[index].revealed = 1;
	if (board[index].neighbor_mines > 0) {
		return board[index].neighbor_mines;
	}

	check_tile(board, width, height, x - 1, y - 1);
	check_tile(board, width, height, x, y - 1);
	check_tile(board, width, height, x + 1, y - 1);
	check_tile(board, width, height, x - 1, y);
	check_tile(board, width, height, x + 1, y);
	check_tile(board, width, height, x - 1, y + 1);
	check_tile(board, width, height, x, y + 1);
	check_tile(board, width, height, x + 1, y + 1);

	return 0;
}
