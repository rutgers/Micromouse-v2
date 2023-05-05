#include "Flood.h"
using namespace std;


// if minimum distance of neighboring open cells is not presentCellValue - 1
// replace present cell's distance with minimum + 1

// open cells = no wall, includes cell that we came from

// push all neighbor locations onto the stack except the goal locations (since these should stay 0)


openCells checkOpenCells(configuration currentCfg) {
    openCells temp;

    char dir = currentCfg.dir;

    switch(dir) {
        case 'N':
            temp.openS = true;
            if(API::wallFront()) temp.openN = true;
            if(API::wallLeft()) temp.openW = true;
            if(API::wallRight()) temp.openE = true;
            break;
        case 'S':
            temp.openN = true;
            if(API::wallFront()) temp.openS = true;
            if(API::wallLeft()) temp.openE = true;
            if(API::wallRight()) temp.openW = true;
            break;
        case 'E':
            temp.openW = true;
            if(API::wallFront()) temp.openE = true;
            if(API::wallLeft()) temp.openN = true;
            if(API::wallRight()) temp.openS = true;
            break;
        case 'W':
            temp.openE = true;
            if(API::wallFront()) temp.openW = true;
            if(API::wallLeft()) temp.openS = true;
            if(API::wallRight()) temp.openN = true;
            break;
    }

    return temp;
}

void flowElevation(configuration* currentCfg, int maze[16][16]) {
    // given the maze, configuration, and wall checks, move to lower elevation until we hit 0
    // prioritize movements without turns if possible

    int x = currentCfg->x;
    int y = currentCfg->y;
    char dir = currentCfg->dir;


    // if the front cell is valid, lower, and there's no wall, move forward


    // static bool wallFront();
    // static bool wallRight();
    // static bool wallLeft();


    openCells checkOpen = checkOpenCells(*currentCfg);
    bool openN = checkOpen.openN;
    bool openS = checkOpen.openS;
    bool openE = checkOpen.openE;
    bool openW = checkOpen.openW;

    //min of the open cells
    int N = 1337;
    int S = 1337;
    int E = 1337;
    int W = 1337;

    //N = +x
    //S = -x
    //E = +y
    //W = -y

    if(x+1 <= 15 && openN) N = maze[x+1][y];
    if(x-1 >= 0 && openS) S = maze[x-1][y];
    if(y+1 <= 15 && openE) E = maze[x][y+1];
    if(y-1 >= 0 && openW) W = maze[x][y-1];

    // find the min using arraysort
    int arraySort[4] = {N, S, E, W};
    std::sort(arraySort, arraySort + 4);
    int min = arraySort[0];

    // if minimum distance of neighboring open cells is presentCellValue - 1
    // prefer to move forward without spinning
    // TODO
    if(N == min) {
        move(currentCfg, 'N');
        // currentCfg.x++;
        // currentCfg.dir = 'N';
    }
    if(S == min) {
        move(currentCfg, 'S');
        // currentCfg.x--;
        // currentCfg.dir = 'S';
    }
    if(E == min) {
        move(currentCfg, 'E');
        // currentCfg.y++;
        // currentCfg.dir = 'E';
    }
    if(W == min) {
        move(currentCfg, 'W');
        // currentCfg.y--;
        // currentCfg.dir = 'W';
    }

    return ;
}


void checkNeigboringOpen(configuration poppedCfg, int maze[16][16]) {
    openCells checkOpen = checkOpenCells(poppedCfg);

    bool openN = checkOpen.openN;
    bool openS = checkOpen.openS;
    bool openE = checkOpen.openE;
    bool openW = checkOpen.openW;

    int x = poppedCfg.x;
    int y = poppedCfg.y;
    char dir = poppedCfg.dir;

    //min of the open cells
    int N = 1337;
    int S = 1337;
    int E = 1337;
    int W = 1337;


    //N = +x
    //S = -x
    //E = +y
    //W = -y

    if(x+1 <= 15 && openN) N = maze[x+1][y];
    if(x-1 >= 0 && openS) S = maze[x-1][y];
    if(y+1 <= 15 && openE) E = maze[x][y+1];
    if(y-1 >= 0 && openW) W = maze[x][y-1];

    // find the min using arraysort
    int arraySort[4] = {N, S, E, W};
    std::sort(arraySort, arraySort + 4);
    int min = arraySort[0];

    // if minimum distance of neighboring open cells is not presentCellValue - 1

    if(min != maze[x][y] - 1) {
        // replace present cell's distance with minimum + 1
        maze[x][y] = min + 1;

        // push all neighbor locations onto the stack except the goal locations

        configuration pushCfg = poppedCfg;

        if(x+1 <= 15 && !(x+1 == 7 || x+1 == 8) && !(y == 7 || y == 8)) {
            pushCfg.x += 1;
            cellStack.push(pushCfg);
            pushCfg.x -= 1;
        }
        if(x-1 >= 0 && !(x-1 == 7 || x-1 == 8) && !(y == 7 || y == 8)) {
            pushCfg.x -= 1;
            cellStack.push(pushCfg);
            pushCfg.x += 1;
        }
        if(y+1 <= 15 && !(x == 7 || x == 8) && !(y+1 == 7 || y+1 == 8)) {
            pushCfg.y += 1;
            cellStack.push(pushCfg);
            pushCfg.y -= 1;
        }
        if(y-1 >= 0 && !(x == 7 || x == 8) && !(y-1 == 7 || y-1 == 8)) {
            pushCfg.y -= 1;
            cellStack.push(pushCfg);
            pushCfg.y += 1;
        }
    }

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




void move(configuration* currentCfg, char direction) {

    char facing = currentCfg->dir;

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

    //N = +x
    //S = -x
    //E = +y
    //W = -y

    switch(direction) {

        case 'N':
            currentCfg->x++; currentCfg->dir = 'N';
        break;

        case 'S':
            currentCfg->x--; currentCfg->dir = 'S';
        break;

        case 'E':
            currentCfg->y++; currentCfg->dir = 'E';
        break;

        case 'W':
            currentCfg->y--; currentCfg->dir = 'W';
        break;
    }

    return;
}