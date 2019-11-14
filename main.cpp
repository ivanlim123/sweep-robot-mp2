//
//  main.cpp
//  mini project 2
//
//  Created by Ivan Lim on 08/11/2019.
//  Copyright © 2019 Ivan Lim. All rights reserved.
//

#include <iostream>

using namespace std;

int counts = 0;

class Map {
public:
    Map(int Row = 0, int Col = 0, int Battery = 0);
    int row, col;
    int battery;
    
    char **array;
    int **minStep;
    bool **visited;
    
    int batteryX, batteryY;
    int curX, curY;
    int myBattery;
    int remaining;
    
    void printMap();
    void BFS();
    void BFSUtil(int x, int y, int steps, int *queueX, int *queueY);
};

int main(void) {
    int row = 0, col = 0;
    int battery = 0;
    cin>>row>>col>>battery;
    Map myMap(row, col, battery);
    
    for(int i = 0; i < row; i++) {
        for(int j = 0; j < col; j++) {
            cin>>myMap.array[i][j];
            // find starting point
            if(myMap.array[i][j] == 'R') {
                myMap.batteryX = i;
                myMap.batteryY = j;
            }
            if(myMap.array[i][j] == '0') {
                myMap.remaining++;
            }
        }
    }
    myMap.BFS();
    myMap.printMap();
    
    return 0;
}

Map::Map(int Row, int Col, int Battery) {
    row = Row;
    col = Col;
    battery = Battery;
    remaining = 0;
    array = new char*[row];
    minStep = new int*[row];
    for(int i = 0; i < row; i++) {
        array[i] = new char[col];
        minStep[i] = new int[col];
    }
    
    for(int i = 0; i < row; i++) {
        for(int j = 0; j < col; j++) {
            array[i][j] = 0;
            minStep[i][j] = -1;
        }
    }
}

void Map::printMap() {
    for(int i = 0; i < row; i++) {
        for(int j = 0; j < col; j++) {
            cout<<array[i][j];
        }
        cout<<endl;
    }
    cout<<endl;
    
    for(int i = 0; i < row; i++) {
        for(int j = 0; j < col; j++) {
            if(minStep[i][j]>-1 && minStep[i][j]<10) cout<<" ";
            cout<<" "<<minStep[i][j];
        }
        cout<<endl;
    }
}

void Map::BFS() {
    int *queueX = new int[row*col];
    int *queueY = new int[row*col];
    int steps = 0;
    int stop = 0;
    curX = batteryX;
    curY = batteryY;
    minStep[curX][curY] = steps;
    queueX[counts] = curX;
    queueY[counts] = curY;
    counts++;
    
    int current = 0;
    for(steps = 1; counts != stop; steps++) {
        stop = counts;
        for(; current < stop; current++) {
            BFSUtil(queueX[current]+1, queueY[current], steps, queueX, queueY);
            BFSUtil(queueX[current]-1, queueY[current], steps, queueX, queueY);
            BFSUtil(queueX[current], queueY[current]+1, steps, queueX, queueY);
            BFSUtil(queueX[current], queueY[current]-1, steps, queueX, queueY);
        }
    }
    
    delete []queueX;
    delete []queueY;
}

void Map::BFSUtil(int x, int y, int steps, int *queueX, int *queueY) {
    if(x>=0 && y>=0 && x<row && y<col) {
        if(array[x][y]=='0' && minStep[x][y]==-1) {
            minStep[x][y] = steps;
            queueX[counts] = x;
            queueY[counts] = y;
            counts++;
        }
    }
}

