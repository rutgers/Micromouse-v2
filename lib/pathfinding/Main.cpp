#include <iostream>
#include <string>
#include <stack>
#include <algorithm>

#include "API.h"
#include "Flood.h"
using namespace std;

// move function seems to only work in main function ._.
// is an undefined reference when I try to include it in the Flood.cpp file
// for whatever reason

// pass the pointer to currentCfg so that we can instantly update it after calling move
static void move(configuration* currentCfg, char direction);


//the actual loop function for maze-solving

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

    //sample wall following algorithm
    
    if (!API::wallLeft()) {
        API::turnLeft();
    }
    while (API::wallFront()) {
        API::turnRight();
    }
    API::moveForward();
*/


/*
Flood fill algorithm
mouse tries to approach smaller values of the maze
as the mouse explores, it updates the values along its path. 

after it reaches its destination, in order to find the
*/

const int N = 16; 
int maze[N][N]= {{14, 13, 12, 11, 10, 9, 8, 7, 7, 8, 9, 10, 11, 12, 13, 14},
                {13, 12, 11, 10, 9, 8, 7, 6, 6, 7, 8, 9, 10, 11, 12, 13},
                {12, 11, 10, 9, 8, 7, 6, 5, 5, 6, 7, 8, 9, 10, 11, 12},   
                {11, 10, 9, 8, 7, 6, 5, 4, 4, 5, 6, 7, 8, 9, 10, 11},   
                {10, 9, 8, 7, 6, 5, 4, 3, 3, 4, 5, 6, 7, 8, 9, 10},   
                {9, 8, 7, 6, 5, 4, 3, 2, 2, 3, 4, 5, 6, 7, 8, 9},   
                {8, 7, 6, 5, 4, 3, 2, 1, 1, 2, 3, 4, 5, 6, 7, 8},   
                {7, 6, 5, 4, 3, 2, 1, 0, 0, 1, 2, 3, 4, 5, 6, 7},   
                {7, 6, 5, 4, 3, 2, 1, 0, 0, 1, 2, 3, 4, 5, 6, 7},   
                {8, 7, 6, 5, 4, 3, 2, 1, 1, 2, 3, 4, 5, 6, 7, 8},   
                {9, 8, 7, 6, 5, 4, 3, 2, 2, 3, 4, 5, 6, 7, 8, 9},   
                {10, 9, 8, 7, 6, 5, 4, 3, 3, 4, 5, 6, 7, 8, 9, 10},   
                {11, 10, 9, 8, 7, 6, 5, 4, 4, 5, 6, 7, 8, 9, 10, 11},   
                {12, 11, 10, 9, 8, 7, 6, 5, 5, 6, 7, 8, 9, 10, 11, 12},   
                {13, 12, 11, 10, 9, 8, 7, 6, 6, 7, 8, 9, 10, 11, 12, 13},  
                {14, 13, 12, 11, 10, 9, 8, 7, 7, 8, 9, 10, 11, 12, 13, 14}};
/*
    bot starts at 0, 0, facing north
    pretend array is flipped ccw 90 degrees and bot is at bottom left of maze facing up
    N = +x
    S = -x
    E = +y
    W = -y
*/

void log(const std::string& text) {
    std::cerr << text << std::endl;
}

int main(int argc, char* argv[]) {
    log("Running...");
    API::setColor(0, 0, 'G');
    API::setText(0, 0, "abc");
    
    //Modified Flood Fill
    //https://marsuniversity.github.io/ece387/FloodFill.pdf
    configuration currentCfg;
    // global struct for keeping track of current pos/orientation

    currentCfg.x = 0;
    currentCfg.y = 0;
    currentCfg.dir = 'N';

    move(&currentCfg, 'N');

    if(currentCfg.x==1) move(&currentCfg, 'E');



    while(true) {
        // Micromouse moves from higher to lower elevations
        // If there are two open cells of equal elevation to go to,
        // prioritize the one in front that doesn't require a turn 


        /*
        Flood::flowElevation(&currentCfg, maze);

        //1) Push the current cell location onto the stack
        std::stack<configuration> stack;
        stack.push(currentCfg);

        //2) Repeat while stack is not empty
        
        while(!stack.empty()) {
            //pull the cell location from the stack
            poppedCfg = stack.top();

            stack.pop();
        }

        if(maze[currentCfg.x][currentCfg.y] == 0) {
            break;
        }
        */
    }
}









static void move(configuration* currentCfg, char direction) {    

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