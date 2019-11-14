//
//  main.cpp
//  mini project 2
//
//  Created by Ivan Lim on 08/11/2019.
//  Copyright © 2019 Ivan Lim. All rights reserved.
//

#include <iostream>

using namespace std;

class Map {
public:
    Map(int Row = 0, int Col = 0);
    int row, col;
    char **array;
};

int main(void) {
    int row = 0, col = 0;
    int battery = 0;
    cin>>row>>col>>battery;
    Map myMap(row, col);
    
    for(int i = 0; i < row; i++) {
        for(int j = 0; j < col; j++) {
            cin>>myMap.array[i][j];
        }
    }
    
    for(int i = 0; i < row; i++) {
        for(int j = 0; j < col; j++) {
            cout<<myMap.array[i][j];
        }
        cout<<endl;
    }
    
    return 0;
}

Map::Map(int Row, int Col) {
    row = Row;
    col = Col;
    array = new char*[row];
    for(int i = 0; i < row; i++) {
        array[i] = new char[col];
    }
    
    for(int i = 0; i < row; i++) {
        for(int j = 0; j < col; j++) {
            array[i][j] = 0;
        }
    }
}
