#include "environment.h"
#include <map>
#include <math.h>


bool is_inside(int x, int y, int n, int m, int dtype) {
	if (dtype == 2) {
		return (x >= 0 && x < m);
	}
	else {
		return (y >= 0 && y < n);
	}
}

int nearest(int x)
{
	int res = 1;
	while (x > res) {
		res *= 2;
	}
	return res;
}

std::vector<Action> gen_actions(int n, int m)
{
	int topX, topY, botX, botY, width, height, mxSide, sz, diceNum;
	std::vector<Action> actions;

	// urgelj m ni urt tal ni baina 
	if (n > m) std::swap(n, m);

	// urgelj inside the board
	if (1) {
		for (topY = 1; topY < n - 1; topY++) {
			for (topX = 1; topX < m - 1; topX++) {
				for (sz = 1; sz <= min(n - topY - 1, m - topX - 1); sz *= 2) {
					// 3rd diceNum of this size
					diceNum = log2(sz) * 3;
					for (int s = 0; s < 4; s++) {
						if (sz == 1) {
							Action action(topX, topY, diceNum, s);
							actions.push_back(action);
						}
						else {
							for (int i = 0; i < 3; i++) {
								Action action(topX, topY, (diceNum - i), s);
								actions.push_back(action);
							}
						}
					}
				}
			}
		}
	}

	// corner contained actions

	// topleft corner contained actions
	if (1) {
		topX = 0;
		topY = 0;

		for (botY = 0; botY < n - 1; botY++) {
			for (botX = 0; botX < m - 1; botX++) {
				width = botX - topX + 1;
				height = botY - topY + 1;
				mxSide = max(width, height);
				sz = nearest(mxSide);

				// 3rd diceNum of this size
				diceNum = log2(sz) * 3;

				if (sz == 1) {
					for (int i = 0; i <= 2; i += 2) {
						Action action(topX, topY, 0, i);
						actions.push_back(action);
					}
					continue;
				}

				// TYPE I
				for (int i = 0; i <= 2; i += 2) {
					Action action(botX - sz + 1, botY - sz + 1, diceNum - 2, i);
					actions.push_back(action);
				}


				// TYPE II
				if (height > 2) {
					for (int i = 0; i < 3; i++) {
						Action action(botX - sz + 1, botY - sz + 1, diceNum - 1, i);
						actions.push_back(action);
					}
				}

				// TYPE III
				if (width > 2) {
					for (int i = 0; i < 4; i++) {
						if (i == 1) continue;
						Action action(botX - sz + 1, botY - sz + 1, diceNum, i);
						actions.push_back(action);
					}
				}
			}
		}
	}

	// topright corner contained actions        
	if (1) {
		topY = 0;
		botX = m - 1;

		for (botY = 0; botY < n - 1; botY++) {
			for (topX = 1; topX <= m - 1; topX++) {

				height = botY - topY + 1;
				width = botX - topX + 1;
				mxSide = max(height, width);
				sz = nearest(mxSide);

				// 3th type dice of this size
				diceNum = (log2(sz) * 3);

				if (sz == 1) continue;

				// TYPE II
				if (height > 2) {
					for (int i = 0; i < 2; i++) {
						Action action(topX, botY - sz + 1, (diceNum - 1), i);
						actions.push_back(action);
					}
				}

				// TYPE III
				if (width != 1) {
					for (int i = 2; i < 4; i++) {
						Action action(topX, botY - sz + 1, (diceNum), i);
						actions.push_back(action);
					}
				}

			}
		}


	}

	// bottomleft corner contained actions
	if (1) {
		topX = 0;
		botY = n - 1;

		for (topY = 1; topY < n; topY++) {
			for (botX = 0; botX < m - 1; botX++) {

				height = botY - topY + 1;
				width = botX - topX + 1;
				mxSide = max(height, width);
				sz = nearest(mxSide);

				// 3th type dice of this size
				diceNum = (log2(sz) * 3);

				if (sz == 1) continue;

				// TYPE II
				if (height != 1) {
					for (int i = 0; i < 2; i++) {
						Action action(botX - sz + 1, topY, (diceNum - 1), i);
						actions.push_back(action);
					}
				}

				// TYPE III
				if (width > 2) {
					for (int i = 2; i < 4; i++) {
						Action action(botX - sz + 1, topY, diceNum, i);
						actions.push_back(action);
					}
				}

			}
		}
	}

	// bottomright corner contained actions
	if (1) {
		botX = m - 1;
		botY = n - 1;

		for (topY = 1; topY < n; topY++) {
			for (topX = 1; topX < m; topX++) {

				height = botY - topY + 1;
				width = botX - topX + 1;
				mxSide = max(height, width);
				sz = nearest(mxSide);

				if (sz == 1) {
					for (int i = 1; i <= 3; i += 2) {
						Action action(topX, topY, 0, i);
						actions.push_back(action);
					}
					continue;
				}

				// 3th type dice of this size
				diceNum = (log2(sz) * 3);

				// TYPE I
				for (int i = 1; i <= 3; i += 2) {
					Action action(topX, topY, (diceNum - 2), i);
					actions.push_back(action);
				}

				// TYPE II
				if (height != 1) {
					for (int i = 0; i < 4; i++) {
						if ((i == 2)) continue;
						Action action(topX, topY, (diceNum - 1), i);
						actions.push_back(action);
					}
				}

				// TYPE III
				if (width != 1) {
					for (int i = 1; i < 4; i++) {
						Action action(topX, topY, diceNum, i);
						actions.push_back(action);
					}
				}

			}
		}
	}

	// pairs of corners

	if (1) {

		// from above
		if (1) {
			topX = 0;
			topY = 0;
			botX = m - 1;

			for (botY = 0; botY < n - 1; botY++) {
				width = m;
				height = botY - topY + 1;
				mxSide = max(width, height);
				sz = nearest(mxSide);
				// 3th type dice of this size
				diceNum = (log2(sz) * 3);

				// TYPE I
				Action action(botX - sz + 1, botY - sz + 1, diceNum - 2, 0);
				actions.push_back(action);

				// TYPE II
				if (height > 2) {
					for (int i = 0; i < 2; i++) {
						Action action(botX - sz + 1, botY - sz + 1, diceNum - 1, i);
						actions.push_back(action);
					}
				}

				// TYPE III
				if (m % 2 == 0) {
					Action action1(botX - sz + 1, botY - sz + 1, diceNum, 0);
					actions.push_back(action1);
				}
				else {
					Action action1(botX + 1 - sz + 1, botY - sz + 1, diceNum, 0);
					actions.push_back(action1);
				}

			}

		}

		// from left
		if (1) {
			topX = 0;
			topY = 0;
			botY = n - 1;
			for (botX = 0; botX < m - 1; botX++) {
				width = botX - topX + 1;
				height = n;
				mxSide = max(width, height);
				sz = nearest(mxSide);
				// 3th type dice of this size
				diceNum = (log2(sz) * 3);

				// TYPE I
				Action action(botX - sz + 1, botY - sz + 1, diceNum - 2, 2);
				actions.push_back(action);

				// TYPE II
				if (n % 2 == 0) {
					Action action(botX - sz + 1, botY - sz + 1, diceNum - 1, 2);
					actions.push_back(action);
				}
				else {
					Action action(botX - sz + 1, botY + 1 - sz + 1, diceNum - 1, 2);
					actions.push_back(action);
				}

				// TYPE III
				if (width > 2) {
					for (int i = 2; i < 4; i++) {
						Action action(botX - sz + 1, botY - sz + 1, diceNum, i);
						actions.push_back(action);
					}
				}
			}
		}

		// from right
		if (1) {
			topX = m - 1;
			topY = 0;
			botY = n - 1;
			for (botX = m - 1; botX > 0; botX--) {
				width = topX - botX + 1;
				height = n;
				mxSide = max(width, height);
				sz = nearest(mxSide);
				// 3th type dice of this size
				diceNum = (log2(sz) * 3);

				// TYPE III
				if (width != 1) {
					for (int i = 2; i < 4; i++) {
						Action action(botX, botY - sz + 1, diceNum, i);
						actions.push_back(action);
					}
				}
			}



		}

		// from bottom
		if (1) {
			topX = 0;
			botX = m - 1;
			botY = n - 1;
			for (topY = n - 1; topY > 0; topY--) {
				width = m;
				height = topY - botY + 1;
				mxSide = max(width, height);
				sz = nearest(mxSide);
				// 3th type dice of this size
				diceNum = (log2(sz) * 3);

				// TYPE II
				if (height != 1) {
					for (int i = 0; i < 2; i++) {
						Action action(topX, topY, diceNum - 1, i);
						actions.push_back(action);
					}
				}

				// TYPE III???

			}
		}

	}

	// edge 

	// top
	if (1) {
		topY = 0;
		for (topX = 1; topX < m - 1; topX++) {
			for (width = 1; width <= (m - 1 - topX); width *= 2) {
				for (height = 1; height <= min(width, n - 1); height++) {
					sz = width;

					if (sz == 1) {
						if (topX == 1 || topX == m - 2) continue;
						for (int i = 0; i < 4; i++) {
							if (i == 1) continue;
							Action action(topX, topY, 0, i);
							actions.push_back(action);
						}
						continue;
					}

					// 3th type dice of this size
					diceNum = (log2(sz) * 3);

					// TYPE I
					for (int i = 0; i < 4; i++) {
						if (i == 1) continue;
						Action action(topX, height - width, (diceNum - 2), i);
						actions.push_back(action);
					}


					// TYPE II
					if (height > 2) {
						for (int i = 0; i < 4; i++) {
							Action action(topX, height - width, (diceNum - 1), i);
							actions.push_back(action);
						}
					}

					// TYPE III
					if (topX == 1 || topX == m - 2) continue;
					if (sz == 2 && height == 1) continue;
					for (int i = 0; i < 4; i++) {
						if (i == 1) continue;
						Action action(topX, height - width, diceNum, i);
						actions.push_back(action);
					}
				}
			}
		}
	}

	// bottom
	if (1) {
		topY = n - 1;
		for (topX = 1; topX < m - 1; topX++) {
			for (width = 1; width <= (m - 1 - topX); width *= 2) {
				for (height = 1; height <=min(width, n - 1); height++) {

					int oh = n - height;
					int ow = width;

					sz = width;

					if (sz == 1) {
						if (topX == 1 || topX == m - 2) continue;
						if (oh > ow) {
							Action action(topX, topY, 0, 1);
							actions.push_back(action);
						}
						for (int i = 2; i < 4; i++) {
							Action action(topX, topY, 0, i);
							actions.push_back(action);
						}
						continue;
					}

					// 3th type dice of this size
					diceNum = (log2(sz) * 3);

					// TYPE I
					if (oh > ow) {
						Action action(topX, topY - height + 1, (diceNum - 2), 1);
						actions.push_back(action);
					}
					for (int i = 2; i < 4; i++) {
						Action action(topX, topY - height + 1, (diceNum - 2), i);
						actions.push_back(action);
					}


					// TYPE II
					if (height != 1) {
						for (int i = 0; i < 4; i++) {
							Action action(topX, topY - height + 1, (diceNum - 1), i);
							actions.push_back(action);
						}
					}

					// TYPE III
					if (topX == 1 || topX == m - 2) continue;
					if (sz == 2 && height == 1) continue;
					if (oh > ow) {
						Action action(topX, topY - height + 1, diceNum, 1);
						actions.push_back(action);
					}

					for (int i = 2; i < 4; i++) {
						Action action(topX, topY - height + 1, diceNum, i);
						actions.push_back(action);
					}
				}
			}
		}
	}

	// left
	if (1) {
		topX = 0;
		for (topY = 1; topY < n - 1; topY++) {
			for (height = 1; height <= (n - 1 - topY); height *= 2) {
				for (width = 1; width <= height; width++) {
					sz = height;

					if (sz == 1) {
						if (topY == 1 || topY == n - 2) continue;
						for (int i = 0; i < 3; i++) {
							Action action(width - height, topY, 0, i);
							actions.push_back(action);
						}
						continue;
					}

					// 3th type dice of this size
					diceNum = (log2(sz) * 3);

					// TYPE I
					for (int i = 0; i < 3; i++) {
						Action action(width - height, topY, (diceNum - 2), i);
						actions.push_back(action);
					}

					// TYPE III
					if (width > 2) {
						for (int i = 0; i < 4; i++) {
							Action action(width - height, topY, diceNum, i);
							actions.push_back(action);
						}
					}

					// TYPE II
					if (topY == 1 || topY == n - 2) continue;
					if (sz == 2 && width == 1) continue;
					for (int i = 0; i < 3; i++) {
						Action action(width - height, topY, (diceNum - 1), i);
						actions.push_back(action);
					}

				}
			}
		}
	}

	// right
	if (1) {
		topX = m - 1;
		for (topY = 1; topY < n - 1; topY++) {
			for (height = 1; height <= (n - 1 - topY); height *= 2) {
				for (width = 1; width <= height; width++) {
					sz = height;

					int oh = height;
					int ow = m - width;

					if (sz == 1) {
						if (topY == 1 || topY == n - 2) continue;
						if (ow > oh) {
							Action action(topX, topY, 0, 3);
							actions.push_back(action);
						}
						for (int i = 0; i < 2; i++) {
							Action action(topX, topY, 0, i);
							actions.push_back(action);
						}
						continue;
					}

					// 3th type dice of this size
					diceNum = (log2(sz) * 3);

					// TYPE I
					if (ow > oh) {
						Action action(topX - width + 1, topY, (diceNum - 2), 3);
						actions.push_back(action);
					}
					for (int i = 0; i < 2; i++) {
						Action action(topX - width + 1, topY, (diceNum - 2), i);
						actions.push_back(action);
					}


					// TYPE III
					if (width != 1) {
						for (int i = 0; i < 4; i++) {
							Action action(topX - width + 1, topY, diceNum, i);
							actions.push_back(action);
						}
					}


					// TYPE II
					if (topY == 1 || topY == n - 2) continue;
					if (sz == 2 && width == 1) continue;

					if (ow > oh) {
						Action action(topX - width + 1, topY, diceNum - 1, 3);
						actions.push_back(action);
					}

					for (int i = 0; i < 2; i++) {
						Action action(topX - width + 1, topY, diceNum - 1, i);
						actions.push_back(action);
					}

				}
			}
		}
	}

	// pairs of edges
	if (1) {
		topY = 0;
		sz = log2(n - 1) + 1;
		for (int szz = pow(2, sz); szz < m - 1; szz *= 2) {
			for (topX = 1; topX < m - szz; topX++) {
				// 3th type dice of this size
				if (n + 1 < m && szz <= m - 2 && topX + szz < m) {
					diceNum = log2(szz) * 3;
					// TYPE I
					for (int i = 2; i < 4; i++) {
						Action action(topX, topY, diceNum - 2, i);
						actions.push_back(action);
					}

					// TYPE II
					for (int i = 2; i < 4; i++) {
						Action action(topX, topY, diceNum - 1, i);
						actions.push_back(action);
					}

					if (topX == 1 || topX == m - 2) continue;

					// TYPE III
					for (int i = 2; i < 4; i++) {
						Action action(topX, topY, diceNum, i);
						actions.push_back(action);
					}
				}
			}
		}
	}

	return actions;
}

Environment::~Environment() {
}

GameState::GameState(std::vector<std::vector<int>> state, std::vector<std::vector<int>> goalstate, int height , int width) {
	this->goalstate=goalstate; 
	this->state = state;
	this->height_state = height;
	this->width_state = width;
	this->actions=gen_actions(height, width) ; 	
	this->numActions=actions.size() ; 
}

GameState::~GameState() {}


GameState *GameState::getNextState( int x , int y, int die_index, int dir) const {
	std::vector<std::vector<int>>b(this->state);
	// std::vector<std::vector<int>>& b = board.pieces;
	int n = this->height_state;
	int m = this->width_state;
	// Set up
	int power = ceil(die_index / 3.0);
	int size = int(pow(2, power));
	int dice_type = (power == 0) ? 1 : die_index - ((power - 1) * 3);

	// cout << "die_index:" << die_index << ", n:" << n << ", m:" << m << ", power:" << power << ", size:" << size 
	//      << ", dice_type:" << dice_type << ", dir:" << action.dir << endl;

	std::vector<int> cut_pieces;

	int x_start = max(x, 0);
	int y_start = max(y, 0);
	int x_end = min(m, x + size) - 1;
	int y_end = min(n, y + size) - 1;

	int width = x_end - x_start + 1;
	int height = y_end - y_start + 1;

	int chosen_row_num = floor(height / 2);
	int chosen_col_num = floor(width / 2);
	int first = 1;

	// Determining first and chosen_x_num
	if (dice_type == 2) {
		first = (height + 1) % 2;
		if (is_inside(x, y, n, m, dice_type)) {
			if (!first) {
				chosen_row_num += 1;
			}
			first = 1;
		}
		else if (abs(y) % 2 != 0) {
			first = 0;
		}
		else if (abs(y) % 2 == 0) {
			if (height % 2 == 1) { // n bsn
				first = 1;
				chosen_row_num += 1;
			}
		}
	}

	if (dice_type == 3) {
		first = (width + 1) % 2;
		if (is_inside(x, y, n, m, dice_type)) {
			if (!first) {
				chosen_col_num += 1;
			}
			first = 1;
		}
		else if (abs(x) % 2 != 0) {
			first = 0;
		}
		else if (abs(x) % 2 == 0) {
			if (width % 2 == 1) { // m bsn
				first = 1;
				chosen_col_num += 1;
			}
		}
	}

	// cout << "x_start:" << x_start << ", x_end:" << x_end << ", y_start:" << y_start 
	//      << ", y_end:" << y_end << ", width:" << width << ", height:" << height 
	//      << ", chosen_row_num:" << chosen_row_num << ", chosen_col_num:" << chosen_col_num << endl;

	// CUT PHASE
	if (dice_type == 1) {
		for (int r = y_start; r <= y_end; ++r) {
			for (int c = x_start; c <= x_end; ++c) {
				cut_pieces.push_back(b[r][c]);
				b[r][c] = 0;
			}
		}
	}

	if (dice_type == 2) {
		int x = (first + 1) % 2;
		if (is_inside(x, y, n, m, dice_type)) {
			for (int r = y_start; r <= y_end; r += 2) {
				for (int c = x_start; c <= x_end; ++c) {
					cut_pieces.push_back(b[r][c]);
					b[r][c] = 0;
				}
			}
		}
		else {
			for (int r = y_start + x; r <= y_end; r += 2) {
				for (int c = x_start; c <= x_end; ++c) {
					cut_pieces.push_back(b[r][c]);
					b[r][c] = 0;
				}
			}
		}
	}

	if (dice_type == 3) {
		int x = (first + 1) % 2;
		if (is_inside(x, y, n, m, dice_type)) {
			for (int r = y_start; r <= y_end; ++r) {
				for (int c = x_start; c <= x_end; c += 2) {
					cut_pieces.push_back(b[r][c]);
					b[r][c] = 0;
				}
			}
		}
		else {
			for (int r = y_start; r <= y_end; ++r) {
				for (int c = x_start + x; c <= x_end; c += 2) {
					cut_pieces.push_back(b[r][c]);
					b[r][c] = 0;
				}
			}
		}
	}

	// SHIFT PHASE
	if (dir == 0) {
		for (int c = x_start; c <= x_end; ++c) {
			int write_index = y_start;
			for (int r = y_start; r < n; ++r) {
				if (b[r][c] != 0) {
					b[write_index][c] = b[r][c];
					++write_index;
				}
			}
			for (int r = write_index; r < n; ++r) {
				b[r][c] = 0;
			}
		}
	}
	else if (dir == 1) {
		for (int c = x_start; c <= x_end; ++c) {
			int write_index = y_end;
			for (int r = y_end; r >= 0; --r) {
				if (b[r][c] != 0) {
					b[write_index][c] = b[r][c];
					--write_index;
				}
			}
			for (int r = write_index; r >= 0; --r) {
				b[r][c] = 0;
			}
		}
	}
	else if (dir == 2) {
		for (int r = y_start; r <= y_end; ++r) {
			int write_index = x_start;
			for (int c = x_start; c < m; ++c) {
				if (b[r][c] != 0) {
					b[r][write_index] = b[r][c];
					++write_index;
				}
			}
			for (int c = write_index; c < m; ++c) {
				b[r][c] = 0;
			}
		}
	}
	else if (dir == 3) {
		for (int r = y_start; r <= y_end; ++r) {
			int write_index = x_end;
			for (int c = x_end; c >= 0; --c) {
				if (b[r][c] != 0) {
					b[r][write_index] = b[r][c];
					--write_index;
				}
			}
			for (int c = write_index; c >= 0; --c) {
				b[r][c] = 0;
			}
		}
	}

	// BBBT PHASE
	int bxs = 0, bxe = 0, bys = 0, bye = 0;

	if (dice_type == 1) {
		if (dir < 2) {
			bxs = x_start;
			bxe = x_end;
			if (dir == 0) {
				bys = n - height;
				bye = n - 1;
			}
			else {
				bys = 0;
				bye = height - 1;
			}
		}
		else {
			bys = y_start;
			bye = y_end;
			if (dir == 2) {
				bxs = m - width;
				bxe = m - 1;
			}
			else {
				bxs = 0;
				bxe = width - 1;
			}
		}
	}
	else if (dice_type == 2) {
		if (dir < 2) {
			bxs = x_start;
			bxe = x_end;
			if (dir == 0) {
				bys = n - chosen_row_num;
				bye = n - 1;
			}
			else {
				bys = 0;
				bye = chosen_row_num - 1;
			}
		}
		else {
			bys = y_start;
			bye = y_end;
			if (dir == 2) {
				bxs = m - width;
				bxe = m - 1;
			}
			else {
				bxs = 0;
				bxe = width - 1;
			}
		}
	}
	else if (dice_type == 3) {
		if (dir < 2) {
			bxs = x_start;
			bxe = x_end;
			if (dir == 0) {
				bys = n - height;
				bye = n - 1;
			}
			else {
				bys = 0;
				bye = height - 1;
			}
		}
		else {
			bys = y_start;
			bye = y_end;
			if (dir == 2) {
				bxs = m - chosen_col_num;
				bxe = m - 1;
			}
			else {
				bxs = 0;
				bxe = chosen_col_num - 1;
			}
		}
	}

	int cnt = 0;
	if (dice_type == 1) {
		for (int r = bys; r <= bye; ++r) {
			for (int c = bxs; c <= bxe; ++c) {
				b[r][c] = cut_pieces[cnt];
				cnt++;
			}
		}
	}
	else if (dice_type == 2 || dice_type == 3) {
		for (int r = bys; r <= bye; ++r) {
			for (int c = bxs; c <= bxe; ++c) {
				if (b[r][c] == 0) {
					b[r][c] = cut_pieces[cnt];
					cnt++;
				}
			}
		}
	}
	// std::vector<Action> newActions = this->actions;
	// newActions.push_back(action);
	GameState *nextS =new GameState(b, this->goalstate, this->height_state, this->width_state );
	
	return nextS;
}

std::vector<Environment*> GameState::getNextStates() const {
    std::vector<Environment*> nextStates;
    for (const Action& action : this->actions) {
       
	   int x= action.x; 
	   int y=action.y; 
	   int die_index=action.die_index; 
	   int dir=action.direction; 
        nextStates.push_back(this->getNextState(x, y, die_index, dir));
    }
    return (nextStates);
}

std::vector<std::vector<int>> GameState::getState() const {
	return(this->state);
}

bool GameState::isSolved() const {
	bool isSolved = true;
	if (state != goalstate){
		isSolved=false; 
	}

	return(isSolved);
}


int GameState::getNumActions() const {
	return(this->numActions);
}
