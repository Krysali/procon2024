#include <iostream>
#include <vector>
using namespace std;

int n, m;
int board[6][6];
// die-type_1 reverse action 
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



// die-type_2 reverse action 
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

    if (s==2) { 
        int count = 0 ; 
        for(int index_y = oty; index_y <=oby; index_y+=2){
            for(int index_x = w; index_x <=obx; index_x++){                        
                rem[count].push_back(board[index_y][index_x]);
            }  
            count ++ ; 
            cout<< endl; 
        }
        count = 0 ; 
        for(int index_y = oty; index_y <=oby; index_y+=2){
            for(int index_x = 0;  index_x < w; index_x++){ 
                    chosen[count].push_back(board[index_y][index_x]);    
                }
                count ++ ; 
            }
        int cnt = 0;
        for ( int index_x=0 ; index_x<=obx; index_x++){
            if ( index_x < otx){
                cnt= 0 ; 
                for ( int index_y=oty ; index_y<=oby; index_y+=2 ){
                    board[index_y][index_x]=rem[cnt][index_x] ; 
                    cnt ++ ;   
                }
            }
            else {
                cnt = 0 ; 
                for ( int index_y=oty ; index_y<=oby; index_y+=2 ){
                    board[index_y][index_x]= chosen[cnt][index_x-otx] ; 
                    cnt ++ ; 
                }
            }
        }
    }



    if (s==3 ) {
        int count = 0 ; 
        for(int index_y = oty; index_y <= oby; index_y+=2){
            for(int index_x = otx; index_x < n-w; index_x++){                        
                rem[count].push_back(board[index_y][index_x]);
                cout << board[index_y][index_x] << " " ; 
            }  
            cout << endl ; 
            count ++ ; 
        }
        cout << endl ; 
        count = 0 ; 
        for(int index_y = oty; index_y <=oby; index_y+=2){
             for(int index_x = n-w;  index_x < n; index_x++){ 
                chosen[count].push_back(board[index_y][index_x]);     
                cout << board[index_y][index_x]<< " " ;           
            }
            cout <<endl; 
            count ++ ; 
   
        }


        int cnt = 0;
        for ( int index_x=otx ; index_x<n ; index_x++){
            if ( index_x <=obx ){
                cnt= 0 ; 
                for ( int index_y=oty ; index_y<=oby; index_y+=2 ){
                    board[index_y][index_x]=chosen[cnt][index_x-otx] ; 
                    cnt ++ ; 
                    
                }
                
            }
            else {
                cnt = 0 ; 
                for ( int index_y=oty ; index_y<=oby; index_y+=2 ){
                    board[index_y][index_x]= rem[cnt][index_x -(obx+1)] ; 
                  
                    cnt ++ ; 
                }
            }
        }
           
    }
    
    


    return;
}
// die-type_3 reverse action 
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

    if (s==0 ) { 
        for(int index_y = size; index_y<=oby; index_y++){
            for(int index_x = otx; index_x<=obx; index_x+=2){                        
                rem[index_y-(size)].push_back(board[index_y][index_x]);
            }   
        }
        cout << endl; 
    
        for(int index_y = 0; index_y < size; index_y++){
            for(int index_x = otx;  index_x <= obx; index_x+=2){ 
                chosen[index_y].push_back(board[index_y][index_x]);   
            }                 
        }
        int cnt = 0;

        for ( int index_y=0 ; index_y<=oby ; index_y++){
            if ( index_y< oty ){
                cnt= 0 ; 
                for ( int index_x=otx ; index_x<=obx; index_x+=2 ){
                    board[index_y][index_x]=rem[index_y][cnt] ; 
                    cnt ++ ; 
                }
                
            }
            else {
                cnt = 0 ; 
                for ( int index_x=otx ; index_x<=obx; index_x+=2 ){
                    board[index_y][index_x]=chosen[index_y-oty][cnt] ; 
                    cnt ++ ; 
                }
            }
        }  
    }
    if (s==1) {
        for(int index_y = oty; index_y < n-size; index_y++){
            for(int index_x = otx; index_x <= obx; index_x+=2){                        
                rem[index_y-(oty)].push_back(board[index_y][index_x]);
            }  
         
        }
        for(int index_y = n-size; index_y < n; index_y++){
            for(int index_x = otx;  index_x <= obx; index_x+=2){ 
                chosen[index_y-(n-size)].push_back(board[index_y][index_x]); 
            }                 
        }
        int cnt = 0;
        int topY=Y ; 
        for ( int index_y=oty ; index_y<n ; index_y++){
            if ( index_y<=oby){
                cnt= 0 ; 
                for ( int index_x=otx ; index_x<=obx; index_x+=2 ){
                    board[index_y][index_x]=chosen[index_y-oty][cnt] ; 
                    cnt ++ ; 
                }
                
            }
            else {
                cnt = 0 ; 
                for ( int index_x=otx ; index_x<=obx; index_x+=2 ){
                    board[index_y][index_x]=rem[index_y-(oby+1)][cnt] ; 
                    cnt ++ ; 
                }
            }
        }  
    }

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
    int t = 10 ; 

    while ( t--){
        int size, X, Y, dir;
        cin >> X >> Y >> size>>  dir;
        display();
        reverseTypeIII(X, Y, size, dir);
        display();
    // reverseTypeIII(2, 2, 4, 2);
    // display();
    }
    

    return 0;
}
