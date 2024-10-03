#include <types.h>
#include <ops.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <map>
#include <string>
using namespace std;
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

class nuudel {
    public:
        int nuudelnum;
        int accuracy;
};
vector<Action> actions , actions2;

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

bool log2check(int x) {
    double sad = log2(x);
    int tselmeg = sad * 10;
    if(tselmeg % 10 == 0) {
        return true;
    } else {
        return false;
    }
}

int rowcheck(Board odoogiinstate , Board goalstate , int n , int m) {
    int total = 0;
    for(int i = 0; i < n; i++) {
        for(int loc = 0; loc < m; loc++) {
            for(int j = 0; j < m; j++) {
                if(odoogiinstate.pieces[i][loc] == goalstate.pieces[i][j]) {
                    goalstate.pieces[i][j] = -2;
                    total++;
                    break;
                }
            }
        }
    }
    return total;
}
/*void Tits(Board odoogiinboard , int n , int m){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            cout << odoogiinboard.pieces[i][j] << ' ';
        }
        cout << endl;
    }
}*/
int columncheck(Board odoogiinstate , Board goalstate , int n , int m) {
    int total = 0;
    for(int i = 0; i < n; i++) {
        for(int loc = 0; loc < m; loc++) {
            for(int j = 0; j < m; j++) {
                if(odoogiinstate.pieces[loc][i] == goalstate.pieces[j][i]) {
                    goalstate.pieces[j][i] = -2;
                    total++;
                    break;
                }
            }
        }
    }
    if(total == 9){

    }
    return total;
}

int main()
{
    int topX, topY, botX, botY, width, height, mxSide, sz, diceNum;
    cout << "test" << endl;
    vector<Board> sexVectorAndTitty;
    // n = height, m = width
    int n, m;
    std::cout << "Width: ";
    cin >> m;
    std::cout << endl;
    std::cout << "Height: ";
    cin >> n;
    std::cout << endl << "startstate ;" << endl;

    // urgelj m ni urt tal ni baina 
    if(n > m) swap(n, m);

    // Start state
    vector<int> startstate;
    int piece;
    for(int i = 0; i < n * m; i++){
        cin >> piece;
        startstate.push_back(piece); 
    }  
    Board odoogiinboard;
    odoogiinboard.width = m;
    odoogiinboard.height = n;
    for(int i = 0; i < n; i++){
        vector<int> row;
        for(int j = 0; j < m; j++){
            row.push_back(startstate[i * n + j]);
        }
        odoogiinboard.pieces.push_back(row);
    }
    sexVectorAndTitty.push_back(odoogiinboard);

    std::cout << "Goalstate ;" << endl;

    //Goal state
    vector<int> goalstate;
    for(int i = 0; i < n * m; i++){
        cin >> piece;
        goalstate.push_back(piece); 
    }
    Board goalboard;
    goalboard.width = m;
    goalboard.height = n;
    for(int i = 0; i < n; i++){
        vector<int> row;
        for(int j = 0; j < m; j++){
            row.push_back(goalstate[i * n + j]);
        }
        goalboard.pieces.push_back(row);
    }

   

    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            cout << goalboard.pieces[i][j] << ' ';
        }
        cout << endl;
    }

    std::cout << "Working" << endl;

    // urgelj inside the board` 
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

                    if(sz == 1){
                        for(int i = 0; i <= 2; i += 2){
                            Action action(topX, topY, 0, i);
                            actions.push_back(action);
                        }
                        continue;
                    }

                    // TYPE I
                    for(int i = 0; i <= 2; i += 2){
                        Action action(botX - sz + 1, botY - sz + 1, diceNum - 2, i);
                        actions.push_back(action);
                    }


                    // TYPE II
                    if (height > 2) {
                        for(int i = 0; i < 3; i++){
                            Action action(botX - sz + 1, botY - sz + 1, diceNum - 1, i);
                            actions.push_back(action);
                        }
                    }

                    // TYPE III
                    if(width > 2){
                        for(int i = 0; i < 4; i++){
                            if(i == 1) continue;
                            Action action(botX - sz + 1, botY - sz + 1, diceNum, i);
                            actions.push_back(action);
                        }
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

                    // TYPE II
                    if(height > 2){
                        for(int i = 0; i < 2; i++){
                            Action action(topX, botY - sz + 1, (diceNum - 1), i);
                            actions.push_back(action);  
                        }
                    }

                    // TYPE III
                    if(width != 1){
                        for(int i = 2; i < 4; i++){
                            Action action(topX, botY - sz + 1, (diceNum), i);
                            actions.push_back(action);
                        }
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

                    // TYPE II
                    if(height != 1){
                        for(int i = 0; i < 2; i++){
                            Action action(botX - sz + 1, topY, (diceNum - 1), i);
                            actions.push_back(action);  
                        }
                    }

                    // TYPE III
                    if(width > 2){
                        for(int i = 2; i < 4; i++){
                            Action action(botX - sz + 1, topY, diceNum, i);
                            actions.push_back(action);  
                        }
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

                    if(sz == 1){
                        for(int i = 1; i <= 3; i += 2){
                            Action action(topX, topY, 0, i);
                            actions.push_back(action);
                        }
                        continue;
                    }

                    // 3th type dice of this size
                    diceNum = (log2(sz) * 3);

                    // TYPE I
                    for(int i = 1; i <= 3; i += 2){
                        Action action(topX, topY, (diceNum - 2), i);
                        actions.push_back(action);
                    }

                    // TYPE II
                    if(height != 1){
                        for(int i = 0; i < 4; i++){
                            if((i == 2)) continue;
                            Action action(topX, topY, (diceNum - 1), i);
                            actions.push_back(action);  
                        }
                    }

                    // TYPE III
                    if(width != 1){
                        for(int i = 1; i < 4; i++){
                            Action action(topX, topY, diceNum, i);
                            actions.push_back(action);  
                        }
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
            if( 1){
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
                for(width = 1; width <= (m - 1 - topX); width *= 2){
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

    if(1){
           topY = 0;
           sz = log2(n - 1) + 1;
           for ( int szz = pow(2, sz); szz < m - 1; szz*=2){
             for(topX = 1; topX < m - szz; topX++){
                 // 3th type dice of this size
                 if(n + 1 < m && szz <= m - 2 && topX + szz < m){
                     diceNum = log2(szz) * 3;
                      // TYPE I
                      for(int i = 2; i < 4; i++){
                          Action action(topX, topY, diceNum - 2, i);
                          actions.push_back(action);
                      }

                      // TYPE II
                      for(int i = 2; i < 4; i++){
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
        }
    
    string urgeljluul = "oldoogui";
    int NiitNuudel = 0;
    while(urgeljluul == "oldoogui") {
        sexVectorAndTitty.push_back(odoogiinboard);
        vector<nuudel> allmove;

        // CHECK
        if(1){
            int tooluur = 0 , number = 0;
            for(int a = 0; a < actions.size(); a++){
                //cout << "x:" << a.x << " y:" << a.y << " diceType:" << a.diceType << " dir:" << a.dir << '\n';
                //cout << a << en

                //Moveing by random move
                GameState nextState;
                if(1) {
                    nextState.board = odoogiinboard;
                    nextState.dies = GenerateFixedDies();
                    apply_die(nextState, actions[a].diceNum, actions[a].x, actions[a].y, actions[a].dir);
                    //display_game_state(nextState);
                }

                //Checking this state same with old state
                bool checkstate = false;
                if(1) {
                    for(int chk = 0; chk < sexVectorAndTitty.size(); chk++) {
                        bool CheckOneState = true;
                        for(int urt = 0; urt < m; urt++) {
                            for(int orgon = 0; orgon < n; orgon++) {
                                if(nextState.board.pieces[urt][orgon] != sexVectorAndTitty[chk].pieces[urt][orgon]) {
                                    CheckOneState = false;
                                    break;
                                }
                            }
                        }
                        if(CheckOneState == true) {
                            checkstate = true;
                            break;
                        }
                    }
                }
                
                //Map working 
                if(checkstate == false) {
                    nuudel shinj;
                    shinj.accuracy = 0;
                    shinj.nuudelnum = a;
                    shinj.accuracy += rowcheck(nextState.board , goalboard , n , m);
                    shinj.accuracy += columncheck(nextState.board , goalboard , n , m);
                    //Tits(nextState.board, n , m);
                    allmove.push_back(shinj);
                    //std::cout << "x; " << a.x << " y; " << a.y << " dice; " << a.diceNum << " dir; " << a.dir << endl;
                }else{
                    //std::cout << "x; " << a.x << " y; " << a.y << " dice; " << a.diceNum << " dir; " << a.dir << endl;
                    //display_game_state(nextState);
                }
            }
        }
        nuudel realnuudel;

        //Find best predict move
        if(1) {
            realnuudel.accuracy = 0;
            for(int a = 0; a < allmove.size(); a++) {
                if(realnuudel.accuracy <= allmove[a].accuracy) {
                    realnuudel.nuudelnum = allmove[a].nuudelnum;
                    realnuudel.accuracy = allmove[a].accuracy;
                }
            }
            cout << realnuudel.nuudelnum << " " << realnuudel.accuracy << endl;
            cout << actions[realnuudel.nuudelnum].x << " " << actions[realnuudel.nuudelnum].y << " " << actions[realnuudel.nuudelnum].diceNum << " " << actions[realnuudel.nuudelnum].dir << " " << endl;
        }
        
        GameState realState;
        //cout << shinj.accuracy << endl;
        //cout << actions[shinj.nuudelnum].x << " " << actions[shinj.nuudelnum].y << " " << actions[shinj.nuudelnum].diceNum << " " << actions[shinj.nuudelnum].dir << " " << endl;
        //Moving by best predicted move
        if(1) {
            realState.board = odoogiinboard;
            realState.dies = GenerateFixedDies();
            apply_die(realState, actions[realnuudel.nuudelnum].diceNum, actions[realnuudel.nuudelnum].x, actions[realnuudel.nuudelnum].y, actions[realnuudel.nuudelnum].dir);
            display_game_state(realState);
            
        }
        NiitNuudel++;
        //Checking are we in goalstate
        if(1) {
            vector<int> llr;
            for(int i = 0; i < n; i++){
                for(int j = 0; j < m; j++){
                    int too = realState.board.pieces[i][j];
                    llr.push_back(too);
                }
            }
            bool shalga = true;
            for(int a = 0; a < llr.size(); a++) {
                if(llr[a] != goalstate[a]) {
                    shalga = false;
                }
            }
            if(shalga == true) {
                urgeljluul = "oldson ";
                std::cout << urgeljluul << endl << "NiitNuudel " << NiitNuudel;
                break;
            }
        }
        
        // Updating previous state 
        std::cout << "nextmove" << endl;
        //cout << "medeelel shinjleh" << endl;
        odoogiinboard.width = m;
        odoogiinboard.height = n;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                odoogiinboard.pieces[i][j] = realState.board.pieces[i][j]; // Copy each element from realState
            }
        }   
    // cout<< "uldsen" << endl;
    // int startX, startY, endX, endY , ih , sda;
    // ih = max(n , m);
    // sz = nearest(ih);
    // for(int startX = 0 - sz + 1; startX < m + sz - 2; startX++) {
    //     for(int endX = startX; endX < m + sz - 2; endX++) {
    //         for(int startY = 0 - sz + 1; startY < n + sz - 2; startY++) {
    //             for(int endY = startY; endY < n + sz - 2; endY++) {
    //                 width = endX - startX + 1;
    //                 height = endY - startY + 1;
    //                 if(width == height && log2check(width) == 1) {
    //                     sda = width;
    //                     if(startX >= 0 && startY >= 0 && startX < m && startY < n) {
    //                         //Print debug info
    //                         //cout<< "start point state deer" << ", width=" << width << ", height=" << height << endl;
    //                         for(int dir = 0; dir < 4; dir++) {
    //                             diceNum = log2(sda) * 3;
    //                             if(sda == 1) {
    //                                 Action action(startX, startY, 0, dir);
    //                                 actions2.push_back(action);
    //                             } else {
    //                                 for(int i = 0; i < 3; i++) {
    //                                     Action action(startX, startY, diceNum - i , dir);
    //                                     actions2.push_back(action);
    //                                 }
    //                             }
    //                         }
    //                    } else if(endX >= 0 && endY >= 0 && endX < m && endY < n) {
    //                         //Print debug info
    //                         //cout<< "end point state deer" << ", width=" << width << ", height=" << height << endl;
    //                         for(int dir = 0; dir < 4; dir++) {
    //                             diceNum = log2(sda) * 3;
    //                             if(sda == 1) {
    //                                 Action action(startX, startY, 0, dir);
    //                                 actions2.push_back(action);
    //                             } else {
    //                                 for(int i = 0; i < 3; i++) {
    //                                     Action action(startX, startY, diceNum - i , dir);
    //                                     actions2.push_back(action);
    //                                 }
    //                             }
    //                         }
    //                    } else if(startX >= 0 && startX < m && endY >= 0 && endY < n) {
    //                         //Print debug info
    //                         //cout<< "end point state deer" << ", width=" << width << ", height=" << height << endl;
    //                         for(int dir = 0; dir < 4; dir++) {
    //                             diceNum = log2(sda) * 3;
    //                             if(sda == 1) {
    //                                 Action action(startX, startY, 0, dir);
    //                                 actions2.push_back(action);
    //                             } else {
    //                                 for(int i = 0; i < 3; i++) {
    //                                     Action action(startX, startY, diceNum - i , dir);
    //                                     actions2.push_back(action);
    //                                 }
    //                             }
    //                         }
    //                    } else if(startY >= 0 && startY < n && endX >= 0 && endX < m) {
    //                         //Print debug info
    //                         //cout<< "end point state deer" << ", width=" << width << ", height=" << height << endl;
    //                         for(int dir = 0; dir < 4; dir++) {
    //                             diceNum = log2(sda) * 3;
    //                             if(sda == 1) {
    //                                 Action action(startX, startY, 0, dir);
    //                                 actions2.push_back(action);
    //                             } else {
    //                                 for(int i = 0; i < 3; i++) {
    //                                     Action action(startX, startY, diceNum - i , dir);
    //                                     actions2.push_back(action);
    //                                 }
    //                             }
    //                         }
    //                    }
    //                 }
    //             }
    //         }
    //     }
    // }
    // // CHECK
    // if(1){  
    //     int tooluur = 0 , number = 0;
    //     for(auto a : actions2){
    //         GameState nextState;
    //         nextState.board = board;
    //         nextState.dies = GenerateFixedDies();
    //         apply_die(nextState, a.diceNum, a.x, a.y, a.dir);
    //         //display_game_state(nextState);

    //         vector<int> llr;
    //         for(int i = 0; i < n; i++){
    //             for(int j = 0; j < m; j++){
    //                 int too = nextState.board.pieces[i][j];
    //                 llr.push_back(too);
    //             }
    //         }
    //         if(check[llr] == 0){
    //             check[llr]++;
    //             tooluur++;
    //             std::cout << "x; " << a.x << " y; " << a.y << " dice; " << a.diceNum << " dir; " << a.dir << endl;
    //         }else{
    //             //std::cout << "x; " << a.x << " y; " << a.y << " dice; " << a.diceNum << " dir; " << a.dir << endl;
    //         }
    //         number++;
            
    //     }
    //     std::cout << number << " ";
    //     std::cout << tooluur;
    // }
    }
    return 0;
}