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
#include <math.h>

using namespace std;

char my_array[1001][1001];
char my_compare[1001][1001];

int main() {
    ifstream MapFile;
    MapFile.open("floor.data");
    ifstream InputFile;
    InputFile.open("final.path");
    
    int row = 0, col = 0;
    int battery = 0;

    MapFile>>row>>col>>battery;
    for(int i = 0; i < row; i++) {
        for(int j = 0; j < col; j++) {
            MapFile>>my_array[i][j];
        }
    }
    MapFile.close();
    
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
    
    int firstX = 0, firstY = 0;
    int secondX = 0, secondY = 0;
    int my_battery = battery;
    for(int i = 0; i < count; i++) {
        InputFile>>x>>y;
        if(i==0) {
            bx = x;
            by = y;
            firstX = x;
            firstY = y;
        }
        else {
            firstX = secondX;
            firstY = secondY;
        }
        secondX = x;
        secondY = y;
        
        my_compare[x][y] = '0';
        if(i!=0) my_battery--;
        
        //check distance
        if((abs(firstX-secondX) + abs(firstY-secondY)!=1) && i!=0) {
            cout<<"distance is wrong"<<endl;
            cout<<firstX<<" "<<firstY<<endl;
            cout<<secondX<<" "<<secondY<<endl;
        }
        
        //check battery
        if(my_battery<0) {
            cout<<"low battery"<<endl;
            cout<<my_battery<<" "<<x<<" "<<y<<endl;
        }
        
        if(x==bx && y==by) {
            my_battery = battery;
        }
        
    }
    my_compare[bx][by] = 'R';
    InputFile.close();
    
    //check last position
    if(secondX!=bx && secondY!=by) {
        cout<<"Not returned"<<endl;
        cout<<secondX<<" "<<secondY<<endl;
    }
    
    /*
    for(int i = 0; i < row; i++) {
        for(int j = 0; j < col; j++) {
            cout<<my_compare[i][j];
        }
        cout<<endl;
    }
    */
    
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
