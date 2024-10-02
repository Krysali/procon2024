#include <iostream>
#include <vector>
using namespace std;

int n, m;
int board[6][6];

void reverseTypeI(int X, int Y, int size, int s)
{
    int h, w;
    if(s == 0 || s == 1){
        w = size;
        if(s == 0) h = n - size - Y + 1;
        else h = Y;
    }else{
        h = size;
        if(s == 2) w = m - size - X + 1;
        else w = X;
    }

    return;
}

void reverseTypeII(int X, int Y, int size, int s)
{
    int otx = max(X, 0);
    int oty = max(Y, 0);
    int obx = min(X + size - 1, m - 1);
    int oby = min(Y + size - 1, n - 1);
    int w = obx - otx + 1;
    int h = oby - oty + 1;

    int chosenRowNum = h / 2;
    bool first = (h + 1) % 2;

    if(oby >= n){
        if(!first) chosenRowNum++;
        first = h % 2;
    }

    cout << "otx:" << otx << " oty:" << oty << " obx:" << obx << " oby:" << oby << " w:" << w << " h:" << h << " chosenRowNum:" << chosenRowNum << " first:" << first << '\n';

    vector<int> rem[256], chosen[128];
    
    if(s == 0){

        cout << "REM\n";
        for(int index_y = oty; index_y < n - chosenRowNum; index_y++){
            rem[index_y - oty].clear();
            cout << "index:" << index_y - oty << '\n';
            for(int index_x = otx; index_x <= obx; index_x++){  
                rem[index_y - oty].push_back(board[index_y][index_x]);
                cout << board[index_y][index_x] << ' ';
            }
            cout << '\n';
        }

        cout << "CHOSEN\n";
        for(int index_y = n - chosenRowNum ; index_y < n; index_y++){
            chosen[index_y - (n - chosenRowNum)].clear();
            cout << "index:" << index_y - (n - chosenRowNum) << '\n';
            for(int index_x = otx; index_x <= obx; index_x++){  
                chosen[index_y - (n - chosenRowNum)].push_back(board[index_y][index_x]);
                cout << board[index_y][index_x] << ' ';
            }
            cout << '\n';
        }

        int cnt = 0;
        for(int index_y = oty; index_y < (oty + chosenRowNum * 2); index_y++){
            if(first){
                for(int index_x = otx; index_x <= obx; index_x++){  
                    board[index_y][index_x] = chosen[(cnt / 2)][index_x - otx];
                }
                first = 0;
            }else{
                for(int index_x = otx; index_x <= obx; index_x++){  
                    board[index_y][index_x] = rem[(cnt / 2)][index_x - otx];
                }
                first = 1;
            }
            cnt++;
        }

        int index = 0;
        for(int index_y = (oty + chosenRowNum * 2); index_y < n; index_y++){
            for(int index_x = otx; index_x <= obx; index_x++){  
                board[index_y][index_x] = rem[index + chosenRowNum][index_x - otx];
            }
            index++;
        }
    }
    
    if(s == 1){

        cout << "CHOSEN\n";
        for(int index_y = 0; index_y < chosenRowNum; index_y++){
            chosen[index_y].clear();
            cout << "index:" << index_y << '\n';
            for(int index_x = otx; index_x <= obx; index_x++){
                chosen[index_y].push_back(board[index_y][index_x]);
                cout << board[index_y][index_x] << ' ';
            }
            cout << '\n';
        }

        cout << "REM\n";
        for(int index_y = chosenRowNum ; index_y <= oby; index_y++){
            rem[index_y - chosenRowNum].clear();
            cout << "index:" << index_y - chosenRowNum << '\n';
            for(int index_x = otx; index_x <= obx; index_x++){ 
                rem[index_y - chosenRowNum].push_back(board[index_y][index_x]);
                cout << board[index_y][index_x] << ' ';
            }
            cout << '\n';
        }

        
        for(int index_y = 0; index_y < oty; index_y++){
            for(int index_x = otx; index_x <= obx; index_x++){
                board[index_y][index_x] = rem[index_y][index_x - otx];
            }
        }

        int cnt = 0;
        for(int index_y = oty; index_y < (oty + chosenRowNum * 2); index_y++){
            if(first){
                for(int index_x = otx; index_x <= obx; index_x++){  
                    board[index_y][index_x] = chosen[(cnt / 2)][index_x - otx];
                }
                first = 0;
            }else{
                for(int index_x = otx; index_x <= obx; index_x++){  
                    board[index_y][index_x] = rem[((cnt / 2) + oty)][index_x - otx];
                }
                first = 1;
            }
            cnt++;
        }

        int index = 0;
        for(int index_y = (oty + chosenRowNum * 2); index_y <= oby; index_y++){
            for(int index_x = otx; index_x <= obx; index_x++){  
                board[index_y][index_x] = rem[index + chosenRowNum + oty][index_x - otx];
            }
            index++;
        }

    }

    return;
}

void reverseTypeIII(int X, int Y, int size, int s)
{
    int otx = max(X, 0);
    int oty = max(Y, 0);
    int obx = min(X + size - 1, m - 1);
    int oby = min(Y + size - 1, n - 1);
    int w = obx - otx + 1;
    int h = oby - oty + 1;

    int chosenColNum = w / 2;
    bool first = (w + 1) % 2;

    if(obx >= m){
        if(!first) chosenColNum++;
        first = w % 2;
    }

    cout << "otx:" << otx << " oty:" << oty << " obx:" << obx << " oby:" << oby << " w:" << w << " h:" << h << " chosenColNum:" << chosenColNum << " first:" << first << '\n';

    vector<int> rem[256], chosen[128];
    
    if(s == 2){

        cout << "REM\n";
        for(int index_x = otx; index_x < m - chosenColNum; index_x++){
            rem[index_x - otx].clear();
            cout << "index:" << index_x - otx << '\n';
            for(int index_y = oty; index_y <= oby; index_y++){
                rem[index_x - otx].push_back(board[index_y][index_x]);
                cout << board[index_y][index_x] << ' ';
            }
            cout << '\n';
        }

        cout << "CHOSEN\n";
        for(int index_x = m - chosenColNum ; index_x < m; index_x++){
            chosen[index_x - (m - chosenColNum)].clear();
            cout << "index:" << index_x - m + chosenColNum << '\n';
            for(int index_y = oty; index_y <= oby; index_y++){  
                chosen[index_x - (m - chosenColNum)].push_back(board[index_y][index_x]);
                cout << board[index_y][index_x] << ' ';
            }
            cout << '\n';
        }

        int cnt = 0;
        for(int index_x = otx; index_x < (otx + chosenColNum * 2); index_x++){
            if(first){
                for(int index_y = oty; index_y <= oby; index_y++){  
                    board[index_y][index_x] = chosen[(cnt / 2)][index_y - oty];
                }
                first = 0;
            }else{
                for(int index_y = oty; index_y <= oby; index_y++){  
                    board[index_y][index_x] = rem[(cnt / 2)][index_y - oty];
                }
                first = 1;
            }
            cnt++;
        }

        int index = 0;
        for(int index_x = (otx + chosenColNum * 2); index_x < m; index_x++){
            for(int index_y = oty; index_y <= oby; index_y++){
                board[index_y][index_x] = rem[index + chosenColNum][index_y - oty];
            }
            index++;
        }
    }

    if(s == 3){
        // CHOSEN
        for(int index_x = 0; index_x < chosenColNum; index_x++){
            chosen[index_x].clear();
            for(int index_y = oty; index_y <= oby; index_y++){
                chosen[index_x].push_back(board[index_y][index_x]);
            }
        }
        // REMAINING
        for(int index_x = chosenColNum ; index_x <= obx; index_x++){
            rem[index_x - chosenColNum].clear();
            for(int index_y = oty; index_y <= oby; index_y++){  
                rem[index_x - chosenColNum].push_back(board[index_y][index_x]);
            }
        }

        // BUILD
        for(int index_x = 0; index_x < otx; index_x++){
            for(int index_y = oty; index_y <= oby; index_y++){
                board[index_y][index_x] = rem[index_x][index_y - oty];
            }
        }

        
        int cnt = 0;
        for(int index_x = otx; index_x < (otx + chosenColNum * 2); index_x++){
            if(first){
                for(int index_y = oty; index_y <= oby; index_y++){  
                    board[index_y][index_x] = chosen[(cnt / 2)][index_y - oty];
                }
                first = 0;
            }else{
                for(int index_y = oty; index_y <= oby; index_y++){  
                    
                    int ind = (cnt / 2) + otx;
                    board[index_y][index_x] = rem[ind][index_y - oty];
                }
                first = 1;
            }
            cnt++;
        }

        int index = 0;
        for(int index_x = otx + chosenColNum * 2; index_x <= obx; index_x++){
            if(obx < otx + chosenColNum * 2) break;
            for(int index_y = oty; index_y <= oby; index_y++){
                board[index_y][index_x] = rem[index + chosenColNum + otx][index_y - oty];
            }
            index++;
        }
    }

    return;
}

void display()
{
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            if(board[i][j] < 10) cout << ' ';
            cout << board[i][j] << ' ';
        }
        cout << '\n';
    }
    cout << "\n\n\n\n";
    return;
}

int main()
{
    n = 6;
    m = 6;
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            board[i][j] = i * n + j;
        }
    }

    display();
    //reverseTypeII(2, 2, 4, 1);
    //display();
    reverseTypeIII(2, 2, 4, 2);
    display();

    return 0;
}