#include "API.h"
#include<stack>
#include<algorithm>


//// variable declarations
const int N = 16; 

// global array for the maze, tells the robot how to move
extern int maze[N][N];


struct configuration {
    int x;
    int y;
    char dir;
};

// How to make a global variable
// https://edisciplinas.usp.br/pluginfile.php/5453726/mod_resource/content/0/Extern%20Global%20Variable.pdf

// How to use the stl stack
// https://cplusplus.com/reference/stack/stack/
extern std::stack<configuration> cellStack;
extern configuration currentCfg; // global struct for keeping track of current pos/orientation
extern configuration poppedCfg; // global struct for popped cell cause why not


struct openCells {
    bool openN = true; 
    bool openS = true;
    bool openE = true;
    bool openW = true;
};
// list of walls for recursive cell update to use
extern openCells walls[N][N];



//// function declarations
void initialize();
void flowElevation();
openCells checkOpenCells(configuration currentCfg);
void checkNeigboringOpen(configuration poppedCfg);
void move(char direction);