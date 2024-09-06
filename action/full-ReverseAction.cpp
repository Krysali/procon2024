#include <iostream>
#include <vector>
using namespace std;

int n, m;
int board[6][6];

bool isInside(int x, int y)
{
    if((x >= 0 && x < m) && (y >= 0 && y < n)) return 1;
    else return 0;
}

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

    if(isInside(X, Y)){
        if(!first) chosenRowNum++;
        first = h % 2;
    }else if(abs(Y) % 2 != 0){
        first = 0 ;
    }else if(abs(X) % 2 == 0){
        if(n % 2 == 1){
            first = 1;
            chosenRowNum++;
        }
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
        for(int index_y = oty; index_y < min((oty + chosenRowNum * 2), n); index_y++){
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
// 


    if (s==2 ) {
        int count = 0 ; 
        for(int index_y = ((oby-oty+1)%2==0 ? oty: oty+1); index_y <= oby; index_y+=2){
            for(int index_x = otx; index_x < n-w; index_x++){                        
                rem[count].push_back(board[index_y][index_x]);
                cout << board[index_y][index_x] << " " ; 
            }  
            cout << endl ; 
            count ++ ; 
        }
        cout << endl ; 
        count = 0 ; 
        for(int index_y = ((oby-oty+1)%2==0 ? oty: oty+1); index_y <=oby; index_y+=2){
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
                for ( int index_y=((oby-oty+1)%2==0 ? oty: oty+1); index_y<=oby; index_y+=2 ){
                    board[index_y][index_x]=chosen[cnt][index_x-otx] ; 
                    cnt ++ ; 
                    
                }
                
            }
            else {
                cnt = 0 ; 
                for ( int index_y=((oby-oty+1)%2==0 ? oty: oty+1) ; index_y<=oby; index_y+=2 ){
                    board[index_y][index_x]= rem[cnt][index_x -(obx+1)] ; 
                  
                    cnt ++ ; 
                }
            }
        }
           
    }
    if (s==3) { 
        int count = 0 ; 
        for(int index_y = ((oby-oty+1)%2==0 ? oty: oty+1); index_y <=oby; index_y+=2){
            for(int index_x = w; index_x <=obx; index_x++){                        
                rem[count].push_back(board[index_y][index_x]);
            }  
            count ++ ; 
            cout<< endl; 
        }
        count = 0 ; 
        for(int index_y = ((oby-oty+1)%2==0 ? oty: oty+1); index_y <=oby; index_y+=2){
            for(int index_x = 0;  index_x < w; index_x++){ 
                    chosen[count].push_back(board[index_y][index_x]);    
                }
                count ++ ; 
            }
        int cnt = 0;
        for ( int index_x=0 ; index_x<=obx; index_x++){
            if ( index_x < otx){
                cnt= 0 ; 
                for ( int index_y=((oby-oty+1)%2==0 ? oty: oty+1) ; index_y<=oby; index_y+=2 ){
                    board[index_y][index_x]=rem[cnt][index_x] ; 
                    cnt ++ ;   
                }
            }
            else {
                cnt = 0 ; 
                for ( int index_y=((oby-oty+1)%2==0 ? oty: oty+1); index_y<=oby; index_y+=2 ){
                    board[index_y][index_x]= chosen[cnt][index_x-otx] ; 
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

    // if((X + size - 1) >= m){
    //     if(!first) chosenColNum++;
    //     first = w % 2;
    // }
    if (isInside(X, Y)){
        if (!first) chosenColNum++; 
        first = 1;
    }else if(abs(X) % 2 != 0){
        first = 0 ;
    }else if(abs(X) % 2 == 0){
        if(m % 2 == 1){
            first = 1;
            chosenColNum++;
        }
    }

    cout << "otx:" << otx << " oty:" << oty << " obx:" << obx << " oby:" << oby << " w:" << w << " h:" << h << " chosenColNum:" << chosenColNum << " first:" << first << '\n';

    vector<int> rem[256], chosen[128];

    if (s == 0) { 
        for(int index_y = h; index_y<=oby; index_y++){
            for(int index_x = ((obx-otx+1)%2==0 ? otx : otx+1); index_x<=obx; index_x+=2){                        
                rem[index_y-(h)].push_back(board[index_y][index_x]);
                cout << board[index_y][index_x]<< " " ; 

            }   
        }
        for(int index_y = 0; index_y < h; index_y++){
            for(int index_x = ((obx-otx+1)%2==0 ? otx : otx+1);  index_x <= obx; index_x+=2){ 
                chosen[index_y].push_back(board[index_y][index_x]); 
                cout << board[index_y][index_x]<< " " ;  
            }                
        }
        int cnt = 0;
        for ( int index_y=0 ; index_y<=oby ; index_y++){
            if ( index_y< oby-h+1){
                cnt= 0 ; 
                for ( int index_x=((obx-otx+1)%2==0 ? otx : otx+1) ; index_x<=obx; index_x+=2 ){
                    board[index_y][index_x]=rem[index_y][cnt] ; 
                    cnt ++ ; 
                }    
            }
            else {
                cnt = 0 ; 
                for ( int index_x=((obx-otx+1)%2==0 ? otx : otx+1) ; index_x<=obx; index_x+=2 ){
                    board[index_y][index_x]=chosen[index_y-(oby-h+1)][cnt] ; 
                    cnt ++ ; 
                }
            }
        }  
    }
    if (s == 1) {
        for(int index_y = oty; index_y < n-h; index_y++){
            for(int index_x = ((obx-otx+1)%2==0 ? otx : otx+1); index_x <= obx; index_x+=2){                        
                rem[index_y-(oty)].push_back(board[index_y][index_x]);
            }  
            cout << endl; 
        }
        cout << "bandi" << endl; 
        for(int index_y = n-h; index_y < n; index_y++){
            for(int index_x = ((obx-otx+1)%2==0 ? otx : otx+1);  index_x <= obx; index_x+=2){ 
                cout << board[index_y][index_x]<< " ";
            }                 
        }
        int cnt = 0;
        for ( int index_y=oty ; index_y<n ; index_y++){
            if ( index_y<oty+h){
                cnt= 0 ; 
                for ( int index_x=((obx-otx+1)%2==0 ? otx : otx+1) ; index_x<=obx; index_x+=2 ){
                    board[index_y][index_x]=chosen[index_y-oty][cnt] ; 
                    cout << chosen[index_y-oty][cnt]<< "  ";
                    cnt ++ ; 
                }
            }
            else {
                cnt = 0 ; 
                for ( int index_x=((obx-otx+1)%2==0 ? otx : otx+1) ; index_x<=obx; index_x+=2 ){
                    board[index_y][index_x]=rem[index_y-(oty+h)][cnt] ; 
                    cnt ++ ; 
                }
            }
        }  
    }

// zuv avsan butsaaj hiihdee aldsan zuvhun (X =-1 baih uyd aldaa garj bn )
// X Y size dir 
// -1 0 8 2 
//  0  1  2  3  4  5 
//  6  7  8  9 10 11 
// 12 13 14 15 16 17 
// 18 19 20 21 22 23 
// 24 25 26 27 28 29 
// 30 31 32 33 34 35 

// 3  0  4  1  5  2
//  9  6 10  7 11  8
// 15 12 16 13 17 14
// 21 18 22 19 23 20
// 27 24 28 25 29 26 
// 33 30 34 31 35 32

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
// aldaa 
// 1 0 8 3 
//  0  1  2  3  4  5 
//  6  7  8  9 10 11 
// 12 13 14 15 16 17 
// 18 19 20 21 22 23 
// 24 25 26 27 28 29 
// 30 31 32 33 34 35 

//  2  3  0  4  1  5 
//  8  9  6 10  7 11
// 14 15 12 16 13 17
// 20 21 18 22 19 23
// 26 27 24 28 25 29 
// 32 33 30 34 31 35


    if(s == 3){
        // CHOSEN
        cout << "CHOSEN\n";
        for(int index_x = 0; index_x < chosenColNum; index_x++){
            chosen[index_x].clear();
            cout << "index:" << index_x << '\n';
            for(int index_y = oty; index_y <= oby; index_y++){
                chosen[index_x].push_back(board[index_y][index_x]);
                cout << chosen[index_x][index_y - oty] << ' ';
            }
            cout << '\n';
        }
        // REMAINING
        cout << "REM\n";
        for(int index_x = chosenColNum ; index_x <= obx; index_x++){
            rem[index_x - chosenColNum].clear();
            cout << "index:" << index_x - chosenColNum << '\n';
            for(int index_y = oty; index_y <= oby; index_y++){  
                rem[index_x - chosenColNum].push_back(board[index_y][index_x]);
                cout << rem[index_x - chosenColNum][index_y - oty] << ' ';
            }
            cout << '\n';
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
