#include "API.h"
#include<stack>
#include<algorithm>

const int N = 16; 

extern int maze[N][N];

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


extern std::stack<configuration> cellStack;
extern openCells walls[N][N];
extern configuration currentCfg;
    // global struct for keeping track of current pos/orientation

extern configuration poppedCfg;

// void initialize();
void flowElevation(configuration* currentCfg);
openCells checkOpenCells(configuration poppedCfg);
void checkNeigboringOpen(configuration poppedCfg);
void move(configuration* currentCfg, char direction);

/*
class Flood {

public:
    
};
*/
