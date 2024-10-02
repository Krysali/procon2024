#include <types.h>
#include <ops.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <map>
using namespace std;

int amx , amy , bogsx , bogsy , boov, tomsog, mxSidee, urt, diceNum;

class Action{
    public:
        int x, y, diceNum, dir;
        Action(int x, int y, int diceNum, int dir){
            this -> x = x;
            this -> y = y;
            this -> diceNum = diceNum;
            this -> dir = dir;
        }
};

vector<Action> actions;
map<vector<int>, int> check;

int mearest(int x)
{
    int res = 1;
    while(x > res) res *= 2;
    return res;
}

bool isPowerOfTwo(int x)
{
    if(x == 1) return 1;
    if(x % 2 == 1) return 0;
    return isPowerOfTwo(x / 2);
}

int main() {
    int n, m , xes;
    cin >> n >> m;
     if(n > m) swap(n, m);
    //State finder
    for(int amx = 0; amx < m ; amx++) {
        for(int amy = 0; amy < n; amy++) {
            for(int bogsx = amx; bogsx < m ; bogsx++) {
                for(int bogsy = amy; bogsy < n; bogsy++) {
                    boov = bogsx - amx;
                    tomsog = bogsy - amy;
                    mxSidee = max(boov , tomsog);
                    urt = mearest(mxSidee);
                    // dice uudiig chihne
                    diceNum = (log2(urt)*3);
                    for(int dir = 0; dir < 4; dir++) {
                        if(urt == 1) {
                            Action action(amx , amy , 0 , dir);
                            actions.push_back(action);
                        }
                        for(int dick = 0; dick < 3; dick++) {
                            Action action(amx , amy , diceNum - dick +1 , dir);
                            actions.push_back(action);
                        }
                    }
                }
            }
        }
    }
    // CHECK
    if(1){
        Board board;
        board.height = n;
        board.width = m;
        for(int i = 0; i < n; i++){
            vector<int> row;
            for(int j = 0; j < m; j++){
                row.push_back(i * n + j + 1);
            }
            board.pieces.push_back(row);
        }


        int tooluur = 0 , number = 0;
        for(auto a : actions){
            //cout << "x:" << a.x << " y:" << a.y << " diceType:" << a.diceType << " dir:" << a.dir << '\n';

            GameState nextState;
            nextState.board = board;
            nextState.dies = GenerateFixedDies();
            apply_die(nextState, a.diceNum, a.x, a.y, a.dir);
            //display_game_state(nextState);

            vector<int> llr;
            for(int i = 0; i < n; i++){
                for(int j = 0; j < m; j++){
                    int too = nextState.board.pieces[i][j];
                    llr.push_back(too);
                }
            }
            if(check[llr] == 0){
                check[llr]++;
                tooluur++;
            }else{
               std::cout << "x; " << a.x << " y; " << a.y << " dice; " << a.diceNum << " dir; " << a.dir << endl;
                display_game_state(nextState);
            }
            number++;
            
        }
        std::cout<<number<<" ";
        std::cout << tooluur;
    }
    return 0;
}
