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
