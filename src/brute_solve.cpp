#include <iostream>
#include <set>
#include <vector>
#include <algorithm>
#include <types.h>
using namespace std;

struct Data {

	int x1, y1, val;
};

int cnt = 0;

set<pair<int, int>> s0, s1, s2, s3;

void solve(GameState& game_state) {


	vector<Action> actions;

	vector<vector<int>>& a = game_state.board.pieces;
	vector<vector<int>>& b = game_state.goal_state.pieces;

	int n = game_state.board.height;
	int m = game_state.board.width;

	int dist, dist2;

	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++) {

			if (a[i][j] == 0) s0.insert({ i, j });
			else if (a[i][j] == 1) s1.insert({ i, j });
			else if (a[i][j] == 2) s2.insert({ i, j });
			else s3.insert({ i, j });
		}

	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++) {

			if (a[i][j] != b[i][j]) {

				vector<Data> v;
				Data data;

				//b[i][j] == 0
				if (b[i][j] == 0) {

					auto itr = s0.lower_bound({ i, j });

					pair<int, int> pr = *itr;

					dist = pr.first - i;
					int Y = pr.second;
					int X = pr.first;

					if (Y > j) {

						// ((I - j - dist2 + (m - j))) % (m - j) + j  ene bol Y ees j hurtel dist2 oor shahad ochih indexiig olno


						for (int I = m - 1; I >= j; I--) {

							Action act(0, X, j, 2);
							actions.push_back(act);


							if (a[X][I] == 0) {

								s0.erase({ X, I });
							}
							else if (a[X][I] == 1) {

								s1.erase({ X, I });
							}
							else if (a[X][I] == 2) {

								s2.erase({ X, I });
							}
							else {

								s3.erase({ X, I });
							}
						}

						dist2 = Y - j;
						for (int I = m - 1; I >= j; I--) {

							if (a[X][I] == 0) {

								s0.insert({ X, (I - j - dist2 + (m - j)) % (m - j) + j });
							}
							else if (a[X][I] == 1) {

								s1.insert({ X, (I - j - dist2 + (m - j)) % (m - j) + j });
							}
							else if (a[X][I] == 2) {

								s2.insert({ X, (I - j - dist2 + (m - j)) % (m - j) + j });
							}
							else {

								s3.insert({ X, (I - j - dist2 + (m - j)) % (m - j) + j });
							}

							data.x1 = X;
							data.y1 = (I - j - dist2 + (m - j)) % (m - j) + j;
							data.val = a[X][I];

							v.push_back(data);
						}
					}
					else {

						dist2 = j - Y;

						for (int I = 0; I <= j; I++) {

							Action act(0, X, j, 3);
							actions.push_back(act);

							if (a[X][I] == 0) {

								s0.erase({ X, I });
							}
							else if (a[X][I] == 1) {

								s1.erase({ X, I });
							}
							else if (a[X][I] == 2) {

								s2.erase({ X, I });
							}
							else {

								s3.erase({ X, I });
							}
						}

						for (int I = 0; I <= j; I++) {

							if (a[X][I] == 0) {

								s0.insert({ X, (I + dist2) % (j + 1) });
							}
							else if (a[X][I] == 1) {

								s1.insert({ X, (I + dist2) % (j + 1) });
							}
							else if (a[X][I] == 2) {

								s2.insert({ X, (I + dist2) % (j + 1) });
							}
							else {

								s3.insert({ X, (I + dist2) % (j + 1) });
							}

							data.x1 = X;
							data.y1 = (I + dist2) % (j + 1);
							data.val = a[X][I];

							v.push_back(data);
						}
					}

					// a massivaa update hiih
					for (int l = 0; l < v.size(); l++) {

						int xx = v[l].x1;
						int yy = v[l].y1;

						a[xx][yy] = v[l].val;
					}

					v.clear();
					//(J - i - dist + (n - i)) % (n - i) + i ene bol n - 1 ees j hurtel dist udaa yvaad garah index

					for (int J = n - 1; J >= i; J--) {

						Action act(0, i, j, 0);
						actions.push_back(act);

						if (a[J][j] == 0) {

							s0.erase({ J, j });
						}
						else if (a[J][j] == 1) {

							s1.erase({ J, j });
						}
						else if (a[J][j] == 2) {

							s2.erase({ J, j });
						}
						else {

							s3.erase({ J, j });
						}
					}

					for (int J = n - 1; J >= i; J--) {

						if (a[J][j] == 0) {

							s0.insert({ (J - i - dist + (n - i)) % (n - i) + i, j });
						}
						else if (a[J][j] == 1) {

							s1.insert({ (J - i - dist + (n - i)) % (n - i) + i, j });
						}
						else if (a[J][j] == 2) {

							s2.insert({ (J - i - dist + (n - i)) % (n - i) + i, j });
						}
						else {

							s3.insert({ (J - i - dist + (n - i)) % (n - i) + i, j });
						}

						data.x1 = (J - i - dist + (n - i)) % (n - i) + i;
						data.y1 = j;
						data.val = a[J][j];

						v.push_back(data);
					}

					// a massivaa update hiih
					for (int l = 0; l < v.size(); l++) {

						int xx = v[l].x1;
						int yy = v[l].y1;

						a[xx][yy] = v[l].val;
					}

					v.clear();
				}
				// b[i][j] == 1
				else if (b[i][j] == 1) {

					auto itr = s1.lower_bound({ i, j });

					pair<int, int> pr = *itr;

					dist = pr.first - i;
					int Y = pr.second;
					int X = pr.first;

					if (Y > j) {

						// ((I - j - dist2 + (m - j))) % (m - j) + j  ene bol Y ees j hurtel dist2 oor shahad ochih indexiig olno

						for (int I = m - 1; I >= j; I--) {

							Action act(0, X, j, 2);
							actions.push_back(act);

							if (a[X][I] == 0) {

								s0.erase({ X, I });
							}
							else if (a[X][I] == 1) {

								s1.erase({ X, I });
							}
							else if (a[X][I] == 2) {

								s2.erase({ X, I });
							}
							else {

								s3.erase({ X, I });
							}
						}

						dist2 = Y - j;
						for (int I = m - 1; I >= j; I--) {

							if (a[X][I] == 0) {

								s0.insert({ X, (I - j - dist2 + (m - j)) % (m - j) + j });
							}
							else if (a[X][I] == 1) {

								s1.insert({ X, (I - j - dist2 + (m - j)) % (m - j) + j });
							}
							else if (a[X][I] == 2) {

								s2.insert({ X, (I - j - dist2 + (m - j)) % (m - j) + j });
							}
							else {

								s3.insert({ X, (I - j - dist2 + (m - j)) % (m - j) + j });
							}

							data.x1 = X;
							data.y1 = (I - j - dist2 + (m - j)) % (m - j) + j;
							data.val = a[X][I];

							v.push_back(data);
						}
					}
					else {

						dist2 = j - Y;

						for (int I = 0; I <= j; I++) {

							Action act(0, X, j, 3);
							actions.push_back(act);

							if (a[X][I] == 0) {

								s0.erase({ X, I });
							}
							else if (a[X][I] == 1) {

								s1.erase({ X, I });
							}
							else if (a[X][I] == 2) {

								s2.erase({ X, I });
							}
							else {

								s3.erase({ X, I });
							}
						}

						for (int I = 0; I <= j; I++) {

							if (a[X][I] == 0) {

								s0.insert({ X, (I + dist2) % (j + 1) });
							}
							else if (a[X][I] == 1) {

								s1.insert({ X, (I + dist2) % (j + 1) });
							}
							else if (a[X][I] == 2) {

								s2.insert({ X, (I + dist2) % (j + 1) });
							}
							else {

								s3.insert({ X, (I + dist2) % (j + 1) });
							}

							data.x1 = X;
							data.y1 = (I + dist2) % (j + 1);
							data.val = a[X][I];

							v.push_back(data);
						}
					}

					// a massivaa update hiih
					for (int l = 0; l < v.size(); l++) {

						int xx = v[l].x1;
						int yy = v[l].y1;

						a[xx][yy] = v[l].val;
					}

					v.clear();
					//(J - i - dist + (n - i)) % (n - i) + i ene bol n - 1 ees j hurtel dist udaa yvaad garah index

					for (int J = n - 1; J >= i; J--) {

						Action act(0, i, j, 0);
						actions.push_back(act);

						if (a[J][j] == 0) {

							s0.erase({ J, j });
						}
						else if (a[J][j] == 1) {

							s1.erase({ J, j });
						}
						else if (a[J][j] == 2) {

							s2.erase({ J, j });
						}
						else {

							s3.erase({ J, j });
						}
					}

					for (int J = n - 1; J >= i; J--) {

						if (a[J][j] == 0) {

							s0.insert({ (J - i - dist + (n - i)) % (n - i) + i, j });
						}
						else if (a[J][j] == 1) {

							s1.insert({ (J - i - dist + (n - i)) % (n - i) + i, j });
						}
						else if (a[J][j] == 2) {

							s2.insert({ (J - i - dist + (n - i)) % (n - i) + i, j });
						}
						else {

							s3.insert({ (J - i - dist + (n - i)) % (n - i) + i, j });
						}

						data.x1 = (J - i - dist + (n - i)) % (n - i) + i;
						data.y1 = j;
						data.val = a[J][j];

						v.push_back(data);
					}

					// a massivaa update hiih
					for (int l = 0; l < v.size(); l++) {

						int xx = v[l].x1;
						int yy = v[l].y1;

						a[xx][yy] = v[l].val;
					}

					v.clear();
				}
				// b[i][j] == 2
				else if (b[i][j] == 2) {

					auto itr = s2.lower_bound({ i, j });

					pair<int, int> pr = *itr;

					dist = pr.first - i;
					int Y = pr.second;
					int X = pr.first;

					if (Y > j) {

						// ((I - j - dist2 + (m - j))) % (m - j) + j  ene bol Y ees j hurtel dist2 oor shahad ochih indexiig olno

						for (int I = m - 1; I >= j; I--) {

							Action act(0, X, j, 2);
							actions.push_back(act);

							if (a[X][I] == 0) {

								s0.erase({ X, I });
							}
							else if (a[X][I] == 1) {

								s1.erase({ X, I });
							}
							else if (a[X][I] == 2) {

								s2.erase({ X, I });
							}
							else {

								s3.erase({ X, I });
							}
						}

						dist2 = Y - j;
						for (int I = m - 1; I >= j; I--) {

							if (a[X][I] == 0) {

								s0.insert({ X, (I - j - dist2 + (m - j)) % (m - j) + j });
							}
							else if (a[X][I] == 1) {

								s1.insert({ X, (I - j - dist2 + (m - j)) % (m - j) + j });
							}
							else if (a[X][I] == 2) {

								s2.insert({ X, (I - j - dist2 + (m - j)) % (m - j) + j });
							}
							else {

								s3.insert({ X, (I - j - dist2 + (m - j)) % (m - j) + j });
							}

							data.x1 = X;
							data.y1 = (I - j - dist2 + (m - j)) % (m - j) + j;
							data.val = a[X][I];

							v.push_back(data);
						}
					}
					else {

						dist2 = j - Y;

						for (int I = 0; I <= j; I++) {

							Action act(0, X, j, 3);
							actions.push_back(act);

							if (a[X][I] == 0) {

								s0.erase({ X, I });
							}
							else if (a[X][I] == 1) {

								s1.erase({ X, I });
							}
							else if (a[X][I] == 2) {

								s2.erase({ X, I });
							}
							else {

								s3.erase({ X, I });
							}
						}

						for (int I = 0; I <= j; I++) {

							if (a[X][I] == 0) {

								s0.insert({ X, (I + dist2) % (j + 1) });
							}
							else if (a[X][I] == 1) {

								s1.insert({ X, (I + dist2) % (j + 1) });
							}
							else if (a[X][I] == 2) {

								s2.insert({ X, (I + dist2) % (j + 1) });
							}
							else {

								s3.insert({ X, (I + dist2) % (j + 1) });
							}

							data.x1 = X;
							data.y1 = (I + dist2) % (j + 1);
							data.val = a[X][I];

							v.push_back(data);
						}
					}

					// a massivaa update hiih
					for (int l = 0; l < v.size(); l++) {

						int xx = v[l].x1;
						int yy = v[l].y1;

						a[xx][yy] = v[l].val;
					}

					v.clear();
					//(J - i - dist + (n - i)) % (n - i) + i ene bol n - 1 ees j hurtel dist udaa yvaad garah index

					for (int J = n - 1; J >= i; J--) {

						Action act(0, i, j, 0);
						actions.push_back(act);

						if (a[J][j] == 0) {

							s0.erase({ J, j });
						}
						else if (a[J][j] == 1) {

							s1.erase({ J, j });
						}
						else if (a[J][j] == 2) {

							s2.erase({ J, j });
						}
						else {

							s3.erase({ J, j });
						}
					}

					for (int J = n - 1; J >= i; J--) {

						if (a[J][j] == 0) {

							s0.insert({ (J - i - dist + (n - i)) % (n - i) + i, j });
						}
						else if (a[J][j] == 1) {

							s1.insert({ (J - i - dist + (n - i)) % (n - i) + i, j });
						}
						else if (a[J][j] == 2) {

							s2.insert({ (J - i - dist + (n - i)) % (n - i) + i, j });
						}
						else {

							s3.insert({ (J - i - dist + (n - i)) % (n - i) + i, j });
						}

						data.x1 = (J - i - dist + (n - i)) % (n - i) + i;
						data.y1 = j;
						data.val = a[J][j];

						v.push_back(data);
					}

					// a massivaa update hiih
					for (int l = 0; l < v.size(); l++) {

						int xx = v[l].x1;
						int yy = v[l].y1;

						a[xx][yy] = v[l].val;
					}

					v.clear();
				}
				//b[i][j] == 3
				else {

					auto itr = s3.lower_bound({ i, j });

					pair<int, int> pr = *itr;

					dist = pr.first - i;
					int Y = pr.second;
					int X = pr.first;

					if (Y > j) {

						// ((I - j - dist2 + (m - j))) % (m - j) + j  ene bol Y ees j hurtel dist2 oor shahad ochih indexiig olno

						for (int I = m - 1; I >= j; I--) {

							Action act(0, X, j, 2);
							actions.push_back(act);

							if (a[X][I] == 0) {

								s0.erase({ X, I });
							}
							else if (a[X][I] == 1) {

								s1.erase({ X, I });
							}
							else if (a[X][I] == 2) {

								s2.erase({ X, I });
							}
							else {

								s3.erase({ X, I });
							}
						}

						dist2 = Y - j;
						for (int I = m - 1; I >= j; I--) {

							if (a[X][I] == 0) {

								s0.insert({ X, (I - j - dist2 + (m - j)) % (m - j) + j });
							}
							else if (a[X][I] == 1) {

								s1.insert({ X, (I - j - dist2 + (m - j)) % (m - j) + j });
							}
							else if (a[X][I] == 2) {

								s2.insert({ X, (I - j - dist2 + (m - j)) % (m - j) + j });
							}
							else {

								s3.insert({ X, (I - j - dist2 + (m - j)) % (m - j) + j });
							}

							data.x1 = X;
							data.y1 = (I - j - dist2 + (m - j)) % (m - j) + j;
							data.val = a[X][I];

							v.push_back(data);
						}
					}
					else {

						dist2 = j - Y;

						for (int I = 0; I <= j; I++) {

							Action act(0, X, j, 3);
							actions.push_back(act);

							if (a[X][I] == 0) {

								s0.erase({ X, I });
							}
							else if (a[X][I] == 1) {

								s1.erase({ X, I });
							}
							else if (a[X][I] == 2) {

								s2.erase({ X, I });
							}
							else {

								s3.erase({ X, I });
							}
						}

						for (int I = 0; I <= j; I++) {

							if (a[X][I] == 0) {

								s0.insert({ X, (I + dist2) % (j + 1) });
							}
							else if (a[X][I] == 1) {

								s1.insert({ X, (I + dist2) % (j + 1) });
							}
							else if (a[X][I] == 2) {

								s2.insert({ X, (I + dist2) % (j + 1) });
							}
							else {

								s3.insert({ X, (I + dist2) % (j + 1) });
							}

							data.x1 = X;
							data.y1 = (I + dist2) % (j + 1);
							data.val = a[X][I];

							v.push_back(data);
						}
					}

					// a massivaa update hiih
					for (int l = 0; l < v.size(); l++) {

						int xx = v[l].x1;
						int yy = v[l].y1;

						a[xx][yy] = v[l].val;
					}

					v.clear();
					//(J - i - dist + (n - i)) % (n - i) + i ene bol n - 1 ees j hurtel dist udaa yvaad garah index

					for (int J = n - 1; J >= i; J--) {

						Action act(0, i, j, 0);
						actions.push_back(act);

						if (a[J][j] == 0) {

							s0.erase({ J, j });
						}
						else if (a[J][j] == 1) {

							s1.erase({ J, j });
						}
						else if (a[J][j] == 2) {

							s2.erase({ J, j });
						}
						else {

							s3.erase({ J, j });
						}
					}

					for (int J = n - 1; J >= i; J--) {

						if (a[J][j] == 0) {

							s0.insert({ (J - i - dist + (n - i)) % (n - i) + i, j });
						}
						else if (a[J][j] == 1) {

							s1.insert({ (J - i - dist + (n - i)) % (n - i) + i, j });
						}
						else if (a[J][j] == 2) {

							s2.insert({ (J - i - dist + (n - i)) % (n - i) + i, j });
						}
						else {

							s3.insert({ (J - i - dist + (n - i)) % (n - i) + i, j });
						}

						data.x1 = (J - i - dist + (n - i)) % (n - i) + i;
						data.y1 = j;
						data.val = a[J][j];

						v.push_back(data);
					}

					// a massivaa update hiih
					for (int l = 0; l < v.size(); l++) {

						int xx = v[l].x1;
						int yy = v[l].y1;

						a[xx][yy] = v[l].val;
					}

					v.clear();
				}

				cnt += dist + dist2;

			}

		}
	return;
}
