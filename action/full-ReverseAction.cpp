// Function to check if the point (x, y) is inside the boundaries
bool is_inside(int x, int y, int n, int m, int dtype) {
    if (dtype == 2) {
        return (x >= 0 && x < m);
    } else {
        return (y >= 0 && y < n);
    }
}

Board reverseTypeI(int X, int Y, int size, int s, int n, int m, vector<vector<int>> board)
{
    int otx = max(X, 0);
    int oty = max(Y, 0);
    int obx = min(X + size - 1, m - 1);
    int oby = min(Y + size - 1, n - 1);
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
    int otx = max(X, 0);
    int oty = max(Y, 0);
    int obx = min(X + size - 1, m - 1);
    int oby = min(Y + size - 1, n - 1);
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

    vector<int> rem[256], chosen[128];

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
 
Board reverseTypeIII(int X, int Y, int size, int s, int n, int m, vector<vector<int>> board)
{
    int otx = max(X, 0);
    int oty = max(Y, 0);
    int obx = min(X + size - 1, m - 1);
    int oby = min(Y + size - 1, n - 1);
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
    vector<vector<int>> cboard = bord.pieces;
    int n = bord.n;
    int m = bord.m;
    int power = ceil(double(a.diceNum) / 3.0);
    int size = int(pow(2, power));
    int dice_type;

    if (power == 0) {
        dice_type = 1;
    } else {
        dice_type = a.diceNum - ((power - 1) * 3);
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

