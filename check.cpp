//
//  main.cpp
//  check
//
//  Created by Ivan Lim on 09/11/2019.
//  Copyright © 2019 Ivan Lim. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#define MAX_INPUT_NUMBER 10000

using namespace std;

char my_array[1001][1001];
char my_compare[1001][1001];

int main() {
    ifstream InputFile;
    InputFile.open("final.path");
    
    int row = 0, col = 0;
    int battery = 0;
    cin>>row>>col>>battery;
    for(int i = 0; i < row; i++) {
        for(int j = 0; j < col; j++) {
            cin>>my_array[i][j];
        }
    }
    
    for(int i = 0; i < row; i++) {
        for(int j = 0; j < col; j++) {
            my_compare[i][j] = '1';
        }
    }
    
    int x = 0, y = 0;
    int bx = 0, by = 0;
    
    int count = 0;
    InputFile>>count;
    count++;
    
    for(int i = 0; i < count; i++) {
        InputFile>>x>>y;

        if(i==0) {
            bx = x;
            by = y;
        }
        my_compare[x][y] = '0';
    }
    my_compare[bx][by] = 'R';
    InputFile.close();
    
    for(int i = 0; i < row; i++) {
        for(int j = 0; j < col; j++) {
            cout<<my_compare[i][j];
        }
        cout<<endl;
    }
    
    
    bool same = true;
    for(int i = 0; i < row && same; i++) {
        for(int j = 0; j < col && same; j++) {
            if(my_array[i][j]!=my_compare[i][j]) {
                same = false;
            }
        }
    }
    if(same) {
        cout<<"You are correct!";
    }
    else {
        cout<<"You are wrong!";
    }
    
    return 0;
}
