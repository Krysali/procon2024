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
                Action action(botX - sz + 1, botY - sz + 1, diceNum - 2, 0);
                actions.push_back(action);

                // TYPE II
                if(height > 2){
                    for(int i = 0; i < 2; i++){
                        Action action(botX - sz + 1, botY - sz + 1, diceNum - 1, i);
                        actions.push_back(action);
                    }
                }

                // TYPE III
                if(m % 2 == 0){
                    Action action1(botX - sz + 1, botY - sz + 1, diceNum, 0);
                    actions.push_back(action1);
                }else{
                    Action action1(botX + 1 - sz + 1, botY - sz + 1, diceNum, 0);
                    actions.push_back(action1);
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
                    Action action(botX - sz + 1, botY - sz + 1, diceNum - 1, 2);
                    actions.push_back(action);
                }else{
                    Action action(botX - sz + 1, botY + 1 - sz + 1, diceNum - 1, 2);
                    actions.push_back(action);
                }

                // TYPE III
                if(width > 2){
                    for(int i = 2; i < 4; i++){
                        Action action(botX - sz + 1, botY - sz + 1, diceNum, i);
                        actions.push_back(action);
                    }
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

                // TYPE II
                /*if(n % 2 == 1){
                    for(int i = 0; i < 2; i++){
                        Action action(botX, botY - sz + 1, diceNum - 1, i);
                        actions.push_back(action);
                    }
                }*/

                // TYPE III
                if(width != 1){
                    for(int i = 2; i < 4; i++){
                        Action action(botX, botY - sz + 1, diceNum, i);
                        actions.push_back(action);
                    }
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

                // TYPE II
                if(height != 1){
                    for(int i = 0; i < 2; i++){
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
    if(1){
        topY = 0;
        for(topX = 1; topX < m - 1; topX++){
            for(width = 1; width < (m - 1 - topX); width *= 2){
                for(height = 1; height <= min(width, n - 1); height++){
                    sz = width;

                    if(sz == 1){
                        if(topX == 1 || topX == m - 2) continue;
                        for(int i = 0; i < 4; i++){
                            if(i == 1) continue;
                            Action action(topX, topY, 0, i);
                            actions.push_back(action);
                        }
                        continue;
                    }

                    // 3th type dice of this size
                    diceNum = (log2(sz) * 3);

                    // TYPE I
                    for(int i = 0; i < 4; i++){
                        if(i == 1) continue;
                        Action action(topX, height - width, (diceNum - 2), i);
                        actions.push_back(action);
                    }


                    // TYPE II
                    if(height > 2){
                        for(int i = 0; i < 4; i++){
                            Action action(topX, height - width, (diceNum - 1), i);
                            actions.push_back(action);  
                        }
                    }

                    // TYPE III
                    if(topX == 1 || topX == m - 2) continue;
                    if(sz == 2 && height == 1) continue;
                    for(int i = 0; i < 4; i++){
                        if(i == 1) continue;
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

                    if(sz == 1){
                        if(topX == 1 || topX == m - 2) continue;
                        if(oh > ow){
                            Action action(topX, topY, 0, 1);
                            actions.push_back(action);
                        }
                        for(int i = 2; i < 4; i++){
                            Action action(topX, topY, 0, i);
                            actions.push_back(action);
                        }
                        continue;
                    }

                    // 3th type dice of this size
                    diceNum = (log2(sz) * 3);

                    // TYPE I
                    if(oh > ow){
                        Action action(topX, topY - height + 1, (diceNum - 2), 1);
                        actions.push_back(action);
                    }
                    for(int i = 2; i < 4; i++){
                        Action action(topX, topY - height + 1, (diceNum - 2), i);
                        actions.push_back(action);
                    }
                    
                    
                    // TYPE II
                    if(height != 1){
                        for(int i = 0; i < 4; i++){
                            Action action(topX, topY - height + 1, (diceNum - 1), i);
                            actions.push_back(action);  
                        }
                    }
                    
                    // TYPE III
                    if(topX == 1 || topX == m - 2) continue;
                    if(sz == 2 && height == 1) continue;
                    if(oh > ow){
                        Action action(topX, topY - height + 1, diceNum, 1);
                        actions.push_back(action); 
                    }

                    for(int i = 2; i < 4; i++){
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

                    if(sz == 1){
                        if(topY == 1 || topY == n - 2) continue;
                        for(int i = 0; i < 3; i++){
                            Action action(width - height, topY, 0, i);
                            actions.push_back(action);
                        }
                        continue;
                    }

                    // 3th type dice of this size
                    diceNum = (log2(sz) * 3);

                    // TYPE I
                    for(int i = 0; i < 3; i++){
                        Action action(width - height, topY, (diceNum - 2), i);
                        actions.push_back(action);
                    }

                    // TYPE III
                    if(width > 2){
                        for(int i = 0; i < 4; i++){
                            Action action(width - height, topY, diceNum, i);
                            actions.push_back(action);  
                        }
                    }

                    // TYPE II
                    if(topY == 1 || topY == n - 2) continue;
                    if(sz == 2 && width == 1) continue;
                    for(int i = 0; i < 3; i++){
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

                    if(sz == 1){
                        if(topY == 1 || topY == n - 2) continue;
                        if(ow > oh){
                            Action action(topX, topY, 0, 3);
                            actions.push_back(action);
                        }
                        for(int i = 0; i < 2; i++){
                            Action action(topX, topY, 0, i);
                            actions.push_back(action);
                        }
                        continue;
                    }

                    // 3th type dice of this size
                    diceNum = (log2(sz) * 3);

                    // TYPE I
                    if(ow > oh){
                        Action action(topX - width + 1, topY, (diceNum - 2), 3);
                        actions.push_back(action);
                    }
                    for(int i = 0; i < 2; i++){
                        Action action(topX - width + 1, topY, (diceNum - 2), i);
                        actions.push_back(action);
                    }

                    
                    // TYPE III
                    if(width != 1){
                        for(int i = 0; i < 4; i++){
                            Action action(topX - width + 1, topY, diceNum, i);
                            actions.push_back(action);  
                        }
                    }

                    
                    // TYPE II
                    if(topY == 1 || topY == n - 2) continue;
                    if(sz == 2 && width == 1) continue;

                    if(ow > oh){
                        Action action(topX - width + 1, topY, diceNum - 1, 3);
                        actions.push_back(action); 
                    }

                    for(int i = 0; i < 2; i++){
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
                for(int i = 2; i < 4; i++){
                    Action action(topX, topY, diceNum - 2, i);
                    actions.push_back(action);
                }

                // TYPE II
                for(int i = 2; i < 4; i++){
                    if(i == 1 && sz == 2) continue; 
                    Action action(topX, topY, diceNum - 1, i);
                    actions.push_back(action);
                }

                if(topX == 1 || topX == m - 2) continue;

                // TYPE III
                for(int i = 2; i < 4; i++){
                    Action action(topX, topY, diceNum, i);
                    actions.push_back(action);
                }
            }
        }
    }

    cout << actions.size();
    return 0;
}