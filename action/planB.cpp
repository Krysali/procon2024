#include<iostream>
#include<set>
#include<vector>
#include<algorithm>
using namespace std;

struct Data {

	int x1, y1, val;
};

int cnt = 0;

set<pair<int, int>> s0, s1, s2, s3;

int main() {

	int n, m, dist, dist2;
	cin >> n >> m;

	int a[32][32], b[32][32];

	for ( int i = 0; i < n; i ++)
		for ( int j = 0; j < m; j ++) {

			cin >> a[i][j];

			if ( a[i][j] == 0) s0.insert({i, j});
			else if ( a[i][j] == 1) s1.insert({i, j});
			else if ( a[i][j] == 2) s2.insert({i, j});
			else s3.insert({i, j});
		}

	for ( int i = 0; i < n; i ++)
		for ( int j = 0; j < m; j ++)
			cin >> b[i][j];

    cout << '\n';

	if ( true) {

		for ( int i = 0; i < n; i ++)
			for ( int j = 0; j < m; j ++) {

				if ( a[i][j] != b[i][j]) {

					vector<Data> v;
					Data data;

					//b[i][j] == 0
					if ( b[i][j] == 0) {

						auto itr = s0.lower_bound({i, j});
						
						pair<int, int> pr = *itr;

						dist = pr.first - i;
						int Y = pr.second;
                        int X = pr.first;
						
						if ( Y > j) {

							// ((I - j - dist2 + (m - j))) % (m - j) + j  ene bol Y ees j hurtel dist2 oor shahad ochih indexiig olno
							dist2 = Y - j;
							for ( int I = m - 1; I >= j; I --) {

								if ( a[X][I] == 0) {

									s0.erase({X, I});
									s0.insert({X, (I - j - dist2 + (m - j)) % (m - j) + j});
								}
								else if ( a[X][I] == 1) {

									s1.erase({X, I});
									s1.insert({X, (I - j - dist2 + (m - j)) % (m - j) + j});
								}
								else if ( a[X][I] == 2) {

									s2.erase({X, I});
									s2.insert({X, (I - j - dist2 + (m - j)) % (m - j) + j});
								}
								else {

									s3.erase({X, I});
									s3.insert({X, (I - j - dist2 + (m - j)) % (m - j) + j});
								}

								data.x1 = X;
								data.y1 =(I - j - dist2 + (m - j)) % (m - j) + j;
								data.val = a[X][I];

								v.push_back(data);
							}
						}
						else {

							dist2 = j - Y;
							for ( int I = 0; I <= j; I ++) {

								if ( a[X][I] == 0) {

									s0.erase({X, I});
									s0.insert({X, (I + dist2) % (j + 1)});
								}
								else if ( a[X][I] == 1) {

									s1.erase({X, I});
									s1.insert({X, (I + dist2) % (j + 1)});
								}
								else if ( a[X][I] == 2) {

									s2.erase({X, I});
									s2.insert({X, (I + dist2) % (j + 1)});
								}
								else {

									s3.erase({X, I});
									s3.insert({X, (I + dist2) % (j + 1)});
								}

								data.x1 = X;
								data.y1 = (I + dist2) % (j + 1);
								data.val = a[X][I];

								v.push_back(data);
							}
						}

						// a massivaa update hiih
						for ( int l = 0; l < v.size(); l ++) {

							int xx = v[l].x1;
							int yy = v[l].y1;

							a[xx][yy] = v[l].val;
						}

                        for ( int I = 0; I < n; I ++) {

                            for ( int J = 0; J < m; J ++)
                                cout << a[I][J] << " ";

                            cout << '\n';
                        }

						v.clear();
						//(J - i - dist + (n - i)) % (n - i) + i ene bol n - 1 ees j hurtel dist udaa yvaad garah index
						for ( int J = n - 1; J >= i; J --) {

							if ( a[J][j] == 0) {

								s0.erase({J, j});
								s0.insert({(J - i - dist + (n - i)) % (n - i) + i, j});
							}
							else if ( a[J][j] == 1) {

								s1.erase({J, j});
								s1.insert({(J - i - dist + (n - i)) % (n - i) + i, j});
							}
							else if ( a[J][j] == 2) {

								s2.erase({J, j});
								s2.insert({(J - i - dist + (n - i)) % (n - i) + i, j});
							}
							else {

								s3.erase({J, j});
								s3.insert({(J - i - dist + (n - i)) % (n - i) + i, j});
							}

							data.x1 = (J - i - dist + (n - i)) % (n - i) + i;
							data.y1 = j;
							data.val = a[J][j];

							v.push_back(data);
						}

						// a massivaa update hiih
						for ( int l = 0; l < v.size(); l ++) {

							int xx = v[l].x1;
							int yy = v[l].y1;

							a[xx][yy] = v[l].val;
						}

						v.clear();

                        for ( int I = 0; I < n; I ++) {

                            for ( int J = 0; J < m; J ++)
                                cout << a[I][J] << " ";

                            cout << '\n';
                        }
					}
					// b[i][j] == 1
					else if ( b[i][j] == 1) {

						auto itr = s1.lower_bound({i, j});
						
						pair<int, int> pr = *itr;

						dist = pr.first - i;
						int Y = pr.second;
                        int X = pr.first;
						
						if ( Y > j) {

							// ((I - j - dist2 + (m - j))) % (m - j) + j  ene bol Y ees j hurtel dist2 oor shahad ochih indexiig olno
							dist2 = Y - j;
							for ( int I = m - 1; I >= j; I --) {

								if ( a[X][I] == 0) {

									s0.erase({X, I});
									s0.insert({X, (I - j - dist2 + (m - j)) % (m - j) + j});
								}
								else if ( a[X][I] == 1) {

									s1.erase({X, I});
									s1.insert({X, (I - j - dist2 + (m - j)) % (m - j) + j});
								}
								else if ( a[X][I] == 2) {

									s2.erase({X, I});
									s2.insert({X, (I - j - dist2 + (m - j)) % (m - j) + j});
								}
								else {

									s3.erase({X, I});
									s3.insert({X, (I - j - dist2 + (m - j)) % (m - j) + j});
								}
								data.x1 = X;
								data.y1 =(I - j - dist2 + (m - j)) % (m - j) + j;
								data.val = a[X][I];

								v.push_back(data);
							}
						}
						else {

							dist2 = j - Y;
							for ( int I = 0; I <= j; I ++) {

								if ( a[X][I] == 0) {

									s0.erase({X, I});
									s0.insert({X, (I + dist2) % (j + 1)});
								}
								else if ( a[X][I] == 1) {

									s1.erase({X, I});
									s1.insert({X, (I + dist2) % (j + 1)});
								}
								else if ( a[X][I] == 2) {

									s2.erase({X, I});
									s2.insert({X, (I + dist2) % (j + 1)});
								}
								else {

									s3.erase({X, I});
									s3.insert({X, (I + dist2) % (j + 1)});
								}
								data.x1 = X;
								data.y1 = (I + dist2) % (j + 1);
								data.val = a[X][I];

								v.push_back(data);
							}
						}
						for ( int l = 0; l < v.size(); l ++) {

							int xx = v[l].x1;
							int yy = v[l].y1;

							a[xx][yy] = v[l].val;
						}

                        for ( int I = 0; I < n; I ++) {

                            for ( int J = 0; J < m; J ++)
                                cout << a[I][J] << " ";

                            cout << '\n';
                        }

						v.clear();

						//(J - i - dist + (n - i)) % (n - i) + i ene bol n - 1 ees j hurtel dist udaa yvaad garah index
						for ( int J = n - 1; J >= i; J --) {

							if ( a[J][j] == 0) {

								s0.erase({J, j});
								s0.insert({(J - i - dist + (n - i)) % (n - i) + i, j});
							}
							else if ( a[J][j] == 1) {

								s1.erase({J, j});
								s1.insert({(J - i - dist + (n - i)) % (n - i) + i, j});
							}
							else if ( a[J][j] == 2) {

								s2.erase({J, j});
								s2.insert({(J - i - dist + (n - i)) % (n - i) + i, j});
							}
							else {

								s3.erase({J, j});
								s3.insert({(J - i - dist + (n - i)) % (n - i) + i, j});
							}
							data.x1 = (J - i - dist + (n - i)) % (n - i) + i;
							data.y1 = j;
							data.val = a[J][j];

							v.push_back(data);
						}
						for ( int l = 0; l < v.size(); l ++) {

							int xx = v[l].x1;
							int yy = v[l].y1;

							a[xx][yy] = v[l].val;
						}

                        for ( int I = 0; I < n; I ++) {

                            for ( int J = 0; J < m; J ++)
                                cout << a[I][J] << " ";

                            cout << '\n';
                        }
                        
						v.clear();
					}
					// b[i][j] == 2
					else if ( b[i][j] == 2) {

						auto itr = s2.lower_bound({i, j});
						
						pair<int, int> pr = *itr;

						dist = pr.first - i;
						int Y = pr.second;
                        int X = pr.first;
						
						if ( Y > j) {

							// ((I - j - dist2 + (m - j))) % (m - j) + j  ene bol Y ees j hurtel dist2 oor shahad ochih indexiig olno
							dist2 = Y - j;
							for ( int I = m - 1; I >= j; I --) {

								if ( a[X][I] == 0) {

									s0.erase({X, I});
									s0.insert({X, (I - j - dist2 + (m - j)) % (m - j) + j});
								}
								else if  ( a[X][I] == 1) { 

									s1.erase({X, I});
									s1.insert({X, (I - j - dist2 + (m - j)) % (m - j) + j});
								}
								else if ( a[X][I] == 2) {

									s2.erase({X, I});
									s2.insert({X, (I - j - dist2 + (m - j)) % (m - j) + j});
								}
								else {

									s3.erase({X, I});
									s3.insert({X, (I - j - dist2 + (m - j)) % (m - j) + j});
								}
								data.x1 = X;
								data.y1 =(I - j - dist2 + (m - j)) % (m - j) + j;
								data.val = a[X][I];

								v.push_back(data);
							}
						}
						else {

							dist2 = j - Y;
							for ( int I = 0; I <= j; I ++) {

								if ( a[X][I] == 0) {

									s0.erase({X, I});
									s0.insert({X, (I + dist2) % (j + 1)});
								}
								else if ( a[X][I] == 1) {

									s1.erase({X, I});
									s1.insert({X, (I + dist2) % (j + 1)});
								}
								else if ( a[X][I] == 2) {

									s2.erase({X, I});
									s2.insert({X, (I + dist2) % (j + 1)});
								}
								else {

									s3.erase({X, I});
									s3.insert({X, (I + dist2) % (j + 1)});
								}
								data.x1 = X;
								data.y1 = (I + dist2) % (j + 1);
								data.val = a[X][I];

								v.push_back(data);
							}
						}
						for ( int l = 0; l < v.size(); l ++) {

							int xx = v[l].x1;
							int yy = v[l].y1;

							a[xx][yy] = v[l].val;
						}

                        for ( int I = 0; I < n; I ++) {

                            for ( int J = 0; J < m; J ++)
                                cout << a[I][J] << " ";

                            cout << '\n';
                        }

						v.clear();

						//(J - i - dist + (n - i)) % (n - i) + i ene bol n - 1 ees j hurtel dist udaa yvaad garah index
						for ( int J = n - 1; J >= i; J --) {

							if ( a[J][j] == 0) {

								s0.erase({J, j});
								s0.insert({(J - i - dist + (n - i)) % (n - i) + i, j});
							}
							else if ( a[J][j] == 1) {

								s1.erase({J, j});
								s1.insert({(J - i - dist + (n - i)) % (n - i) + i, j});
							}
							else if ( a[J][j] == 2) {

								s2.erase({J, j});
								s2.insert({(J - i - dist + (n - i)) % (n - i) + i, j});
							}
							else {

								s3.erase({J, j});
								s3.insert({(J - i - dist + (n - i)) % (n - i) + i, j});
							}
							data.x1 = (J - i - dist + (n - i)) % (n - i) + i;
							data.y1 = j;
							data.val = a[J][j];

							v.push_back(data);
						}
						for ( int l = 0; l < v.size(); l ++) {

							int xx = v[l].x1;
							int yy = v[l].y1;

							a[xx][yy] = v[l].val;
						}

                        for ( int I = 0; I < n; I ++) {

                            for ( int J = 0; J < m; J ++)
                                cout << a[I][J] << " ";

                            cout << '\n';
                        }

						v.clear();
					}
					//b[i][j] == 3
					else {

						auto itr = s3.lower_bound({i, j});
						
						pair<int, int> pr = *itr;

						dist = pr.first - i;
						int Y = pr.second;
                        int X = pr.first;
						
						if ( Y > j) {

							// ((I - j - dist2 + (m - j))) % (m - j) + j  ene bol Y ees j hurtel dist2 oor shahad ochih indexiig olno
							dist2 = Y - j;
							for ( int I = m - 1; I >= j; I --) {

								if ( a[X][I] == 0) {

									s0.erase({X, I});
									s0.insert({X, (I - j - dist2 + (m - j)) % (m - j) + j});
								}
								else if ( a[X][I] == 1) {

									s1.erase({X, I});
									s1.insert({X, (I - j - dist2 + (m - j)) % (m - j) + j});
								}
								else if ( a[X][I] == 2) {

									s2.erase({X, I});
									s2.insert({X, (I - j - dist2 + (m - j)) % (m - j) + j});
								}
								else {

									s3.erase({X, I});
									s3.insert({X, (I - j - dist2 + (m - j)) % (m - j) + j});
								}
								data.x1 = X;
								data.y1 =(I - j - dist2 + (m - j)) % (m - j) + j;
								data.val = a[X][I];

								v.push_back(data);
							}
						}
						else {

							dist2 = j - Y;
							for ( int I = 0; I <= j; I ++) {

								if ( a[X][I] == 0) {

									s0.erase({X, I});
									s0.insert({X, (I + dist2) % (j + 1)});
								}
								else if ( a[X][I] == 1) {

									s1.erase({X, I});
									s1.insert({X, (I + dist2) % (j + 1)});
								}
								else if ( a[X][I] == 2) {

									s2.erase({X, I});
									s2.insert({X, (I + dist2) % (j + 1)});
								}
								else {

									s3.erase({X, I});
									s3.insert({X, (I + dist2) % (j + 1)});
								}
								data.x1 = X;
								data.y1 = (I + dist2) % (j + 1);
								data.val = a[X][I];

								v.push_back(data);
							}
						}
						for ( int l = 0; l < v.size(); l ++) {

							int xx = v[l].x1;
							int yy = v[l].y1;

							a[xx][yy] = v[l].val;
						}

                        for ( int I = 0; I < n; I ++) {

                            for ( int J = 0; J < m; J ++)
                                cout << a[I][J] << " ";

                            cout << '\n';
                        }

						v.clear();

						//(J - i - dist + (n - i)) % (n - i) + i ene bol n - 1 ees j hurtel dist udaa yvaad garah index
						for ( int J = n - 1; J >= i; J --) {

							if ( a[J][j] == 0) {

								s0.erase({J, j});
								s0.insert({(J - i - dist + (n - i)) % (n - i) + i, j});
							}
							else if ( a[J][j] == 1) {

								s1.erase({J, j});
								s1.insert({(J - i - dist + (n - i)) % (n - i) + i, j});
							}
							else if ( a[J][j] == 2) {

								s2.erase({J, j});
								s2.insert({(J - i - dist + (n - i)) % (n - i) + i, j});
							}
							else {

								s3.erase({J, j});
								s3.insert({(J - i - dist + (n - i)) % (n - i) + i, j});
							}
							data.x1 = (J - i - dist + (n - i)) % (n - i) + i;
							data.y1 = j;
							data.val = a[J][j];

							v.push_back(data);
						}
						for ( int l = 0; l < v.size(); l ++) {

							int xx = v[l].x1;
							int yy = v[l].y1;

							a[xx][yy] = v[l].val;
						}

                        for ( int I = 0; I < n; I ++) {

                            for ( int J = 0; J < m; J ++)
                                cout << a[I][J] << " ";

                            cout << '\n';
                        }

						v.clear();
					}

                    cout << '\n';

					cnt += dist + dist2;
					
				}
				
			}
		
	}
    
	/*
	for ( int i = 0; i < n; i ++) {

		for ( int j = 0; j < m; j ++) {

			cout << a[i][j] << " ";
		}

		cout << '\n';
	}*/

    cout << cnt << '\n';
    
	return 0;
}