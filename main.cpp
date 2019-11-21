//
//  main.cpp
//  mini project 2
//
//  Created by Ivan Lim on 08/11/2019.
//  Copyright © 2019 Ivan Lim. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>

#define MAX_INPUT_NUMBER 1010

using namespace std;

int counts = 0;
int num = 0;
ifstream InputFile;
ofstream OutputFile;

class Grid {
public:
    int x;
    int y;
};

class Map {
public:
    Map(int Row = 0, int Col = 0, int Battery = 0);
    ~Map();
    int row, col;
    int battery;
    
    char **array;
    int **minStep;
    bool **visited;
    Grid *path;
    int currentPathNum;
    int maxPathNum;
    bool **shortestPath;
    int pathCount;
    
    int batteryX, batteryY;
    int curX, curY;
    int myBattery;
    int remaining;
    int direction;
    
    void printMap();
    void BFS();
    void BFSUtil(int x, int y, int steps, int *queueX, int *queueY);
    void walk();
    void charge();
    void WalkNext();
    void FindNext();
    int FindNextUtil(int x, int y, Grid *queue);
    void ConstructPath(int x, int y);
    void ConstructPathUtil(int x, int y, int steps, int *queueX, int *queueY, int **minimumStep);
    void back();
};

int main(void) {
    InputFile.open("floor.data");
    OutputFile.open("output.data");
    
    int row = 0, col = 0;
    int battery = 0;
    
    InputFile>>row>>col>>battery;
    Map myMap(row, col, battery);
    
    for(int i = 0; i < row; i++) {
        for(int j = 0; j < col; j++) {
            InputFile>>myMap.array[i][j];
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
    InputFile.close();
    myMap.BFS();
    //myMap.printMap();
    
    counts = 0;
    OutputFile<<myMap.curX<<" "<<myMap.curY<<" "<<endl;
    myMap.visited[myMap.curX][myMap.curY] = true;
    while(myMap.remaining>0) {
        //cout<<myMap.curX<<" "<<myMap.curY<<" "<<endl;
        //if(myMap.myBattery<0)
        //cout<<"minstep: "<<myMap.minStep[myMap.curX][myMap.curY]<<" "<<"battery: "<<myMap.myBattery<<" remaining: "<<myMap.remaining<<endl;
        //myMap.printMap();
        myMap.walk();
    }
    //OutputFile<<"----back----"<<endl;
    myMap.back();
    //cout<<counts<<endl;
    
    //myMap.printMap();
    OutputFile.close();
    InputFile.open("output.data");
    OutputFile.open("final.path");
    OutputFile<<counts<<endl;
    string line;
    if(InputFile.is_open()) {
        while(!InputFile.eof()) {
            getline(InputFile, line);
            OutputFile<<line<<endl;
        }
        InputFile.close();
    }
    else {
        cout<<"Cannot open Input"<<endl;
    }
    OutputFile.close();
    return 0;
}

Map::~Map() {
    for(int i = 0; i < row; i++) {
        delete [](array[i]);
        delete [](minStep[i]);
        delete [](visited[i]);
        delete [](shortestPath[i]);
    }
    delete []array;
    delete []minStep;
    delete []visited;
    delete []shortestPath;
}

Map::Map(int Row, int Col, int Battery) {
    row = Row;
    col = Col;
    battery = Battery;
    myBattery = battery;
    remaining = 0;
    direction = 0;
    currentPathNum = 0;
    maxPathNum = 0;
    pathCount = 0;
    array = new char*[row];
    minStep = new int*[row];
    visited = new bool*[row];
    shortestPath = new bool*[row];
    for(int i = 0; i < row; i++) {
        array[i] = new char[col];
        minStep[i] = new int[col];
        visited[i] = new bool[col];
        shortestPath[i] = new bool[col];
    }
    
    for(int i = 0; i < row; i++) {
        for(int j = 0; j < col; j++) {
            array[i][j] = 0;
            minStep[i][j] = -1;
            visited[i][j] = false;
            shortestPath[i][j] = false;
        }
    }
    
    
}

void Map::printMap() {
    /*
    for(int i = 0; i < row; i++) {
        for(int j = 0; j < col; j++) {
            cout<<array[i][j];
        }
        cout<<endl;
    }
    cout<<endl;
    */
    
    for(int i = 0; i < row; i++) {
        for(int j = 0; j < col; j++) {
            if(minStep[i][j]>-1 && minStep[i][j]<10) cout<<" ";
            cout<<" "<<minStep[i][j];
        }
        cout<<endl;
    }
    
    /*
    for(int i = 0; i < row; i++) {
        for(int j = 0; j < col; j++) {
            if(visited[i][j]) {
                OutputFile<<" 1";
            }
            else  {
                OutputFile<<" 0";
            }
        }
        OutputFile<<endl;
    }
    OutputFile<<endl;
    */
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
    //cout<<steps;
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

void Map::walk() {
    visited[curX][curY] = true;
    if(curX==batteryX && curY==batteryY) {
        //cout<<"-----Full----"<<endl;
        maxPathNum = 0;
        myBattery = battery;
        direction = 1;
    }
    if(myBattery<=minStep[curX][curY]) {
        //OutputFile<<"-----Charging----"<<endl;
        charge();
    }
    else {
        WalkNext();
    }
    OutputFile<<curX<<" "<<curY<<" "<<endl;
}

void Map::charge() {
    int nextX = -1, nextY = -1;
    if(nextX==-1 && nextY==-1) {
        //visit floor with minStep
        if(minStep[curX][curY-1]<minStep[curX][curY] && !visited[curX][curY-1] && minStep[curX][curY-1]!=-1) {
            nextX = curX;
            nextY = curY-1;
        }
        else if(minStep[curX-1][curY]<minStep[curX][curY] && !visited[curX-1][curY] && minStep[curX-1][curY]!=-1) {
            nextX = curX-1;
            nextY = curY;
        }
        else if(minStep[curX][curY+1]<minStep[curX][curY] && !visited[curX][curY+1] && minStep[curX][curY+1]!=-1) {
            nextX = curX;
            nextY = curY+1;
        }
        else if(minStep[curX+1][curY]<minStep[curX][curY] && !visited[curX+1][curY] && minStep[curX+1][curY]!=-1) {
            nextX = curX+1;
            nextY = curY;
        }
        
        //nearby floors have been visted
        //visit the floor with minimum steps
        if(nextX==-1 && nextY==-1) {
            int min = minStep[curX][curY];
            if(minStep[curX][curY+1]<min && minStep[curX][curY+1]!=-1) {
                min = minStep[curX][curY+1];
                nextX = curX;
                nextY = curY+1;
            }
            if(minStep[curX+1][curY]<min && minStep[curX+1][curY]!=-1) {
                min = minStep[curX+1][curY];
                nextX = curX+1;
                nextY = curY;
            }
            if(minStep[curX][curY-1]<min && minStep[curX][curY-1]!=-1) {
                min = minStep[curX][curY-1];
                nextX = curX;
                nextY = curY-1;
            }
            if(minStep[curX-1][curY]<min && minStep[curX-1][curY]!=-1) {
                min = minStep[curX-1][curY];
                nextX = curX-1;
                nextY = curY;
            }
        }
    }
    curX = nextX;
    curY = nextY;
    
    if(!visited[curX][curY]) remaining--;
    myBattery--;
    counts++;
}

void Map::WalkNext() {
    int nextX = -1, nextY = -1;
    if(curY!=0 && minStep[curX][curY-1]>minStep[curX][curY] && !visited[curX][curY-1]) {
        nextX = curX;
        nextY = curY-1;
        direction = 0;
    }
    else if(curX!=0 && minStep[curX-1][curY]>minStep[curX][curY] && !visited[curX-1][curY]) {
        nextX = curX-1;
        nextY = curY;
        direction = 0;
    }
    else if(curY != col-1 && minStep[curX][curY+1]>minStep[curX][curY] && !visited[curX][curY+1]) {
        nextX = curX;
        nextY = curY+1;
        direction = 0;
    }
    else if(curX != row-1 && minStep[curX+1][curY]>minStep[curX][curY] && !visited[curX+1][curY]) {
        nextX = curX+1;
        nextY = curY;
        direction = 0;
    }
    
    if(nextX==-1 && nextY==-1) {
        //all furthest floors have visited
        if(curY!=0 && minStep[curX][curY-1]<minStep[curX][curY] && !visited[curX][curY-1] && minStep[curX][curY-1]!=-1) {
            nextX = curX;
            nextY = curY-1;
            direction = 0;
        }
        else if(curX!=0 && minStep[curX-1][curY]<minStep[curX][curY] && !visited[curX-1][curY] && minStep[curX-1][curY]!=-1) {
            nextX = curX-1;
            nextY = curY;
            direction = 0;
        }
        else if(curY!=col-1 && minStep[curX][curY+1]<minStep[curX][curY] && !visited[curX][curY+1] && minStep[curX][curY+1]!=-1) {
            nextX = curX;
            nextY = curY+1;
            direction = 0;
        }
        else if(curX!=row-1 && minStep[curX+1][curY]<minStep[curX][curY] && !visited[curX+1][curY] && minStep[curX+1][curY]!=-1) {
            nextX = curX+1;
            nextY = curY;
            direction = 0;
        }
        
        /*
        //nearby floors have been visted
        if(nextX==-1 && nextY==-1) {
            remaining++;
            
            if(direction==1) {
                //after charging go back
                if(curY != col-1 && minStep[curX][curY+1]>minStep[curX][curY] && visited[curX][curY+1] && minStep[curX][curY+1]!=-1) {
                    nextX = curX;
                    nextY = curY+1;
                }
                else if(curX!=0 && minStep[curX-1][curY]>minStep[curX][curY] && visited[curX-1][curY] && minStep[curX-1][curY]!=-1) {
                    nextX = curX-1;
                    nextY = curY;
                }
                else if(curY!=0 && minStep[curX][curY-1]>minStep[curX][curY] && visited[curX][curY-1] && minStep[curX][curY-1]!=-1) {
                    nextX = curX;
                    nextY = curY-1;
                }
                else if(curX != row-1 && minStep[curX+1][curY]>minStep[curX][curY] && visited[curX+1][curY] && minStep[curX+1][curY]!=-1) {
                    nextX = curX+1;
                    nextY = curY;
                }
            }
            else {
                //visit floors with min step
                int min = minStep[curX][curY];
                if(curY!=col-1 && minStep[curX][curY+1]<min && minStep[curX][curY+1]!=-1) {
                    min = minStep[curX][curY+1];
                    nextX = curX;
                    nextY = curY+1;
                }
                if(curX!=row-1 && minStep[curX+1][curY]<min && minStep[curX+1][curY]!=-1) {
                    min = minStep[curX+1][curY];
                    nextX = curX+1;
                    nextY = curY;
                }
                if(curY!=0 && minStep[curX][curY-1]<min && minStep[curX][curY-1]!=-1) {
                    min = minStep[curX][curY-1];
                    nextX = curX;
                    nextY = curY-1;
                }
                if(curX!=0 && minStep[curX-1][curY]<min && minStep[curX-1][curY]!=-1) {
                    min = minStep[curX-1][curY];
                    nextX = curX-1;
                    nextY = curY;
                }
            }
         
        }
         */
        
    }
    if(maxPathNum) {
        //SetDestination(path[cur]);
        if(currentPathNum<maxPathNum) {
            nextX = path[currentPathNum].x;
            nextY = path[currentPathNum].y;
            currentPathNum++;
        }
    }
    
    if(nextX==-1 && nextY==-1) {
        if(maxPathNum!=0) delete []path;
        FindNext();
        nextX = path[currentPathNum].x;
        nextY = path[currentPathNum].y;
        currentPathNum++;
    }
    
    if(minStep[nextX][nextY]>=myBattery) {
        charge();
        return;
    }
    else {
        curX = nextX;
        curY = nextY;
    }
    
    if(!visited[curX][curY])remaining--;
    myBattery--;
    counts++;
}

void Map::ConstructPathUtil(int x, int y, int steps, int *queueX, int *queueY, int **minimumStep) {
    if(x>=0 && y>=0 && x<row && y<col) {
        if((array[x][y]=='0' || array[x][y]=='R') && minimumStep[x][y]==-1) {
            minimumStep[x][y] = steps;
            queueX[pathCount] = x;
            queueY[pathCount] = y;
            pathCount++;
        }
    }
}

void Map::ConstructPath(int x, int y) {
    pathCount = 0;
    
    int **minimumStep = new int*[row];
    for(int i = 0; i < row; i++) {
        minimumStep[i] = new int[col];
        for(int j = 0; j < col; j++) {
            minimumStep[i][j] = -1;
        }
    }
    
    int *queueX = new int[row*col];
    int *queueY = new int[row*col];
    int steps = 0;
    int stop = 0;
    minimumStep[curX][curY] = steps;
    queueX[pathCount] = curX;
    queueY[pathCount] = curY;
    pathCount++;
    
    int current = 0;
    for(steps = 1; pathCount != stop; steps++) {
        stop = pathCount;
        for(; current < stop; current++) {
            ConstructPathUtil(queueX[current]+1, queueY[current], steps, queueX, queueY, minimumStep);
            ConstructPathUtil(queueX[current]-1, queueY[current], steps, queueX, queueY, minimumStep);
            ConstructPathUtil(queueX[current], queueY[current]+1, steps, queueX, queueY, minimumStep);
            ConstructPathUtil(queueX[current], queueY[current]-1, steps, queueX, queueY, minimumStep);
        }
    }
    delete []queueX;
    delete []queueY;
    
    path = new Grid[minimumStep[x][y]+1];
    currentPathNum = 1;
    maxPathNum = minimumStep[x][y]+1;
    while(minimumStep[x][y]) {
        path[minimumStep[x][y]].x = x;
        path[minimumStep[x][y]].y = y;
        if(y!=0 && minimumStep[x][y-1]!=-1 && minimumStep[x][y-1]<minimumStep[x][y]) y--;
        else if(x!=0 && minimumStep[x-1][y]!=-1 && minimumStep[x-1][y]<minimumStep[x][y]) x--;
        else if(y!=col-1 && minimumStep[x][y+1]!=-1 && minimumStep[x][y+1]<minimumStep[x][y]) y++;
        else if(x!=row-1 && minimumStep[x+1][y]!=-1 && minimumStep[x+1][y]<minimumStep[x][y]) x++;
    }
    
    for(int i = 0; i < row; i++) {
        delete [](minimumStep[i]);
    }
    delete []minimumStep;
}

void Map::FindNext() {
    Grid *queue = new Grid[row*col];
    for(int i = 0; i < row; i++) {
        for(int j = 0; j < col; j++) {
            shortestPath[i][j] = false;
        }
    }
    num = 0;
    int stop = 0;
    queue[num].x = curX;
    queue[num].y = curY;
    num++;
    
    int current = 0;
    while(num != stop) {
        stop = num;
        for(; current < stop; current++) {
            //cout<<"current queue: "<<queue[current].x<<" "<<queue[current].y<<endl;
            int finish = 0;
            finish = FindNextUtil(queue[current].x-1, queue[current].y, queue);
            if(finish) goto End;
            finish = FindNextUtil(queue[current].x, queue[current].y-1, queue);
            if(finish) goto End;
            finish = FindNextUtil(queue[current].x, queue[current].y+1, queue);
            if(finish) goto End;
            finish = FindNextUtil(queue[current].x+1, queue[current].y, queue);
            if(finish) goto End;
        }
    }
End:;
    //cout<<"Find Next:"<<queue[num].x<<" "<<queue[num].y<<endl;;
    ConstructPath(queue[num].x, queue[num].y);
    delete []queue;
}

int Map::FindNextUtil(int x, int y, Grid *queue) {
    if(x>=0 && y>=0 && x<row && y<col) {
        if((array[x][y]=='0' || array[x][y]=='R')&& !shortestPath[x][y]) {
            shortestPath[x][y] = true;
            queue[num].x = x;
            queue[num].y = y;
            if(!visited[x][y]) return 1;
            num++;
        }
    }
    return 0;
}

void Map::back() {
    while(minStep[curX][curY]) {
        if(curY!=0 && minStep[curX][curY-1]!=-1 && minStep[curX][curY-1]<minStep[curX][curY]) curY--;
        else if(curX!=0 && minStep[curX-1][curY]!=-1 && minStep[curX-1][curY]<minStep[curX][curY]) curX--;
        else if(curY!=col-1 && minStep[curX][curY+1]!=-1 && minStep[curX][curY+1]<minStep[curX][curY]) curY++;
        else if(curX!=row-1 && minStep[curX+1][curY]!=-1 && minStep[curX+1][curY]<minStep[curX][curY]) curX++;
        OutputFile<<curX<<" "<<curY<<" "<<endl;
        counts++;
        if(!visited[curX][curY]) visited[curX][curY] = true;
    }
}
