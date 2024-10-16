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
    if(pow(2 , level) <= n + 1 && pow(2 , level) <= m + 1) {
        for(int i = 0; i <= n - pow(2 , level) + 1; i++) {
            for(int l = 0; l <= m - pow(2 , level); l++) {
                if(level == 1) {
                    if(a[i][l] == 1 && a[i][l + 1] == 1) {
                        a[i][l] = 2;
                    }
                } else {
                    if(a[i][l] == level && a[i][l + pow(2 , level - 1)] == level && a[i + pow(2 , level - 1)][l] == level && a[i + pow(2 , level - 1)][l + pow(2 , level - 1)]) {
                        a[i][l] = level + 1;
                    }
                }
            }
        }
        cout << level << endl;
        output(n , m , a);
        a = bigger2(n , m , a , level + 1);
        if(level > 1) {
            for(int i = 0; i <= n - pow(2 , level - 1) + 1; i++) {
                for(int l = 0; l <= m - pow(2 , level - 1); l++) {
                    if(a[i][l] == level) {
                        if(a[i][l + pow(2 , level - 2)] == 0 || a[i + pow(2 , level - 2)][l] == 0 || a[i + pow(2 , level - 2)][l + pow(2 , level - 2)] == 0) {
                            a[i][l]--;
                        } else {
                            gaia variant(l , i , (level - 1) * 3 - 1);
                            phantom.push_back(variant);
                            for(int ii = i; ii < i + pow(2 , level - 1); ii += 2) {
                                for(int ll = l; ll < l + pow(2 , level - 1); ll++) {
                                    a[ii][ll] = 0;
                                }
                            }
                        }
                    }
                }
            }
            cout << level << endl;
            output(n , m , a);
        } else {
            for(int i = 0; i < n; i++) {
                for(int l = 0; l < m; l++) {
                    if(a[i][l] == 1) {
                        gaia variant(l , i , 0);
                        phantom.push_back(variant);
                        a[i][l] = 0;
                    }
                }
            }
            cout << level << endl;
            output(n , m , a);
        }
        return a;
    } else {
        for(int i = 0; i < n; i++) {
            for(int l = 0; l < m ; l++) {
                if(a[i][l] == level) {
                    int ares = i , odin = l;
                    gaia variant(odin , ares , (level - 1) * 3 - 1);
                    phantom.push_back(variant);
                    for(int ii = ares; ii < ares + pow(2 , level - 1); ii += 2) {
                        for(int ll = odin; ll < odin + pow(2 , level - 1); ll++) {
                            a[ii][ll] = 0;
                        }
                    }
                }
            }
        }
        cout << level << endl;
        output(n , m , a);
        return a;
    }
}

vector<vector<int>> bigger3(int n , int m , vector<vector<int>> a , int level) {
    if(pow(2 , level) <= n && pow(2 , level) <= m + 1) {
        for(int i = 0; i <= n - pow(2 , level); i++) {
            for(int l = 0; l <= m - pow(2 , level) + 1; l++) {
                if(level == 1) {
                    if(a[i][l] == 1 && a[i + 1][l] == 1) {
                        a[i][l] = 2;
                    }
                } else {
                    if(a[i][l] == level && a[i][l + pow(2 , level - 1)] == level && a[i + pow(2 , level - 1)][l] == level && a[i + pow(2 , level - 1)][l + pow(2 , level - 1)]) {
                        a[i][l] = level + 1;
                    }
                }
            }
        }
        cout << level << endl;
        output(n , m , a);
        a = bigger3(n , m , a , level + 1);
        if(level > 1) {
            for(int i = 0; i <= n - pow(2 , level - 1); i++) {
                for(int l = 0; l <= m - pow(2 , level - 1) + 1; l++) {
                    if(a[i][l] == level) {
                        if(a[i][l + pow(2 , level - 1)] == level && a[i + pow(2 , level - 1)][l] == level && a[i + pow(2 , level - 1)][l + pow(2 , level - 1)]) {
                            a[i][l]--;
                        } else {
                            gaia variant(l , i , (level - 1) * 3);
                            phantom.push_back(variant);
                            for(int ii = i; ii < i + pow(2 , level - 1); ii++) {
                                for(int ll = l; ll < l + pow(2 , level - 1); ll += 2) {
                                    a[ii][ll] = 0;
                                }
                            }
                        }
                    }
                }
            }
            cout << level << endl;
            output(n , m , a);
        } else {
            for(int i = 0; i < n; i++) {
                for(int l = 0; l < m; l++) {
                    if(a[i][l] == 1) {
                        gaia variant(l , i , 0);
                        phantom.push_back(variant);
                        a[i][l] = 0;
                    }
                }
            }
            cout << level << endl;
            output(n , m , a);
        }
        return a;
    } else {
        for(int i = 0; i < n; i++) {
            for(int l = 0; l < m ; l++) {
                if(a[i][l] == level) {
                    int ares = i , odin = l;
                    gaia variant(odin , ares , (level - 1) * 3);
                    phantom.push_back(variant);
                    for(int ii = ares; ii < ares + pow(2 , level - 1); ii++) {
                        for(int ll = odin; ll < odin + pow(2 , level - 1); ll += 2) {
                            a[ii][ll] = 0;
                        }
                    }
                }
            }
        }
        cout << level << endl;
        output(n , m , a);
        return a;
    }
}


void ghost(int n , int m , vector<vector<int>> a , int direction) {
    if(direction == 0 || direction == 1) {
        a = bigger3(n , m , a , 1);
    } else {
        a = bigger2(n , m , a , 1);
    }
    for(auto fk : phantom) {
        cout << fk.x << " " << fk.y << " " << fk.dice_num << endl;
    }
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
    ghost(n , m , a , direction);
}