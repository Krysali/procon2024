#include <iostream>
#include <vector>
#include <cmath>
using namespace std;
class gaia {
public:
    int x , y , dice_num;
    gaia(int x , int y , int dice_num) {
        this -> x = x;
        this -> y = y;
        this -> dice_num = dice_num;
    }
};

vector<gaia> phantom;

void output(int n , int m , vector<vector<int>> a) {
    for(int i = 0; i < n; i++) {
        for(int l = 0; l < m; l++) {
            cout << a[i][l] << " ";
        }
        cout << endl;
    }
}

vector<vector<int>> bigger2(int n , int m , vector<vector<int>> a , int level) {
    if(pow(2 , level - 1) <= n + 1 && pow(2 , level - 1) <= m + 1) {
        for(int i = 0; i < n - level + 2; i++) {
            for(int l = 0; l < m - level + 2; l++) {
                if(a[i][l] >= level * -1 && a[i][l] <= level * -1 + 1 && a[i + pow(2 , level - 2)][l] >= level * -1 && a[i + pow(2 , level - 2)][l] <= level * -1 + 1 && a[i][l + pow(2 , level - 2)] >= level * -1 && a[i][l + pow(2 , level - 2)] <= level * -1 + 1 && a[i + pow(2 , level - 2)][l + pow(2 , level - 2)] >= level * -1 && a[i + pow(2 , level - 2)][l + pow(2 , level - 2)] <= level * -1 + 1) {
                    a[i][l] = level * -1;
                }
                if()
            }
        }
        a = bigger(n , m , a , level + 1);
        if(level > 2) {
            for(int i = 0; i < n - level + 1; i++) {
                for(int l = 0; l < m - level + 1; l++) {
                    if(a[i][l] == (level - 1) * -1) {
                        if(a[i + pow(2 , level - 3)][l] != 0 || a[i][l + pow(2 , level - 3)] != 0  || a[i + pow(2 , level - 3)][l + pow(2 , level - 3)] != 0) {
                            a[i][l]++;
                        } else {
                            gaia variant(i , l , (level - 2) * 3 - 2);
                            phantom.push_back(variant);
                            for(int ii = i; ii < i + pow(2 , level - 3) - 1; ii++) {
                                for(int ll = l; ll < l + pow(2 , level - 3) - 1; ll++) {
                                    a[ii][ll] = 0;
                                }
                            }
                        }
                    }
                }
            }
        } else {
            for(int i = 0; i < n; i++) {
                for(int l = 0; l < m; l++) {
                    if(a[i][l] == -1) {
                        if(a[i][l])
                        gaia variant(l , i , 0);
                        phantom.push_back(variant);
                        a[i][l] = 0;
                    }
                }
            }
        }
        return a;
    } else {
        for(int i = 0; i < n; i++) {
            for(int l = 0; l <m; l++) {
                if(a[i][l] == -1 * (level - 1) ) {
                    int ares = i , odin = l;
                    gaia variant(odin , ares , (level - 2) * 3 - 2);
                    phantom.push_back(variant);
                    for(int ii = ares; ii <= ares + pow(2 , level - 2) - 1; ii++) {
                        for(int ll = odin; ll <= odin + pow(2 , level - 2) - 1; ll++) {
                            a[ii][ll] = 0;
                        }
                    }
                }
            }
        }
        return a;
    }
}

vector<vector<int>> bigger3(int n , int m , vector<vector<int>> a , int level) {
    if(pow(2 , level - 1) <= n + 1 && pow(2 , level - 1) <= m + 1) {
        for(int i = 0; i < n - level + 2; i++) {
            for(int l = 0; l < m - level + 2; l++) {
                if(a[i][l] >= level * -1 && a[i][l] <= level * -1 + 1 && a[i + pow(2 , level - 2)][l] >= level * -1 && a[i + pow(2 , level - 2)][l] <= level * -1 + 1 && a[i][l + pow(2 , level - 2)] >= level * -1 && a[i][l + pow(2 , level - 2)] <= level * -1 + 1 && a[i + pow(2 , level - 2)][l + pow(2 , level - 2)] >= level * -1 && a[i + pow(2 , level - 2)][l + pow(2 , level - 2)] <= level * -1 + 1) {
                    a[i][l] = level * -1;
                }
                if()
            }
        }
        a = bigger(n , m , a , level + 1);
        if(level > 2) {
            for(int i = 0; i < n - level + 1; i++) {
                for(int l = 0; l < m - level + 1; l++) {
                    if(a[i][l] == (level - 1) * -1) {
                        if(a[i + pow(2 , level - 3)][l] != 0 || a[i][l + pow(2 , level - 3)] != 0  || a[i + pow(2 , level - 3)][l + pow(2 , level - 3)] != 0) {
                            a[i][l]++;
                        } else {
                            gaia variant(i , l , (level - 2) * 3 - 2);
                            phantom.push_back(variant);
                            for(int ii = i; ii < i + pow(2 , level - 3) - 1; ii++) {
                                for(int ll = l; ll < l + pow(2 , level - 3) - 1; ll++) {
                                    a[ii][ll] = 0;
                                }
                            }
                        }
                    }
                }
            }
        } else {
            for(int i = 0; i < n; i++) {
                for(int l = 0; l < m; l++) {
                    if(a[i][l] == -1) {
                        if(a[i][l])
                        gaia variant(l , i , 0);
                        phantom.push_back(variant);
                        a[i][l] = 0;
                    }
                }
            }
        }
        return a;
    } else {
        for(int i = 0; i < n; i++) {
            for(int l = 0; l <m; l++) {
                if(a[i][l] == -1 * (level - 1) ) {
                    int ares = i , odin = l;
                    gaia variant(odin , ares , (level - 2) * 3 - 2);
                    phantom.push_back(variant);
                    for(int ii = ares; ii <= ares + pow(2 , level - 2) - 1; ii++) {
                        for(int ll = odin; ll <= odin + pow(2 , level - 2) - 1; ll++) {
                            a[ii][ll] = 0;
                        }
                    }
                }
            }
        }
        return a;
    }
}


vector<vector<int>> bigger(int n , int m , vector<vector<int>> a , int level , int direction) {
    if(pow(2 , level - 1) <= n && pow(2 , level - 1) <= m) {
        for(int i = 0; i < n - level + 1; i++) {
            for(int l = 0; l < m - level + 1; l++) {
                if(a[i][l] >= level * -1 && a[i][l] <= level * -1 + 1 && a[i + pow(2 , level - 2)][l] >= level * -1 && a[i + pow(2 , level - 2)][l] <= level * -1 + 1 && a[i][l + pow(2 , level - 2)] >= level * -1 && a[i][l + pow(2 , level - 2)] <= level * -1 + 1 && a[i + pow(2 , level - 2)][l + pow(2 , level - 2)] >= level * -1 && a[i + pow(2 , level - 2)][l + pow(2 , level - 2)] <= level * -1 + 1) {
                    a[i][l] = level * -1;
                }
            }
        }
        a = bigger(n , m , a , level + 1);
        if(level > 2) {
            for(int i = 0; i < n - level + 1; i++) {
                for(int l = 0; l < m - level + 1; l++) {
                    if(a[i][l] == (level - 1) * -1) {
                        if(a[i + pow(2 , level - 3)][l] != 0 || a[i][l + pow(2 , level - 3)] != 0  || a[i + pow(2 , level - 3)][l + pow(2 , level - 3)] != 0) {
                            a[i][l]++;
                        } else {
                            gaia variant(i , l , (level - 2) * 3 - 2);
                            phantom.push_back(variant);
                            for(int ii = i; ii < i + pow(2 , level - 3) - 1; ii++) {
                                for(int ll = l; ll < l + pow(2 , level - 3) - 1; ll++) {
                                    a[ii][ll] = 0;
                                }
                            }
                        }
                    }
                }
            }
        } else {
            if(direction == 0 || direction == 1) {
                a = bigger3(n , m , a , 2);
            } else {
                a = bigger2(n , m , a , 2);
            }
        }
        return a;
    } else {
        for(int i = 0; i < n; i++) {
            for(int l = 0; l <m; l++) {
                if(a[i][l] == -1 * (level - 1) ) {
                    int ares = i , odin = l;
                    gaia variant(odin , ares , (level - 2) * 3 - 2);
                    phantom.push_back(variant);
                    for(int ii = ares; ii <= ares + pow(2 , level - 2) - 1; ii++) {
                        for(int ll = odin; ll <= odin + pow(2 , level - 2) - 1; ll++) {
                            a[ii][ll] = 0;
                        }
                    }
                }
            }
        }
        return a;
    }
}

void ghost(int n , int m , vector<vector<int>> a , int direction) {
    for(int i = 0; i < n; i++) {
        for(int l = 0; l < m; l++) {
            if(a[i][l] == 1) {
                a[i][l] = -1;
            }
        }
    }
    a = bigger(n , m , a , 2 , direction);
    output(n, m , a);
    for(auto fk : phantom) {
        cout << fk.x << " " << fk.y << " " << fk.dice_num << endl;
    }
}

void vandal(int n , int m , vector<vector<int>> a , int direction) {
    ghost(n , m , a , direction);
}

int main() {
    int n , m , direction;
    vector<vector<int>> a;
    cin >> n >> m;
    for(int i = 0; i < n; i++) {
        vector<int> row;
        for(int j = 0; j < m; j++) {
            int f;
            cin >> f;
            row.push_back(f);
        }
        a.push_back(row);
    }
    cin >> direction;
    vandal(n , m , a , direction);
}