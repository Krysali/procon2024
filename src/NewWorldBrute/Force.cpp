#include<iostream>
#include<algorithm>
#include<set>
using namespace std;

int cnt = 0;

set<pair<int, int>> s0, s1, s2, s3;

int main(){

    int n, m;
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

    while ( true) {

        for ( int i = 0; i < n; i ++)
            for ( int j = 0; j < m; j ++) {

                int val = a[i][j];

                while ( a[i][j] != b[i][j]) {

                    auto itr = s1.lower_bound(2);
                }
            }
    }

    return 0;
}