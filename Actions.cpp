#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

#define xx first
#define yy second

class Action {
    public:
        int x, y, diceType, dir;
        Action(int topX, int topY, int d, int s){
            x = topX;
            y = topY;
            diceType = d;
            dir = s;
        }
};

int n, m, topX, topY, bottomX, bottomY, height, width, mxSide;
vector<Action> actions;
vector<pair<int, int>> cord, pairs;

int nearest(int x){
    int y = 1;
    while(x > y){
        y *= 2;
    }
    return y;
}

bool isPowerOfTwo(int x)
{
    if(x == 1) return 1;
    if(x % 2 == 1) return 0;
    return isPowerOfTwo(x / 2);
}

void main_()
{
    cin >> n >> m;
    if(n > m) swap(n, m);

    // inside the board actions
    for(topY = 1; topY < n - 1; topY++){
        for(topX = 1; topX < m - 1; topX++){
            for(int size = 1; size <= min(n - topY - 1, m - topX - 1); size *= 2){
                // 3th type dice of this size
                int diceNum = (log2(size) * 3);
                for(int s = 0; s < 4; s++){
                    if(size == 1){
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

    // corner contained actions

    // topleft corner contained
    if(true){
        topY = 0;
        topX = 0;
        for(int bottomY = 0; bottomY < n - 1; bottomY++){
            for(int bottomX = 0; bottomX < m - 1; bottomX++){
                height = bottomY - topY + 1;
                width = bottomX - topX + 1;
                mxSide = max(height, width);
                int size = nearest(mxSide);

                if(size == 1){
                    for(int i = 0; i <= 2; i += 2){
                        Action action(topX, topY, 0, i);
                        actions.push_back(action);
                    }
                    continue;
                }

                // 3th type dice of this size
                int diceNum = (log2(size) * 3);

                // TYPE I
                for(int i = 0; i <= 2; i += 2){
                    Action action(bottomX - size + 1, bottomY - size + 1, diceNum - 2, i);
                    actions.push_back(action);
                }

                // TYPE II
                if(height != 1){
                    for(int i = 0; i < 3; i++){
                        if(i == 1 && height == 2) continue;
                        Action action(bottomX - size + 1, bottomY - size + 1, (diceNum - 1), i);
                        actions.push_back(action);  
                    }
                }

                // TYPE III
                if(width != 1){
                    for(int i = 0; i < 4; i++){
                        if((i == 1) || (i == 3 && width == 2)) continue;
                        Action action(bottomX - size + 1, bottomY - size + 1, diceNum, i);
                        actions.push_back(action);  
                    }
                }

            }
        }
    }
    
    // topright corner contained
    if(true){
        topY = 0;
        bottomX = m - 1;

        for(int bottomY = 0; bottomY < n - 1; bottomY++){
            for(int topX = 1; topX <= m - 1; topX++){
                height = bottomY - topY + 1;
                width = bottomX - topX + 1;
                mxSide = max(height, width);
                int size = nearest(mxSide);

                // 3th type dice of this size
                int diceNum = (log2(size) * 3);

                // TYPE II
                if(height != 1){
                    for(int i = 0; i < 2; i++){
                        if(i == 1 && height == 2) continue;
                        Action action(topX, bottomY - size + 1, (diceNum - 1), i);
                        actions.push_back(action);  
                    }
                }
                

                // TYPE III
                for(int i = 2; i < 4; i++){
                    if((i == 2 && width == 1))continue;
                    Action action(topX, bottomY - size + 1, diceNum, i);
                    actions.push_back(action);  
                }
            }
        }
    }

    // bottomleft corner contained
    if(true){
        bottomY = 0;
        topX = 0;

        for(int topY = 1; topY <= n - 1; topY++){
            for(int bottomX = 0; bottomX < m - 1; bottomX++){
                height = bottomY - topY + 1;
                width = bottomX - topX + 1;
                mxSide = max(height, width);
                int size = nearest(mxSide);

                // 3th type dice of this size
                int diceNum = (log2(size) * 3);

                // TYPE II
                for(int i = 0; i < 2; i++){
                    if(i == 0 && height == 1) continue;
                    Action action(bottomX - size + 1, topY, (diceNum - 1), i);
                    actions.push_back(action);  
                }

                // TYPE III
                if(width != 1){
                    for(int i = 2; i < 4; i++){
                        if(i == 3 && width == 2) continue;
                        Action action(bottomX - size + 1, topY, diceNum, i);
                        actions.push_back(action);  
                    }
                }
            }
        }
    }

    // bottomright corner contained
    if(true){
        bottomX = m - 1;
        bottomY = n - 1;

        for(int topY = 1; topY <= n - 1; topY++){
            for(int topX = 1; topX <= m - 1; topX++){
                height = bottomY - topY + 1;
                width = bottomX - topX + 1;
                mxSide = max(height, width);
                int size = nearest(mxSide);

                if(size == 1){
                    for(int i = 1; i <= 3; i += 2){
                        Action action(topX, topY, 0, i);
                        actions.push_back(action);
                    }
                    continue;
                }

                // 3th type dice of this size
                int diceNum = (log2(size) * 3);

                // TYPE I
                for(int i = 1; i <= 3; i += 2){
                    Action action(topX, topY, (diceNum - 2), i);
                    actions.push_back(action);
                }

                // TYPE II
                for(int i = 0; i < 4; i++){
                    if((i == 2) || (i == 0 && height == 1)) continue;
                    Action action(bottomX - size + 1, topY, (diceNum - 1), i);
                    actions.push_back(action);  
                }

                // TYPE III
                if(width != 1){
                    for(int i = 1; i < 4; i++){
                        if(i == 2 && width == 1) continue;
                        Action action(bottomX - size + 1, topY, diceNum, i);
                        actions.push_back(action);  
                    }
                }
            }
        }
    }
    
    // pairs of corners
    if(true){
        //{x, y}
        cord.push_back({0, 0}); // topleft 0
        cord.push_back({m - 1, 0}); // topright 1
        cord.push_back({0, n - 1}); // bottomleft 2
        cord.push_back({m - 1, n - 1}); // bottomright 3

        pairs.push_back({0, 1}); // from above                                  
        pairs.push_back({0, 2}); // from left                                  
        pairs.push_back({1, 3}); // from right                                  
        pairs.push_back({2, 3}); // from bottom                                  
        int CASE = 0;

        for(auto p : pairs){
            auto top = cord[p.xx];
            auto bottom = cord[p.yy];
            
            switch (CASE) {
                case 0:
                    for(bottomY = 0; bottomY < n - 1; bottomY++){
                        width = m;
                        height = bottomY - top.yy + 1;
                        mxSide = max(width, height);
                        int size = nearest(mxSide);
                        // 3th type dice of this size
                        int diceNum = (log2(size) * 3);

                        // TYPE I
                        Action action(bottom.xx - size + 1, bottomY - size + 1, diceNum - 2, 0);
                        actions.push_back(action);

                         // TYPE II
                        if(height != 1){
                            for(int i = 0; i < 2; i++){
                                if(i == 1 && height == 2) continue;
                                Action action(bottom.xx - size + 1, bottomY - size + 1, diceNum - 1, i);
                                actions.push_back(action);
                            }
                        }
                        
                        // TYPE III
                        for(int i = 0; i < 4; i++){
                            if((i == 1) || (i == 3 && width == 2)) continue;
                            Action action(bottom.xx - size + 1, bottomY - size + 1, diceNum, i);
                            actions.push_back(action);  
                        }
                    }
                    break;
                case 1:
                    for(bottomX = 0; bottomX < m - 1; bottomX++){
                        width = bottomX - topX + 1;
                        height = n;
                        mxSide = max(width, height);
                        int size = nearest(mxSide);
                        // 3th type dice of this size
                        int diceNum = (log2(size) * 3);

                        // TYPE I
                        Action action(bottomX - size + 1, bottom.yy - size + 1, diceNum - 2, 2);
                        actions.push_back(action);

                        // TYPE II
                        for(int i = 0; i < 3; i++){
                            if(i == 1 && n == 2) continue;
                            Action action(bottomX - size + 1, bottom.yy - size + 1, (diceNum - 1), i);
                            actions.push_back(action);  
                        }

                        // TYPE III
                        if(width != 1){
                            for(int i = 2; i <= 3; i++){
                                if(i == 3 && width == 2) continue;
                                Action action(bottomX - size + 1, bottom.yy - size + 1, diceNum, i);
                                actions.push_back(action);  
                            }
                        }
                    }
                    break;
                case 2:
                    for(bottomX = m - 1; bottomX > 0; bottomX--){
                        width = bottomX - topX + 1;
                        height = n;
                        mxSide = max(width, height);
                        int size = nearest(mxSide);
                        // 3th type dice of this size
                        int diceNum = (log2(size) * 3);

                        // TYPE II
                        for(int i = 0; i < 2; i++){
                            if(i == 2 || (i == 1 && n == 2)) continue;
                            Action action(bottomX, top.yy, (diceNum - 1), i);
                            actions.push_back(action);  
                        }

                        // TYPE III
                        for(int i = 2; i <= 3; i++){
                            if(i == 2 && width == 1) continue;
                            Action action(bottomX, top.yy, diceNum, i);
                            actions.push_back(action);  
                        }

                    }
                    break;
                case 3:
                    for(bottomY = n - 1; bottomY > 0; bottomY--){
                        width = m;
                        height = top.yy - bottomY + 1;
                        mxSide = max(width, height);
                        int size = nearest(mxSide);
                        // 3th type dice of this size
                        int diceNum = (log2(size) * 3);

                         // TYPE II
                        for(int i = 0; i < 2; i++){
                            if(i == 0 && height == 1) continue;
                            Action action(top.xx, bottomY, diceNum - 1, i);
                            actions.push_back(action);
                        }
                        
                        // TYPE III
                        for(int i = 2; i < 4; i++){
                            Action action(top.xx, bottomY, diceNum, i);
                            actions.push_back(action);  
                        }
                    }
                    break;
            }
            CASE++;

        }
    }
    
    // edge 

    // top
    topY = 0;
    for(topX = 1; topX < m - 1; topX++){
        for(width = 1; width <= (m - 1 - topX); width *= 2){
            for(height = 1; height <= min(width, n - 1); height++){
                int size = width;

                if(size == 1){
                    for(int i = 0; i < 4; i++){
                        if(i == 1) continue;
                        Action action(topX, topY, 0, i);
                        actions.push_back(action);
                    }
                    continue;
                }

                // 3th type dice of this size
                int diceNum = (log2(size) * 3);

                // TYPE I
                for(int i = 0; i < 4; i++){
                    if(i == 1) continue;
                    Action action(topX, height - width, (diceNum - 2), i);
                    actions.push_back(action);
                }

                // TYPE II
                if(height != 1){
                    for(int i = 0; i < 4; i++){
                        if(i == 1 && height == 2) continue;
                        Action action(topX, height - width, (diceNum - 1), i);
                        actions.push_back(action);  
                    }
                }

                // TYPE III
                for(int i = 0; i < 4; i++){
                    if(i == 1) continue;
                    Action action(topX, height - width, diceNum, i);
                    actions.push_back(action);  
                }
            }
        }
    }

    // left
    topX = 0;
    for(topY = 1; topY < n - 1; topY++){
        for(height = 1; height <= (n - 1 - topY); height *= 2){
            for(width = 1; width <= height; width++){
                int size = height;

                if(size == 1){
                    for(int i = 0; i < 4; i++){
                        if(i == 3) continue;
                        Action action(width - height, topY, 0, i);
                        actions.push_back(action);
                    }
                    continue;
                }

                // 3th type dice of this size
                int diceNum = (log2(size) * 3);

                // TYPE I
                for(int i = 0; i < 3; i++){
                    Action action(width - height, topY, (diceNum - 2), i);
                    actions.push_back(action);
                }

                // TYPE II
                for(int i = 0; i < 3; i++){
                    Action action(width - height, topY, (diceNum - 1), i);
                    actions.push_back(action);  
                }

                // TYPE III
                if(width != 1){
                    for(int i = 0; i < 4; i++){
                        if(i == 3 && width == 2) continue;
                        Action action(width - height, topY, diceNum, i);
                        actions.push_back(action);  
                    }
                }

            }
        }
    }

    // right
    topX = m - 1;
    for(topY = 1; topY < n - 1; topY++){
        for(height = 1; height <= (n - 1 - topY); height *= 2){
            for(width = 1; width <= height; width++){
                int size = height;

                // 3th type dice of this size
                int diceNum = (log2(size) * 3);

                // TYPE II
                for(int i = 0; i < 2; i++){
                    Action action(topX - width + 1, topY, (diceNum - 1), i);
                    actions.push_back(action);  
                }

                // TYPE III
                for(int i = 0; i < 4; i++){
                    if(i == 2 && width == 1) continue;
                    Action action(topX - width + 1, topY, diceNum, i);
                    actions.push_back(action);  
                }

            }
        }
    }

    // bottom
    topY = n - 1;
    for(topX = 1; topX < m - 1; topX++){
        for(width = 1; width <= (m - 1 - topX); width *= 2){
            for(height = 1; height <= min(width, n - 1); height++){
                int size = width;
                
                // 3th type dice of this size
                int diceNum = (log2(size) * 3);

                // TYPE II
                for(int i = 0; i < 4; i++){
                    if(i == 0 && height == 1) continue;
                    Action action(topX, topY - height + 1, (diceNum - 1), i);
                    actions.push_back(action);  
                }

                // TYPE III
                for(int i = 2; i < 4; i++){
                    Action action(topX, topY - height + 1, diceNum, i);
                    actions.push_back(action);  
                }
            }
        }
    }

     // pairs of edges

    if(isPowerOfTwo(n)){
        if(n + 1 < m){
            topY = 0;
            for(topX = 1; topX < m - n; topX++){
                int size = n;
                // 3th type dice of this size
                int diceNum = (log2(size) * 3);

                // TYPE I
                for(int i = 2; i < 4; i++){
                    Action action(topX, topY, diceNum - 2, i);
                    actions.push_back(action);
                }

                // TYPE II
                for(int i = 0; i < 4; i++){
                    if(i == 1 && size == 2) continue; 
                    Action action(topX, topY, diceNum - 1, i);
                    actions.push_back(action);
                }

                // TYPE III
                for(int i = 2; i < 4; i++){
                    Action action(topX, topY, diceNum, i);
                    actions.push_back(action);
                }
            }
        }
    }

    // Full dice
    if(true){
        int size = nearest(m);
        int diceNum = (log2(size) * 3);

        // TYPE II
        for(int i = 0; i < 2; i++){
            Action action(0, 0, diceNum - 1, i);
            actions.push_back(action);
        }

        // TYPE III
        for(int i = 2; i < 4; i++){
            Action action(0, 0, diceNum, i);
            actions.push_back(action);
        }
    }

    cout << actions.size() << '\n';
    for(auto a : actions){
        //cout << "x:" << a.x << " y:" << a.y << " diceType:" << a.diceType << " dir:" << a.dir << '\n';
    } 

    return ;
}

static void run_with_stack_size(void (*func)(void), size_t stsize) {
    char *stack, *send;
    stack = (char *)malloc(stsize);
    send = stack + stsize - 16;
    send = (char *)((uintptr_t)send / 16 * 16);
    asm volatile(
        "mov %%rsp, (%0)\n"
        "mov %0, %%rsp\n"
        :
        : "r"(send));
    func();
    asm volatile("mov (%0), %%rsp\n" : : "r"(send));
    free(stack);
}

int main() {
    run_with_stack_size(main_, 1024 * 1024 * 1024); // run with a 1 GiB stack
    return 0;
}
