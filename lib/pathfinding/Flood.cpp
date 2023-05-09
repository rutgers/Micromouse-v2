#include "Flood.h"
#include <iostream>

using namespace std;

// "Definition checked against [extern] declaration"
int maze[N][N] = 
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
 
std::stack<configuration> cellStack;
openCells walls[N][N];
configuration currentCfg;
configuration poppedCfg;



void initialize() {
    // set current configuration to (0, 0) facing N
    currentCfg.x = 0;
    currentCfg.y = 0;
    currentCfg.dir = 'N';

    // set borders for walls array
    for(int i = 0; i < 16; i++) {
        walls[i][0].openS = false; // move along south wall
        walls[i][15].openN = false; // move along north wall
        walls[0][i].openW = false; // move along west wall
        walls[15][i].openE = false; // move along east wall
    }
}



// open cells = no wall, includes cell that we came from
// use sensors/API to check if there's a wall to the front, left, right
// -> make a decision based on current orientation to update walls array
openCells checkOpenCells(configuration currentCfg) {
    openCells temp;
    temp.openN = false;
    temp.openS = false;
    temp.openE = false;
    temp.openW = false;

    int x = currentCfg.x;
    int y = currentCfg.y;
    char dir = currentCfg.dir;


    switch(dir) {
        case 'N':
            temp.openS = true;
            if(!API::wallFront()) temp.openN = true;
            if(!API::wallLeft()) temp.openW = true;
            if(!API::wallRight()) temp.openE = true;
            break;
        case 'S':
            temp.openN = true;
            if(!API::wallFront()) temp.openS = true;
            if(!API::wallLeft()) temp.openE = true;
            if(!API::wallRight()) temp.openW = true;
            break;
        case 'E':
            temp.openW = true;
            if(!API::wallFront()) temp.openE = true;
            if(!API::wallLeft()) temp.openN = true;
            if(!API::wallRight()) temp.openS = true;
            break;
        case 'W':
            temp.openE = true;
            if(!API::wallFront()) temp.openW = true;
            if(!API::wallLeft()) temp.openS = true;
            if(!API::wallRight()) temp.openN = true;
            break;
    }


    //update walls array
    walls[x][y].openN = temp.openN;
    walls[x][y].openS = temp.openS;
    walls[x][y].openE = temp.openE;
    walls[x][y].openW = temp.openW;

    return temp;
}



void flowElevation() {
    // given the maze, configuration, and wall checks, move to lower elevation until we hit 0
    // prioritize movements without turns if possible (TODO)

    int x = currentCfg.x; // up and down on the array = EW, first term
    int y = currentCfg.y; // left and right on the array = NS, second term
    char dir = currentCfg.dir;


    // check if surrounding cells are valid, 
    // pick the lowest out of the open cells,
    // and move forward

    openCells checkOpen = checkOpenCells(currentCfg);
    bool openN = checkOpen.openN;
    bool openS = checkOpen.openS;
    bool openE = checkOpen.openE;
    bool openW = checkOpen.openW;

    //min of the open cells
    int N = 1337;
    int S = 1337;
    int E = 1337;
    int W = 1337;

    //N = +y
    //S = -y
    //E = +x
    //W = -x

    if(y+1 <= 15 && openN) N = maze[x][y+1];
    if(y-1 >= 0 && openS) S = maze[x][y-1];
    if(x+1 <= 15 && openE) E = maze[x+1][y];
    if(x-1 >= 0 && openW) W = maze[x-1][y];

    // find the min using arraysort
    int arraySort[4] = {N, S, E, W};
    std::sort(arraySort, arraySort + 4);
    int min = arraySort[0];
    std::cerr << "Min Cell Calculated: " << min << std::endl;
    
    // move to minimum of open cells (usually presentCellValue - 1)
    // prefer to move forward without spinning (TODO)

    /*
    // extra parameters for move
    if(N == min && maze[x][y] == min + 1 && openN) move(currentCfg, 'N');
    if(S == min && maze[x][y] == min + 1 && openS) move(currentCfg, 'S');
    if(E == min && maze[x][y] == min + 1 && openE) move(currentCfg, 'E');
    if(W == min && maze[x][y] == min + 1 && openW) move(currentCfg, 'W');
    */
    if(N == min && openN) {
        move('N');
        return;
    }
    if(S == min && openS) {
        move('S');
        return;
    }
    if(E == min && openE) {
        move('E');
        return;
    }
    if(W == min && openW) {
        move('W');
        return;
    }

    //update wall array after moving too
    checkOpenCells(currentCfg);

    return;
}



// if minimum distance of neighboring OPEN cells is not presentCellValue - 1
// ->  replace present cell's distance with minimum + 1
// -> push all neighbor locations onto the stack except the goal locations 
//    (since these should stay 0)
void checkNeigboringOpen(configuration poppedCfg) {
    
    /*
    openCells checkOpen = checkOpenCells(poppedCfg);

    bool openN = checkOpen.openN;
    bool openS = checkOpen.openS;
    bool openE = checkOpen.openE;
    bool openW = checkOpen.openW;

    // For the popped configuration, refer to the global 
    // walls array instead of checking from the API
    */

    int x = poppedCfg.x;
    int y = poppedCfg.y;
    char dir = poppedCfg.dir;

    bool openN = walls[x][y].openN;
    bool openS = walls[x][y].openS;
    bool openE = walls[x][y].openE;
    bool openW = walls[x][y].openW;

    //min of the open cells
    int N = 1337;
    int S = 1337;
    int E = 1337;
    int W = 1337;

    //N = +y
    //S = -y
    //E = +x
    //W = -x

    if(y+1 <= 15 && openN) N = maze[x][y+1];
    if(y-1 >= 0 && openS) S = maze[x][y-1];
    if(x+1 <= 15 && openE) E = maze[x+1][y];
    if(x-1 >= 0 && openW) W = maze[x-1][y];


    // find the min using arraysort
    int arraySort[4] = {N, S, E, W};
    std::sort(arraySort, arraySort + 4);
    int min = arraySort[0];
    std::cerr << maze[x][y] << std::endl;
    std::cerr << arraySort[0] << " " <<  arraySort[1] << " " << arraySort[2] << " " <<arraySort[3] << std::endl;

    // if minimum distance of neighboring open cells is not presentCellValue - 1

    if(min != maze[x][y] - 1) {
        std::cerr << "min check failed";
        // replace present cell's distance with minimum + 1
        maze[x][y] = min + 1;

        // push all neighbor locations onto the stack except the goal locations

        configuration pushCfg = poppedCfg;


        if(x+1 <= 15 && !((x+1==7 && y==7) || (x+1==7 && y==8) || (x+1==8 && y==7) || (x+1==8 && y==8))) {
            pushCfg.x += 1;
            cellStack.push(pushCfg);
            std::cerr << "Pushed (" << pushCfg.x << ", " << pushCfg.y << ")";
            pushCfg.x -= 1;
        }
        if(x-1 >= 0 && !((x-1==7 && y==7) || (x-1==7 && y==8) || (x-1==8 && y==7) || (x-1==8 && y==8))) {
            pushCfg.x -= 1;
            cellStack.push(pushCfg);
            std::cerr << "Pushed (" << pushCfg.x << ", " << pushCfg.y << ")";
            pushCfg.x += 1;
        }
        if(y+1 <= 15 && !((x==7 && y+1==7) || (x==7 && y+1==8) || (x==8 && y+1==7) || (x==8 && y+1==8))) {
            pushCfg.y += 1;
            cellStack.push(pushCfg);
            std::cerr << "Pushed (" << pushCfg.x << ", " << pushCfg.y << ")";
            pushCfg.y -= 1;
        }
        if(y-1 >= 0 && !((x==7 && y-1==7) || (x==7 && y-1==8) || (x==8 && y-1==7) || (x==8 && y-1==8))) {
            pushCfg.y -= 1;
            cellStack.push(pushCfg);
            std::cerr << "Pushed (" << pushCfg.x << ", " << pushCfg.y << ")";
            pushCfg.y += 1;
        }
    }

    std::cerr << "stack size: " << cellStack.size();

    return;
}



/*
    // check for walls to the front, right, or left
    static bool wallFront();
    static bool wallRight();
    static bool wallLeft();

    //allow the mouse to move
    static void moveForward(int distance = 1);
    static void turnRight();
    static void turnLeft();

    //sample calls
    API::wallLeft();
    API:turnLeft();
*/
void move(char direction) {

    char facing = currentCfg.dir;

    // if facing and direction are the same, go straight
    if(facing == direction) {
        API::moveForward();
    } else {

        if(facing == 'N') {
            switch(direction) {

            case 'S': // turn around
            API::turnRight(); API::turnRight(); API::moveForward();
            break;

            case 'W': // turnLeft
            API::turnLeft(); API::moveForward();
            break;

            case 'E': // turnRight
            API::turnRight(); API::moveForward();
            break;
            }
        }

        if(facing == 'S') {
            switch(direction) {

            case 'N': // turn around
            API::turnRight(); API::turnRight(); API::moveForward();
            break;

            case 'E': // turnLeft
            API::turnLeft(); API::moveForward();
            break;

            case 'W': // turnRight
            API::turnRight(); API::moveForward();
            break;
            }
        }

        if(facing == 'E') {
            switch(direction) {

            case 'W': // turn around
            API::turnRight(); API::turnRight(); API::moveForward();
            break;

            case 'N': // turnLeft
            API::turnLeft(); API::moveForward();
            break;

            case 'S': // turnRight
            API::turnRight(); API::moveForward();
            break;
            }
        }

        if(facing == 'W') {
            switch(direction) {

            case 'E': // turn around
            API::turnRight(); API::turnRight(); API::moveForward();
            break;

            case 'S': // turnLeft
            API::turnLeft(); API::moveForward();
            break;

            case 'N': // turnRight
            API::turnRight(); API::moveForward();
            break;
            }
        }
    }

    //N = +y
    //S = -y
    //E = +x
    //W = -x

    currentCfg.dir = direction;

    switch(direction) {

        case 'N':
            currentCfg.y++;
        break;

        case 'S':
            currentCfg.y--;
        break;

        case 'E':
            currentCfg.x++;
        break;

        case 'W':
            currentCfg.x--;
        break;
    }

    return;
}