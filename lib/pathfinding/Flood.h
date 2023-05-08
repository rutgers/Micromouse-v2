#include "API.h"
#include<stack>
#include<algorithm>

const int N = 16; 

static int maze[N][N] = 
{{14, 13, 12, 11, 10, 9, 8, 7, 7, 8, 9, 10, 11, 12, 13, 14},
 {13, 12, 11, 10,  9, 8, 7, 6, 6, 7, 8,  9, 10, 11, 12, 13},
 {12, 11, 10,  9,  8, 7, 6, 5, 5, 6, 7,  8,  9, 10, 11, 12},   
 {11, 10,  9,  8,  7, 6, 5, 4, 4, 5, 6,  7,  8,  9, 10, 11},   
 {10,  9,  8,  7,  6, 5, 4, 3, 3, 4, 5,  6,  7,  8,  9, 10},   
 {9,   8,  7,  6,  5, 4, 3, 2, 2, 3, 4,  5,  6,  7,  8,  9},   
 {8,   7,  6,  5,  4, 3, 2, 1, 1, 2, 3,  4,  5,  6,  7,  8},   
 {7,   6,  5,  4,  3, 2, 1, 0, 0, 1, 2,  3,  4,  5,  6,  7},   
 {7,   6,  5,  4,  3, 2, 1, 0, 0, 1, 2,  3,  4,  5,  6,  7},   
 {8,   7,  6,  5,  4, 3, 2, 1, 1, 2, 3,  4,  5,  6,  7,  8},   
 {9,   8,  7,  6,  5, 4, 3, 2, 2, 3, 4,  5,  6,  7,  8,  9},   
 {10,  9,  8,  7,  6, 5, 4, 3, 3, 4, 5,  6,  7,  8,  9, 10},   
 {11, 10,  9,  8,  7, 6, 5, 4, 4, 5, 6,  7,  8,  9, 10, 11},   
 {12, 11, 10,  9,  8, 7, 6, 5, 5, 6, 7,  8,  9, 10, 11, 12},   
 {13, 12, 11, 10,  9, 8, 7, 6, 6, 7, 8,  9, 10, 11, 12, 13},  
 {14, 13, 12, 11, 10, 9, 8, 7, 7, 8, 9, 10, 11, 12, 13, 14}};

/*
    bot starts at 0, 0, facing north
    array is flipped cw 90 degrees
    so bot starts at the top left facing right


    N = +y col(second term, left to right on the array)
    S = -y col
    E = +x row (first term, up and down on the array)
    W = -x row
*/

struct openCells {
    bool openN = true; 
    bool openS = true;
    bool openE = true;
    bool openW = true;
};




struct configuration {
    int x;
    int y;
    char dir;
};


static configuration poppedCfg;

void initialize();
void flowElevation(configuration* currentCfg, openCells walls[16][16]);
openCells checkOpenCells(configuration poppedCfg, openCells walls[16][16]);
void checkNeigboringOpen(configuration poppedCfg, int maze[16][16], openCells walls[16][16], std::stack<configuration> cellStack);
void move(configuration* currentCfg, char direction);

/*
class Flood {

public:
    
};
*/
