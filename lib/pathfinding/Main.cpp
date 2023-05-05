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

// static void move(configuration* currentCfg, char direction);


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


    // move(&currentCfg, 'N');

    // if(currentCfg.x==1) move(&currentCfg, 'E');
    // if(currentCfg.dir=='E') move(&currentCfg, 'S');


    while(true) {
        // Micromouse moves from higher to lower elevations
        // If there are two open cells of equal elevation to go to,
        // prioritize the one in front that doesn't require a turn 
        
        flowElevation(&currentCfg, maze);

        //1) Push the current cell location onto the stack
        cellStack.push(currentCfg);

        //2) Repeat while stack is not empty
        
        while(!cellStack.empty()) {
            //pull the cell location from the stack
            poppedCfg = cellStack.top();
            checkNeigboringOpen(poppedCfg, maze);

            cellStack.pop();
        }

        if(maze[currentCfg.x][currentCfg.y] == 0) {
            break;
        }
        
    }

   for(int i = 0; i < N; i++) {
    for(int j = 0; j < N; j++) {
        std::cerr << maze[i][j] << " ";
    }
    log("\n");
   }
   
}
