#ifndef TYPES_H
#define TYPES_H

#include <vector>
#include <string>
#include <nlohmann/json.hpp>

const int MAX_DIMENSION = 256;

// Structure to represent a custom die
struct CustomDie {
    int width, height;
    std::vector<std::vector<bool>> cells;  // Use bool for 0/1 values

    CustomDie(int w, int h){
        width = w;
        height = h;
    }
};

// Structure to represent the game board
struct Board {
    int n, m;
    std::vector<std::vector<int>> pieces;

    Board() : n(0), m(0) {}

    Board(int N, int M){
        n = N;
        m = M;
    }
};

// Structure to represent a move
struct Action {
    int x, y;
    int die_index;
    int direction;

    Action(int X, int Y, int di, int dir){
        x = X;
        y = Y;
        die_index = di;
        direction = dir;
    }
};

// Structure to represent the entire game state
struct GameState {
    Board board;
    Board goal_state;
    std::vector<Action> moves;

    GameState(Board b, Board g){
        board = b;
        goal_state = g;
    }
    
    void apply_die(Action action)
    
    bool is_solved(){
        for(int i = 0; i < board.n; i++){
            for(int j = 0; j < board.m; j++){
                if(board.pieces[i][j] != goal_state.pieces[i][j]) return 0;
            }
        }
        return 1;
    }
};

bool is_solved(const GameState& game_state) {
    return (game_state.board.pieces == game_state.goal_state.pieces);
}

void GameState::apply_die(GameState& game_state, Action action) {

    auto board = game_state.board.pieces;
    int n = game_state.board.n;
    int m = game_state.board.m;
    // Set up
    int power = ceil(action.die_index / 3.0);
    int size = int(pow(2, power));
    int dice_type = (power == 0) ? 1 : action.die_index - ((power - 1) * 3);

    // cout << "die_index:" << action.die_index << ", n:" << n << ", m:" << m << ", power:" << power << ", size:" << size 
    //      << ", dice_type:" << dice_type << ", dir:" << action.dir << endl;

    std::vector<int> cut_pieces;

    int x_start = std::max(action.x, 0);
    int y_start = std::max(action.y, 0);
    int x_end = std::min(m, action.x + size) - 1;
    int y_end = std::min(n, action.y + size) - 1;

    int width = x_end - x_start + 1;
    int height = y_end - y_start + 1;

    int chosen_row_num = floor(height / 2);
    int chosen_col_num = floor(width / 2);
    int first = 1;

    // Determining first and chosen_x_num
    if (dice_type == 2) {
        first = (height + 1) % 2;
        if (is_inside(action.x, action.y, n, m, dice_type)) {
            if (!first) {
                chosen_row_num += 1;
            }
            first = 1;
        } else if (abs(action.y) % 2 != 0) {
            first = 0;
        } else if (abs(action.y) % 2 == 0) {
            if (height % 2 == 1) { // n bsn
                first = 1;
                chosen_row_num += 1;
            }
        }
    }

    if (dice_type == 3) {
        first = (width + 1) % 2;
        if (is_inside(action.x, action.y, n, m, dice_type)) {
            if (!first) {
                chosen_col_num += 1;
            }
            first = 1;
        } else if (abs(action.x) % 2 != 0) {
            first = 0;
        } else if (abs(action.x) % 2 == 0) {
            if (width % 2 == 1) { // m bsn
                first = 1;
                chosen_col_num += 1;
            }
        }
    }

    // cout << "x_start:" << x_start << ", x_end:" << x_end << ", y_start:" << y_start 
    //      << ", y_end:" << y_end << ", width:" << width << ", height:" << height 
    //      << ", chosen_row_num:" << chosen_row_num << ", chosen_col_num:" << chosen_col_num << endl;

    // CUT PHASE
    if (dice_type == 1) {
        for (int r = y_start; r <= y_end; ++r) {
            for (int c = x_start; c <= x_end; ++c) {
                cut_pieces.push_back(board[r][c]);
                board[r][c] = 0;
            }
        }
    }

    if (dice_type == 2) {
        int x = (first + 1) % 2;
        if (is_inside(action.x, action.y, n, m, dice_type)) {
            for (int r = y_start; r <= y_end; r += 2) {
                for (int c = x_start; c <= x_end; ++c) {
                    cut_pieces.push_back(board[r][c]);
                    board[r][c] = 0;
                }
            }
        } else {
            for (int r = y_start + x; r <= y_end; r += 2) {
                for (int c = x_start; c <= x_end; ++c) {
                    cut_pieces.push_back(board[r][c]);
                    board[r][c] = 0;
                }
            }
        }
    }

    if (dice_type == 3) {
        int x = (first + 1) % 2;
        if (is_inside(action.x, action.y, n, m, dice_type)) {
            for (int r = y_start; r <= y_end; ++r) {
                for (int c = x_start; c <= x_end; c += 2) {
                    cut_pieces.push_back(board[r][c]);
                    board[r][c] = 0;
                }
            }
        } else {
            for (int r = y_start; r <= y_end; ++r) {
                for (int c = x_start + x; c <= x_end; c += 2) {
                    cut_pieces.push_back(board[r][c]);
                    board[r][c] = 0;
                }
            }
        }
    }

    // SHIFT PHASE
    if (action.dir == 0) {
        for (int c = x_start; c <= x_end; ++c) {
            int write_index = y_start;
            for (int r = y_start; r < n; ++r) {
                if (board[r][c] != 0) {
                    board[write_index][c] = board[r][c];
                    ++write_index;
                }
            }
            for (int r = write_index; r < n; ++r) {
                board[r][c] = 0;
            }
        }
    }else if(action.dir == 1) {
        for (int c = x_start; c <= x_end; ++c) {
            int write_index = y_end;
            for (int r = y_end; r >= 0; --r) {
                if (board[r][c] != 0) {
                    board[write_index][c] = board[r][c];
                    --write_index;
                }
            }
            for (int r = write_index; r >= 0; --r) {
                board[r][c] = 0;
            }
        }
    }else if (action.dir == 2) {
        for (int r = y_start; r <= y_end; ++r) {
            int write_index = x_start;
            for (int c = x_start; c < m; ++c) {
                if (board[r][c] != 0) {
                    board[r][write_index] = board[r][c];
                    ++write_index;
                }
            }
            for (int c = write_index; c < m; ++c) {
                board[r][c] = 0;
            }
        }
    }else if (action.dir == 3) {
        for (int r = y_start; r <= y_end; ++r) {
            int write_index = x_end;
            for (int c = x_end; c >= 0; --c) {
                if (board[r][c] != 0) {
                    board[r][write_index] = board[r][c];
                    --write_index;
                }
            }
            for (int c = write_index; c >= 0; --c) {
                board[r][c] = 0;
            }
        }
    }

    // BBBT PHASE
    int bxs = 0, bxe = 0, bys = 0, bye = 0;

    if (dice_type == 1) {
        if (action.dir < 2) {
            bxs = x_start;
            bxe = x_end;
            if (action.dir == 0) {
                bys = n - height;
                bye = n - 1;
            } else {
                bys = 0;
                bye = height - 1;
            }
        } else {
            bys = y_start;
            bye = y_end;
            if (action.dir == 2) {
                bxs = m - width;
                bxe = m - 1;
            } else {
                bxs = 0;
                bxe = width - 1;
            }
        }
    } else if (dice_type == 2) {
        if (action.dir < 2) {
            bxs = x_start;
            bxe = x_end;
            if (action.dir == 0) {
                bys = n - chosen_row_num;
                bye = n - 1;
            } else {
                bys = 0;
                bye = chosen_row_num - 1;
            }
        } else {
            bys = y_start;
            bye = y_end;
            if (action.dir == 2) {
                bxs = m - width;
                bxe = m - 1;
            } else {
                bxs = 0;
                bxe = width - 1;
            }
        }
    } else if (dice_type == 3) {
        if (action.dir < 2) {
            bxs = x_start;
            bxe = x_end;
            if (action.dir == 0) {
                bys = n - height;
                bye = n - 1;
            } else {
                bys = 0;
                bye = height - 1;
            }
        } else {
            bys = y_start;
            bye = y_end;
            if (action.dir == 2) {
                bxs = m - chosen_col_num;
                bxe = m - 1;
            } else {
                bxs = 0;
                bxe = chosen_col_num - 1;
            }
        }
    }

    //cout << "bxs:" << bxs << ", bxe:" << bxe << ", bys:" << bys << ", bye:" << bye << endl;

    int cnt = 0;
    if (dice_type == 1) {
        for (int r = bys; r <= bye; ++r) {
            for (int c = bxs; c <= bxe; ++c) {
                board[r][c] = cut_pieces[cnt];
                cnt++;
            }
        }
    } else if (dice_type == 2 || dice_type == 3) {
        for (int r = bys; r <= bye; ++r) {
            for (int c = bxs; c <= bxe; ++c) {
                if (board[r][c] == 0) {
                    board[r][c] = cut_pieces[cnt];
                    cnt++;
                }
            }
        }
    }

    // Checking for empty cells
    // for (int r = 0; r < n; ++r) {
    //     for (int c = 0; c < m; ++c) {
    //         if (board[r][c] == 0) {
    //             cout << "AAAAAAIIIIIIIIIIIINNNNNNNNN" << endl;
    //         }
    //     }
    // }

    return;
}

Board reverseTypeI(int X, int Y, int size, int s, int n, int m, vector<vector<int>> board)
{
    int otx = std::max(X, 0);
    int oty = std::max(Y, 0);
    int obx = std::min(X + size - 1, m - 1);
    int oby = std::min(Y + size - 1, n - 1);
    int w = obx - otx + 1;
    int h = oby - oty + 1;
    vector<int> rem[256], chosen[128];
    if (s == 1) { 
        for(int index_y = h; index_y<=oby; index_y++){
            for(int index_x = otx; index_x<=obx; index_x++){                        
                rem[index_y-(h)].push_back(board[index_y][index_x]); 
            }   
        }
        for(int index_y = 0; index_y < h; index_y++){
            for(int index_x = otx;  index_x <= obx; index_x++){ 
                chosen[index_y].push_back(board[index_y][index_x]); 
            }                
        }
        int cnt = 0;
        for ( int index_y=0 ; index_y<=oby ; index_y++){
            if ( index_y< oby-h+1){
                cnt= 0 ; 
                for ( int index_x=otx; index_x<=obx; index_x++ ){
                    board[index_y][index_x]=rem[index_y][cnt] ; 
                    cnt ++ ; 
                }    
            }
            else {
                cnt = 0 ; 
                for ( int index_x=otx ; index_x<=obx; index_x++ ){
                    board[index_y][index_x]=chosen[index_y-(oby-h+1)][cnt] ; 
                    cnt ++ ; 
                }
            }
        }  
    }   
    if (s == 0) {
        for(int index_y = oty; index_y < n-h; index_y++){
            for(int index_x = otx; index_x <= obx; index_x++){                        
                rem[index_y-(oty)].push_back(board[index_y][index_x]);
            }  
        }   
        for(int index_y = n-h; index_y < n; index_y++){
            for(int index_x = otx;  index_x <= obx; index_x++){ 
                chosen[index_y-(n-h)].push_back(board[index_y][index_x]);
            }                 
        }
        int cnt = 0;
        for ( int index_y=oty ; index_y<n ; index_y++){
            if ( index_y<oty+h){
                cnt= 0 ; 
                for ( int index_x=(otx) ; index_x<=obx; index_x++ ){
                    board[index_y][index_x]=chosen[index_y-oty][cnt] ; 
                    cnt++; 
                }
            }
            else {
                cnt = 0 ; 
                for ( int index_x=otx ; index_x<=obx; index_x++ ){
                    board[index_y][index_x]=rem[index_y-(oty+h)][cnt] ; 
                    cnt ++ ; 
                }
            }
        }  
    }
    if (s==2 ) {
        int count = 0 ; 
        for(int index_y = oty; index_y <= oby; index_y++){
            for(int index_x = otx; index_x < m-w; index_x++){                        
                rem[count].push_back(board[index_y][index_x]);
            }  
            count ++ ; 
        }
        count = 0 ; 
        for(int index_y = oty; index_y <=oby; index_y++){
             for(int index_x = m-w;  index_x < m; index_x++){ 
                chosen[count].push_back(board[index_y][index_x]);         
            }
            count ++ ; 
        }
        int cnt = 0;
        for ( int index_x=otx ; index_x<m ; index_x++){
            if ( index_x <=obx ){
                cnt= 0 ; 
                for ( int index_y=oty; index_y<=oby; index_y++ ){
                    board[index_y][index_x]=chosen[cnt][index_x-otx] ; 
                    cnt ++ ;   
                }
            }
            else {
                cnt = 0 ; 
                for ( int index_y=oty ; index_y<=oby; index_y++){
                    board[index_y][index_x]= rem[cnt][index_x -(obx+1)] ; 
                  
                    cnt ++ ; 
                }
            }
        }   
    }
    if (s==3) { 
        int count = 0 ; 
        for(int index_y = oty; index_y <=oby; index_y++){
            for(int index_x = w; index_x <=obx; index_x++){                        
                rem[count].push_back(board[index_y][index_x]);
            }  
            count ++ ; 
        }
        count = 0 ; 
        for(int index_y = oty; index_y <=oby; index_y++){
            for(int index_x = 0;  index_x < w; index_x++){ 
                    chosen[count].push_back(board[index_y][index_x]);    
                }
                count ++ ; 
            }
        int cnt = 0;
        for ( int index_x=0 ; index_x<=obx; index_x++){
            if ( index_x < otx){
                cnt= 0 ; 
                for ( int index_y=oty; index_y<=oby; index_y++ ){
                    board[index_y][index_x]=rem[cnt][index_x] ; 
                    cnt ++ ;   
                }
            }
            else {
                cnt = 0 ; 
                for ( int index_y=oty; index_y<=oby; index_y++){
                    board[index_y][index_x]= chosen[cnt][index_x-otx] ; 
                    cnt ++ ; 
                }
            }
        }
    }
    Board res;
    res.n = n;
    res.m = m;
    res.pieces = board;
    return res;
}

Board reverseTypeII(int X, int Y, int size, int s, int n, int m, vector<vector<int>> board)
{
    int otx = std::max(X, 0);
    int oty = std::max(Y, 0);
    int obx = std::min(X + size - 1, m - 1);
    int oby = std::min(Y + size - 1, n - 1);
    int w = obx - otx + 1;
    int h = oby - oty + 1;

    int chosenRowNum = h / 2;
    bool first = (h + 1) % 2;

    if(is_inside(X, Y, n, m, 2)){
        if(!first) chosenRowNum++;
        first = 1 ; 
    }
    else if(abs(Y) % 2 != 0){
        first = 0 ;
    }
    else if(abs(Y) % 2 == 0){
        if(h % 2 == 1){
            first = 1;
            chosenRowNum++;
        }
    }

    // cout << "otx:" << otx << " oty:" << oty << " obx:" << obx << " oby:" << oby << " w:" << w << " h:" << h << " chosenRowNum:" << chosenRowNum << " first:" << first << '\n';

    std::vector<int> rem[256], chosen[128];

    if(s == 0){

        // cout << "REM\n";
        int rem_cnt= 0 , chosen_cnt= 0 ; 
        for(int index_y = oty; index_y < n - chosenRowNum; index_y++){
            rem[index_y - oty].clear();
            // cout << "index:" << index_y - oty << '\n';
            for(int index_x = otx; index_x <= obx; index_x++){  
                rem[index_y - oty].push_back(board[index_y][index_x]);
                // cout << board[index_y][index_x] << ' ';
            }
            rem_cnt++ ; 
            // cout << '\n';
            
        }

        // cout << "CHOSEN\n";
        for(int index_y = n - chosenRowNum ; index_y < n; index_y++){
            chosen[index_y - (n - chosenRowNum)].clear();
            // cout << "index:" << index_y - (n - chosenRowNum) << '\n';
            for(int index_x = otx; index_x <= obx; index_x++){  
                chosen[index_y - (n - chosenRowNum)].push_back(board[index_y][index_x]);
                // cout << board[index_y][index_x] << ' ';
            }
            chosen_cnt++ ; 
            // cout << '\n';       
        }

        int cnt = 0;
        for(int index_y = oty; index_y < min((oty + chosenRowNum * 2), n); index_y++){
            if(first){
                for(int index_x = otx; index_x <= obx; index_x++){  
                    if ( chosen_cnt>(cnt / 2)){
                        board[index_y][index_x] = chosen[(cnt / 2)][index_x - otx];
                    }
                   
                }
                first = 0;
            }else{
                for(int index_x = otx; index_x <= obx; index_x++){  
                    if ( rem_cnt>cnt / 2){
                        board[index_y][index_x] = rem[(cnt / 2)][index_x - otx];
                    }
                    
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

        // cout << "CHOSEN\n";
        int chosen_cnt = 0 ;
        for(int index_y = 0; index_y < chosenRowNum; index_y++){
            chosen[index_y].clear();
            // cout << "index:" << index_y << '\n';
            for(int index_x = otx; index_x <= obx; index_x++){
                chosen[index_y].push_back(board[index_y][index_x]);
                // cout << board[index_y][index_x] << ' ';    
            }
            chosen_cnt++ ;
            // cout << '\n';
        }

        // cout << "REM\n";
        int rem_cnt= 0 ; 
        for(int index_y = chosenRowNum ; index_y <= oby; index_y++){
            rem[index_y - chosenRowNum].clear();
            // cout << "index:" << index_y - chosenRowNum << '\n';
            for(int index_x = otx; index_x <= obx; index_x++){ 
                rem[index_y - chosenRowNum].push_back(board[index_y][index_x]);
                // cout << board[index_y][index_x] << ' ';
            }
            rem_cnt++;
            // cout << '\n';
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
                    if ( chosen_cnt>(cnt / 2)){
                        board[index_y][index_x] = chosen[(cnt / 2)][index_x - otx];
                    }          
                }
                first = 0;
            }else{
                for(int index_x = otx; index_x <= obx; index_x++){  
                    if (rem_cnt> ((cnt / 2) + oty)){
                        board[index_y][index_x] = rem[((cnt / 2) + oty)][index_x - otx];
                    }
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
    if (s==2 ) {
        int count = 0 ; 
        for(int index_y = ((first!=0 )? oty: oty+1); index_y <= oby; index_y+=2){
            for(int index_x = otx; index_x < m-w; index_x++){       
                         
                rem[count].push_back(board[index_y][index_x]);
            }  
            count ++ ; 
        }
       
        count = 0 ; 
        for(int index_y = ((first!=0 )? oty: oty+1); index_y <=oby; index_y+=2){
             for(int index_x = m-w;  index_x < m; index_x++){ 
                chosen[count].push_back(board[index_y][index_x]);         
            }
            count ++ ; 
        }
        int cnt = 0;
        for ( int index_x=otx ; index_x<m ; index_x++){
            if ( index_x <=obx ){
                cnt= 0 ; 
                for ( int index_y=((first!=0 )? oty: oty+1); index_y<=oby; index_y+=2 ){
                    board[index_y][index_x]=chosen[cnt][index_x-otx] ; 
                    cnt ++ ;   
                }
            }
            else {
                cnt = 0 ; 
                for ( int index_y=((first!=0 ) ? oty: oty+1) ; index_y<=oby; index_y+=2 ){
                    board[index_y][index_x]= rem[cnt][index_x -(obx+1)] ; 
                  
                    cnt ++ ; 
                }
            }
        }
           
    }
    if (s==3) { 
        int count = 0 ; 
        for(int index_y = ((first!=0 ) ? oty: oty+1); index_y <=oby; index_y+=2){
            for(int index_x = w; index_x <=obx; index_x++){                        
                rem[count].push_back(board[index_y][index_x]);
            }  
            count ++ ; 
        }
        count = 0 ; 
        for(int index_y = ((first!=0 ) ? oty: oty+1); index_y <=oby; index_y+=2){
            for(int index_x = 0;  index_x < w; index_x++){ 
                    chosen[count].push_back(board[index_y][index_x]);    
                }
                count ++ ; 
            }
        int cnt = 0;
        for ( int index_x=0 ; index_x<=obx; index_x++){
            if ( index_x < otx){
                cnt= 0 ; 
                for ( int index_y=((first!=0 ) ? oty: oty+1) ; index_y<=oby; index_y+=2 ){
                    board[index_y][index_x]=rem[cnt][index_x] ; 
                    cnt ++ ;   
                }
            }
            else {
                cnt = 0 ; 
                for ( int index_y=((first!=0 ) ? oty: oty+1); index_y<=oby; index_y+=2 ){
                    board[index_y][index_x]= chosen[cnt][index_x-otx] ; 
                    cnt ++ ; 
                }
            }
        }
    }
    Board res;
    res.n = n;
    res.m = m;
    res.pieces = board;
    return res;
}
 
Board reverseTypeIII(int X, int Y, int size, int s, int n, int m, std::vector<std::vector<int>> board)
{
    int otx = std::max(X, 0);
    int oty = std::max(Y, 0);
    int obx = std::min(X + size - 1, m - 1);
    int oby = std::min(Y + size - 1, n - 1);
    int w = obx - otx + 1;
    int h = oby - oty + 1;

    int chosenColNum = w / 2;
    bool first = (w + 1) % 2;

    // if((X + size - 1) >= m){
    //     if(!first) chosenColNum++;
    //     first = w % 2;
    // }
    if (is_inside(X, Y, n, m, 3)){
        if (!first) chosenColNum++; 
        first = 1;
    }else if(abs(X) % 2 != 0){
        first = 0 ;
    }else if(abs(X) % 2 == 0){
        if(w % 2 == 1){
            first = 1;
            chosenColNum++;
        }
    }
    

    

    vector<int> rem[256], chosen[128];
    if (s == 0) {
        for(int index_y = oty; index_y < n-h; index_y++){
            for(int index_x = ((first!=0)? otx : otx+1); index_x <= obx; index_x+=2){                        
                rem[index_y-(oty)].push_back(board[index_y][index_x]);
            }  
        }
        for(int index_y = n-h; index_y < n; index_y++){
            for(int index_x = ((first!=0) ? otx : otx+1);  index_x <= obx; index_x+=2){ 
                chosen[index_y-(n-h)].push_back(board[index_y][index_x]);
            }                 
        }
        int cnt = 0;
        for ( int index_y=oty ; index_y<n ; index_y++){
            if ( index_y<oty+h){
                cnt= 0 ; 
                for ( int index_x=((first!=0) ? otx : otx+1) ; index_x<=obx; index_x+=2 ){
                    board[index_y][index_x]=chosen[index_y-oty][cnt] ; 
                    cnt ++ ; 
                }
            }
            else {
                cnt = 0 ; 
                for ( int index_x=((first!=0) ? otx : otx+1) ; index_x<=obx; index_x+=2 ){
                    board[index_y][index_x]=rem[index_y-(oty+h)][cnt] ; 
                    cnt ++ ; 
                }
            }
        }  
    }

    if (s == 1) { 
        for(int index_y = h; index_y<=oby; index_y++){
            for(int index_x = ((first!=0) ? otx : otx+1); index_x<=obx; index_x+=2){                        
                rem[index_y-(h)].push_back(board[index_y][index_x]); 
            }   
        }
        for(int index_y = 0; index_y < h; index_y++){
            for(int index_x = ((first!=0) ? otx : otx+1);  index_x <= obx; index_x+=2){ 
                chosen[index_y].push_back(board[index_y][index_x]); 
            }                
        }
        int cnt = 0;
        for ( int index_y=0 ; index_y<=oby ; index_y++){
            if ( index_y< oby-h+1){
                cnt= 0 ; 
                for ( int index_x=((first!=0) ? otx : otx+1) ; index_x<=obx; index_x+=2 ){
                    board[index_y][index_x]=rem[index_y][cnt] ; 
                    cnt ++ ; 
                }    
            }
            else {
                cnt = 0 ; 
                for ( int index_x=((first!=0)? otx : otx+1) ; index_x<=obx; index_x+=2 ){
                    board[index_y][index_x]=chosen[index_y-(oby-h+1)][cnt] ; 
                    cnt ++ ; 
                }
            }
        }  
    }
    if(s == 2){
        int rem_cnt= 0 , chosen_cnt= 0 ; 
        for(int index_x = otx; index_x < m - chosenColNum; index_x++){
            rem[index_x - otx].clear();
            // cout << "index:" << index_x - otx << '\n';
            for(int index_y = oty; index_y <= oby; index_y++){
                rem[index_x - otx].push_back(board[index_y][index_x]);
                // cout << board[index_y][index_x] << ' ';
            }
            rem_cnt++ ; 
            // cout << '\n';
        }

        // cout << "CHOSEN\n";
        for(int index_x = m - chosenColNum ; index_x < m; index_x++){
            chosen[index_x - (m - chosenColNum)].clear();
            // cout << "index:" << index_x - m + chosenColNum << '\n';
            for(int index_y = oty; index_y <= oby; index_y++){  
                chosen[index_x - (m - chosenColNum)].push_back(board[index_y][index_x]);
                // cout << board[index_y][index_x] << ' ';
            }
            chosen_cnt++ ; 
            // cout << '\n';
        }

        int cnt = 0;
        for(int index_x = otx; index_x < (otx + chosenColNum * 2); index_x++){
            if(first){
                for(int index_y = oty; index_y <= oby; index_y++){  
                    if ( chosen_cnt>(cnt / 2)){
                        board[index_y][index_x] = chosen[(cnt / 2)][index_y - oty];
                    }
                }
                first = 0;
            }else{
                for(int index_y = oty; index_y <= oby; index_y++){  
                    if ( rem_cnt>(cnt / 2)){
                        board[index_y][index_x] = rem[(cnt / 2)][index_y - oty];
                    }
                    
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
        // cout << "CHOSEN\n";
        int rem_cnt= 0 , chosen_cnt= 0 ; 
        for(int index_x = 0; index_x < chosenColNum; index_x++){
            chosen[index_x].clear();
            // cout << "index:" << index_x << '\n';
            for(int index_y = oty; index_y <= oby; index_y++){
                chosen[index_x].push_back(board[index_y][index_x]);
                // cout << chosen[index_x][index_y - oty] << ' ';
            }
            chosen_cnt++ ;
            // cout << '\n';
        }
        // REMAINING
        // cout << "REM\n";
        for(int index_x = chosenColNum ; index_x <= obx; index_x++){
            rem[index_x - chosenColNum].clear();
            // cout << "index:" << index_x - chosenColNum << '\n';
            for(int index_y = oty; index_y <= oby; index_y++){  
                rem[index_x - chosenColNum].push_back(board[index_y][index_x]);
                
                // cout << rem[index_x - chosenColNum][index_y - oty] << ' ';
            }
            rem_cnt++ ; 
            // cout << '\n';
        }

        // BUILD
        for(int index_x = 0; index_x < otx; index_x++){
            for(int index_y = oty; index_y <= oby; index_y++){
                board[index_y][index_x] = rem[index_x][index_y - oty];
            }
        }
        
        int cnt = 0;
        for(int index_x = otx; index_x < min((otx + chosenColNum * 2), m); index_x++){
            if(first){
                for(int index_y = oty; index_y <= oby; index_y++){  
                    if ( chosen_cnt>(cnt/2)){
                        board[index_y][index_x] = chosen[(cnt / 2)][index_y - oty];
                    }
                    
                }
                first = 0;
            }else{
                for(int index_y = oty; index_y <= oby; index_y++){  
                    if (rem_cnt>((cnt / 2) + otx)){
                        int ind = (cnt / 2) + otx;
                        board[index_y][index_x] = rem[ind][index_y - oty];
                    }   
                }
                first = 1;
            }
            cnt++;
        }
        int index = 0;
        for(int index_x = otx + chosenColNum * 2; index_x <= obx; index_x++){
            for(int index_y = oty; index_y <= oby; index_y++){
                board[index_y][index_x] = rem[index + chosenColNum + otx][index_y - oty];
            }
            index++;
        }
    }
    Board res;
    res.n = n;
    res.m = m;
    res.pieces = board;
    return res;
}

Board reverse(Board bord, Action a) {
    // Setup
    std::vector<std::vector<int>> cboard = bord.pieces;
    int n = bord.n;
    int m = bord.m;
    int power = ceil(double(a.die_index) / 3.0);
    int size = int(pow(2, power));
    int dice_type;

    if (power == 0) {
        dice_type = 1;
    } else {
        dice_type = a.die_index - ((power - 1) * 3);
    }

    // Determine the dice type and call the corresponding reverse function
    if (dice_type == 1) {
        return reverseTypeI(a.x, a.y, size, a.dir, n, m, cboard);
    } else if (dice_type == 2) {
        return reverseTypeII(a.x, a.y, size, a.dir, n, m, cboard);
    } else {
        return reverseTypeIII(a.x, a.y, size, a.dir, n, m, cboard);
    }
}


// DEPRECATED: Use only as a fallback when the API fails
std::string ReadJsonFile(const std::string& file_path);

// Function to parse json data into a game state
GameState ParseJson(const std::string& problem_json);

// Function to output json using the game state
std::string OutputJson(const GameState& game_state);

#endif // TYPES_H