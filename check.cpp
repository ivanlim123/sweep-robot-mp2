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
    InputFile.open("output.data");
    
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
    
    string line;
    string command[MAX_INPUT_NUMBER];
    int index = 0;
    
    if(InputFile.is_open()) {
        while(!InputFile.eof()) {
            getline(InputFile, line);
            command[index++] = line;
        }
        InputFile.close();
    }
    else {
        cout<<"Cannot open Input"<<endl;
    }
    cout<<endl;
    
    for(int i = 0; i < index-1; i++) {
        string cmd = command[i];
        string first = "";
        string second = "";
        bool space = false;
        unsigned long len = cmd.length();
        for(unsigned long j = 0; j < len; j++) {
            char ch = cmd[j];
            if(ch==' ') {
                if(!space) {
                    space = true;
                }
            }
            if(!space) {
                first.push_back(ch);
            }
            else if(space) {
                second.push_back(ch);
            }
        }
        x = stoi(first);
        y = stoi(second);
        

        if(i==0) {
            bx = x;
            by = y;
        }
        my_compare[x][y] = '0';
    }
    my_compare[bx][by] = 'R';
    
    
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
