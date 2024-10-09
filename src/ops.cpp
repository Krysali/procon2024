#include <ops.h>
#include <iostream>
#include <algorithm>

void GameState::apply_die(const Action& action) {
	std::vector<int> punched_pieces = punch_pieces(action);
	shift_pieces(action);
	put_back_pieces(action, punched_pieces);
	actions.push_back(action);
}

std::vector<int> GameState::punch_pieces(const Action& action) {
	std::vector<int> punched_pieces;
	Die& die = dies[action.die_index];

	// Calculate the overlapping region
	int start_x = std::max(0, action.x);
	int start_y = std::max(0, action.y);
	int end_x = std::min(board.width, action.x + die.width);
	int end_y = std::min(board.height, action.y + die.height);

	if (action.direction == 2 || action.direction == 3) { // Left and Right (prioritize y)
		for (int dy = start_y; dy < end_y; ++dy) {
			for (int dx = start_x; dx < end_x; ++dx) {
				int die_x = dx - action.x;
				int die_y = dy - action.y;

				if (die.cells[die_y][die_x]) {
					punched_pieces.push_back(board.pieces[dy][dx]);
					board.pieces[dy][dx] = -1;
				}
			}
		}
	}
	else { // Up and Down (prioritize x)
		for (int dx = start_x; dx < end_x; ++dx) {
			for (int dy = start_y; dy < end_y; ++dy) {
				int die_x = dx - action.x;
				int die_y = dy - action.y;

				if (die.cells[die_y][die_x]) {
					punched_pieces.push_back(board.pieces[dy][dx]);
					board.pieces[dy][dx] = -1;
				}
			}
		}
	}

	return punched_pieces;
}

void GameState::shift_pieces(const Action& action) {

	if (action.direction == 2) { // Left
		for (int y = 0; y < board.height; ++y) {
			int dest_x = 0;
			bool row_shifted = false; // Flag to track row changes

			for (int x = 0; x < board.width; ++x) {
				if (board.pieces[y][x] != -1) {
					if (x != dest_x) { // Check for change in position
						board.pieces[y][dest_x] = board.pieces[y][x];
						row_shifted = true; // If there are any shifts
					}
					dest_x++;
				}
			}
			if (row_shifted) {
				// Fill remaining spaces with -1
				for (int fill = dest_x; fill < board.width; fill++) {
					board.pieces[y][fill] = -1;
				}
			}
		}
	}
	else if (action.direction == 3) { // Right
		for (int y = 0; y < board.height; ++y) {
			int dest_x = board.width - 1;
			bool row_shifted = false;

			for (int x = board.width - 1; x >= 0; --x) {
				if (board.pieces[y][x] != -1) {
					if (x != dest_x) {
						board.pieces[y][dest_x] = board.pieces[y][x];
						row_shifted = true;
					}
					dest_x--;
				}
			}
			if (row_shifted) {
				// Fill remaining spaces on the left with -1
				for (int fill = dest_x; fill >= 0; --fill) {
					board.pieces[y][fill] = -1;
				}
			}
		}
	}
	else if (action.direction == 0) { // Up
		for (int x = 0; x < board.width; ++x) {
			int dest_y = 0;
			bool column_shifted = false;

			for (int y = 0; y < board.height; ++y) {
				if (board.pieces[y][x] != -1) {
					if (y != dest_y) {
						board.pieces[dest_y][x] = board.pieces[y][x];
						column_shifted = true;
					}
					dest_y++;
				}
			}
			if (column_shifted) {
				// Fill remaining spaces at the bottom with -1
				for (int fill = dest_y; fill < board.height; ++fill) {
					board.pieces[fill][x] = -1;
				}
			}
		}
	}
	else if (action.direction == 1) { // Down
		for (int x = 0; x < board.width; ++x) {
			int dest_y = board.height - 1;
			bool column_shifted = false;

			for (int y = board.height - 1; y >= 0; --y) {
				if (board.pieces[y][x] != -1) {
					if (y != dest_y) {
						board.pieces[dest_y][x] = board.pieces[y][x];
						column_shifted = true;
					}
					dest_y--;
				}
			}
			if (column_shifted) {
				// Fill remaining spaces at the top with -1
				for (int fill = dest_y; fill >= 0; --fill) {
					board.pieces[fill][x] = -1;
				}
			}
		}
	}
}

void GameState::put_back_pieces(const Action& action, std::vector<int>& punched_pieces) {
	int piece_index = 0;

	if (action.direction == 2 || action.direction == 3) { // Left and Right
		for (int y = 0; y < board.height; ++y) {
			for (int x = 0; x < board.width; ++x) {
				if (board.pieces[y][x] == -1) {
					board.pieces[y][x] = punched_pieces[piece_index++];
				}
			}
		}
	}
	else if (action.direction == 0 || action.direction == 1) { // Up and Down
		for (int x = 0; x < board.width; ++x) {
			for (int y = 0; y < board.height; ++y) {
				if (board.pieces[y][x] == -1) {
					board.pieces[y][x] = punched_pieces[piece_index++];
				}
			}
		}
	}
}
