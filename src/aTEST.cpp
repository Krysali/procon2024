#include <types.h>
#include <ops.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <unordered_map>
using namespace std;

int topX, topY, botX, botY, width, height, mxSide, sz, diceNum;

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
unordered_map<string, int> check;

int nearest(int x)
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

int main()
{
    // n = height, m = width
    int n, m;
    cin >> n >> m;

    // urgelj m ni urt tal ni baina 
    if(n > m) swap(n, m);

    // urgelj inside the board
    if(1){
        for(topY = 1; topY < n - 1; topY++){
            for(topX = 1; topX < m - 1; topX++){
                for(sz = 1; sz <= min(n - topY - 1, m - topX - 1); sz *= 2){
                    // 3rd diceNum of this size
                    diceNum = log2(sz) * 3;
                    for(int s = 0; s < 4; s++){
                        if(sz == 1){
                            Action action(topX, topY, diceNum, s);
                            actions.push_back(action);
                        }else{
                            for(int i = 0; i < 3; i++){
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
    if(1){
        topX = 0;
        topY = 0;

        for(botY = 0; botY < n - 1; botY++){
            for(botX = 0; botX < m - 1; botX++){
                width = botX - topX + 1;
                height = botY - topY + 1;
                mxSide = max(width, height);
                sz = nearest(mxSide);
                
                // 3rd diceNum of this size
                diceNum = log2(sz) * 3;

                if(sz == 1) continue;

                // TYPE I
                for(int i = 0; i < 4; i++){
                    Action action(botX - sz + 1, botY - sz + 1, diceNum - 2, i);
                    actions.push_back(action);
                }
                
    
                // TYPE II
                for(int i = 0; i < 4; i++){
                    Action action(botX - sz + 1, botY - sz + 1, diceNum - 1, i);
                    actions.push_back(action);
                }

                // TYPE III
                for(int i = 0; i < 4; i++){
                    Action action(botX - sz + 1, botY - sz + 1, diceNum, i);
                    actions.push_back(action);
                }
            }
        }
    }

    // topright corner contained actions
    if(1){
        topY = 0;
        botX = m - 1;

        for(botY = 0; botY < n - 1; botY++){
            for(topX = 1; topX <= m - 1; topX++){   

                height = botY - topY + 1;
                width = botX - topX + 1;
                mxSide = max(height, width);
                sz = nearest(mxSide);

                // 3th type dice of this size
                diceNum = (log2(sz) * 3);

                
                if(sz == 1) continue;

                // TYPE I
                for(int i = 0; i < 4; i++){
                    Action action(topX, botY - sz + 1, diceNum - 2, i);
                    actions.push_back(action);
                }

                // TYPE II
                for(int i = 0; i < 4; i++){
                    Action action(topX, botY - sz + 1, (diceNum - 1), i);
                    actions.push_back(action);  
                }

                // TYPE III
                for(int i = 0; i < 4; i++){
                    Action action(topX, botY - sz + 1, (diceNum), i);
                    actions.push_back(action);
                }

            }
        }


    }

    // bottomleft corner contained actions
    if(1){
        topX = 0;
        botY = n - 1;

        for(topY = 1; topY < n; topY++){
            for(botX = 0; botX < m - 1; botX++){
                
                height = botY - topY + 1;
                width = botX - topX + 1;
                mxSide = max(height, width);
                sz = nearest(mxSide);

                // 3th type dice of this size
                diceNum = (log2(sz) * 3);

                
                if(sz == 1) continue;

                // TYPE I
                for(int i = 0; i < 4; i++){
                    Action action(botX - sz + 1, topY, diceNum - 2, i);
                    actions.push_back(action);
                }

                // TYPE II
                for(int i = 0; i < 4; i++){
                    Action action(botX - sz + 1, topY, (diceNum - 1), i);
                    actions.push_back(action);  
                }

                // TYPE III 
                for(int i = 0; i < 4; i++){
                    Action action(botX - sz + 1, topY, diceNum, i);
                    actions.push_back(action);  
                } 
                
            }
        }
    }

    // bottomright corner contained actions
    if(1){
        botX = m - 1;
        botY = n - 1;

        for(topY = 1; topY < n; topY++){
            for(topX = 1; topX < m; topX++){

                height = botY - topY + 1;
                width = botX - topX + 1;
                mxSide = max(height, width);
                sz = nearest(mxSide);

                // 3th type dice of this size
                diceNum = (log2(sz) * 3);

                
                if(sz == 1) continue;

                // TYPE I
                for(int i = 0; i < 4; i++){
                    Action action(topX, topY, (diceNum - 2), i);
                    actions.push_back(action);
                }

                // TYPE II
                for(int i = 0; i < 4; i++){
                    Action action(topX, topY, (diceNum - 1), i);
                    actions.push_back(action);  
                }
                

                // TYPE III
                for(int i = 0; i < 4; i++){
                    Action action(topX, topY, diceNum, i);
                    actions.push_back(action);  
                }
                
            
            }
        }
    }

    // pairs of corners

    if(1){

        // from above
        if(1){
            topX = 0;
            topY = 0;
            botX = m - 1;

            for(botY = 0; botY < n - 1; botY++){
                width = m;
                height = botY - topY + 1;
                mxSide = max(width, height);
                sz = nearest(mxSide);
                // 3th type dice of this size
                diceNum = (log2(sz) * 3);

                 // TYPE I
                for(int i = 0; i < 4; i++){
                    Action action(botX - sz + 1, botY - sz + 1, diceNum - 2, i);
                    actions.push_back(action);
                }

                // TYPE II
                for(int i = 0; i < 4; i++){
                    Action action(botX - sz + 1, botY - sz + 1, diceNum - 1, i);
                    actions.push_back(action);
                }
                

                // TYPE III
                if(m % 2 == 0){
                    for(int i = 0; i < 4; i++){
                        Action action1(botX - sz + 1, botY - sz + 1, diceNum, i);
                        actions.push_back(action1);
                    }
                    
                }else{
                    for(int i = 0; i < 4; i++){
                        Action action1(botX + 1 - sz + 1, botY - sz + 1, diceNum, i);
                        actions.push_back(action1);
                    }
                }

            }
        
        }

        // from left
        if(1){
            topX = 0;
            topY = 0;
            botY = n - 1;
            for(botX = 0; botX < m - 1; botX++){
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
                if(n % 2 == 0){
                    for(int i = 0; i < 4; i++){
                        Action action(botX - sz + 1, botY - sz + 1, diceNum - 1, i);
                        actions.push_back(action);
                    }   
                }else{
                    for(int i = 0 ; i < 4;i++){
                        Action action(botX - sz + 1, botY + 1 - sz + 1, diceNum - 1, i);
                        actions.push_back(action);
                    }
                }

                // TYPE III
                for(int i = 0; i < 4; i++){
                    Action action(botX - sz + 1, botY - sz + 1, diceNum, i);
                    actions.push_back(action);
                }
                
            }
        }

        // from right
        if(1){
            topX = m - 1;
            topY = 0;
            botY = n - 1;
            for(botX = m - 1; botX > 0; botX--){
                width = topX - botX + 1;
                height = n;
                mxSide = max(width, height);
                sz = nearest(mxSide);
                // 3th type dice of this size
                diceNum = (log2(sz) * 3);

                // TYPE I
                Action action(botX, botY - sz + 1, diceNum - 2, 3);
                actions.push_back(action);

                // TYPE II
                for(int i = 0; i < 4; i++){
                    Action action(botX, botY - sz + 1, diceNum - 1, i);
                    actions.push_back(action);
                }

                // TYPE III
                for(int i = 0; i < 4; i++){
                    Action action(botX, botY - sz + 1, diceNum, i);
                    actions.push_back(action);
                }
                
            }

            

        }
    
        // from bottom
        if(1){
            topX = 0;
            botX = m - 1;
            botY = n - 1;
            for(topY = n - 1; topY > 0; topY--){
                width = m;
                height = topY - botY + 1;
                mxSide = max(width, height);
                sz = nearest(mxSide);
                // 3th type dice of this size
                diceNum = (log2(sz) * 3);

                Action action(topX, topY, diceNum - 2, 1);
                actions.push_back(action);

                // TYPE II
                for(int i = 0; i < 4; i++){
                    Action action(topX, topY, diceNum - 1, i);
                    actions.push_back(action);
                }

                // TYPE III???
                for(int i = 0; i < 4; i++){
                    Action action(topX, topY, diceNum, i);
                    actions.push_back(action);
                }

            }
        }
    
    }

    // edge 

    // top
    if(1){
        topY = 0;
        for(topX = 1; topX < m - 1; topX++){
            for(width = 1; width < (m - 1 - topX); width *= 2){
                for(height = 1; height <= min(width, n - 1); height++){
                    sz = width;

                    // 3th type dice of this size
                    diceNum = (log2(sz) * 3);

                    
                    if(sz == 1) continue;

                    // TYPE I
                    for(int i = 0; i < 4; i++){
                        Action action(topX, height - width, (diceNum - 2), i);
                        actions.push_back(action);
                    }

                    // TYPE II
                    for(int i = 0; i < 4; i++){
                        Action action(topX, height - width, (diceNum - 1), i);
                        actions.push_back(action);  
                    }
                    

                    // TYPE III
                    for(int i = 0; i < 4; i++){
                        Action action(topX, height - width, diceNum, i);
                        actions.push_back(action);  
                    }
                }
            }
        }
    }
    
    // bottom
    if(1){
        topY = n - 1;
        for(topX = 1; topX < m - 1; topX++){
            for(width = 1; width <= (m - 1 - topX); width *= 2){
                for(height = 1; height <= min(width, n - 1); height++){

                    int oh = n - height;
                    int ow = width;

                    sz = width;

                    // 3th type dice of this size
                    diceNum = (log2(sz) * 3);

                    
                    if(sz == 1) continue;

                    // TYPE I
                    for(int i = 0; i < 4; i++){
                        Action action(topX, topY - height + 1, (diceNum - 2), i);
                        actions.push_back(action);
                    }
                    
                    // TYPE II
                    for(int i = 0; i < 4; i++){
                        Action action(topX, topY - height + 1, (diceNum - 1), i);
                        actions.push_back(action);  
                    }
                    
                    
                    // TYPE III
                    for(int i = 0; i < 4; i++){
                        Action action(topX, topY - height + 1, diceNum, i);
                        actions.push_back(action);  
                    }
                }
            }
        }
    }

    // left
    if(1){
        topX = 0;
        for(topY = 1; topY < n - 1; topY++){
            for(height = 1; height <= (n - 1 - topY); height *= 2){
                for(width = 1; width <= height; width++){
                    sz = height;

                    // 3th type dice of this size
                    diceNum = (log2(sz) * 3);

                    
                    if(sz == 1) continue;
                    // TYPE I
                    for(int i = 0; i < 4; i++){
                        Action action(width - height, topY, (diceNum - 2), i);
                        actions.push_back(action);
                    }

                    // TYPE III
                    for(int i = 0; i < 4; i++){
                        Action action(width - height, topY, diceNum, i);
                        actions.push_back(action);  
                    }
                    

                    // TYPE II
                    for(int i = 0; i < 4; i++){
                        Action action(width - height, topY, (diceNum - 1), i);
                        actions.push_back(action);  
                    }

                }
            }
        }
    }

    // right
    if(1){
        topX = m - 1;
        for(topY = 1; topY < n - 1; topY++){
            for(height = 1; height <= (n - 1 - topY); height *= 2){
                for(width = 1; width <= height; width++){
                    sz = height;

                    int oh = height;
                    int ow = m - width;


                    // 3th type dice of this size
                    diceNum = (log2(sz) * 3);

                    
                    if(sz == 1) continue;

                    // TYPE I
                    for(int i = 0; i < 4; i++){
                        Action action(topX - width + 1, topY, (diceNum - 2), i);
                        actions.push_back(action);
                    }

                    // TYPE III
                    for(int i = 0; i < 4; i++){
                        Action action(topX - width + 1, topY, diceNum, i);
                        actions.push_back(action);  
                    }
                    

                    
                    // TYPE II
                    for(int i = 0; i < 4; i++){
                        Action action(topX - width + 1, topY, diceNum - 1, i);
                        actions.push_back(action);  
                    }
                    
                }
            }
        }
    }

    // pairs of edges
    if(isPowerOfTwo(n)){
        if(n + 1 < m){
            topY = 0;
            for(topX = 1; topX < m - n; topX++){
                sz = n;
                // 3th type dice of this size
                diceNum = (log2(sz) * 3);

                // TYPE I
                for(int i = 0; i < 4; i++){
                    Action action(topX, topY, diceNum - 2, i);
                    actions.push_back(action);
                }

                // TYPE II
                for(int i = 0; i < 4; i++){
                    Action action(topX, topY, diceNum - 1, i);
                    actions.push_back(action);
                }

                // TYPE III
                for(int i = 0; i < 4; i++){
                    Action action(topX, topY, diceNum, i);
                    actions.push_back(action);
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
            //cout << "x:" << a.x << " y:" << a.y << " diceType:" << a.diceNum << " dir:" << a.dir << '\n';

            GameState nextState;
            nextState.board = board;
            nextState.dies = GenerateFixedDies();
            apply_die(nextState, a.diceNum, a.x, a.y, a.dir);
            //display_game_state(nextState);

            string llr;
            for(int i = 0; i < n; i++){
                for(int j = 0; j < m; j++){
                    int too = nextState.board.pieces[i][j];
                    if(too < 10){
                        char c = nextState.board.pieces[i][j] + '0'; 
                        llr += c;
                        continue;
                    }
                    while(too > 0){
                        int k = too % 10;
                        too /= 10;
                        char c = k + '0'; 
                        llr += c;
                    }
                }
            }
            
            if(check[llr] == 0){
                check[llr]++;
                tooluur++;
            }
            number++;
            
        }
        std::cout<<number<<" ";
        std::cout << tooluur;
    }

    return 0;
}