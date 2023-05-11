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


    //// STEP 1: Initial Solve
    initialize();

    while(true) {
        // Micromouse moves from higher to lower elevations
        std::cerr << "[" << currentCfg.x << " " << currentCfg.y << " " << currentCfg.dir << "] -> " << maze[currentCfg.x][currentCfg.y] << std::endl;
        flowElevation();
        

        //end condition
        if(maze[currentCfg.x][currentCfg.y] == 0) {
            break;
        }

        std::cerr << "Walls Array "<< walls[currentCfg.x][currentCfg.y].openN << walls[currentCfg.x][currentCfg.y].openS << walls[currentCfg.x][currentCfg.y].openE << walls[currentCfg.x][currentCfg.y].openW << std::endl;

        //1) Push the current cell location onto the stack
        cellStack.push(currentCfg);

        //2) Repeat while stack is not empty        
        while(!cellStack.empty()) {
            //pull the cell location from the stack
            poppedCfg = cellStack.top();
            cellStack.pop();

            // std::cerr << poppedCfg.x << " " << poppedCfg.y << " " << poppedCfg.dir << std::endl;

            checkNeigboringOpen(poppedCfg);
        }

        mazePrintout();


    }

    //// STEP 2: RETURN PATH
    // invertMaze doesn't seem to be the best implementation, especially for
    // pathing back to center in STEP 3, since multiple cells could have equal 
    // distance from the center -> sinks of 0's
    invertMaze('s');

    while(true) {
        // Micromouse moves from higher to lower elevations
        std::cerr << "[" << currentCfg.x << " " << currentCfg.y << " " << currentCfg.dir << "] -> " << maze[currentCfg.x][currentCfg.y] << std::endl;
        flowElevation();

        //end condition
        if(currentCfg.x == 0 && currentCfg.y == 0) {
            break;
        }

        std::cerr << "Walls Array "<< walls[currentCfg.x][currentCfg.y].openN << walls[currentCfg.x][currentCfg.y].openS << walls[currentCfg.x][currentCfg.y].openE << walls[currentCfg.x][currentCfg.y].openW << std::endl;

        //1) Push the current cell location onto the stack
        cellStack.push(currentCfg);

        //2) Repeat while stack is not empty        
        while(!cellStack.empty()) {
            //pull the cell location from the stack
            poppedCfg = cellStack.top();
            cellStack.pop();

            // std::cerr << poppedCfg.x << " " << poppedCfg.y << " " << poppedCfg.dir << std::endl;

            checkNeigboringOpen(poppedCfg);
        }
        
        mazePrintout();

    }
    

    //// STEP 3: FAST RUN, same code as STEP 1, but with invert maze
    
    invertMaze('c');

    mazePrintout();

    
    while(true) {
        // Micromouse moves from higher to lower elevations
        std::cerr << "[" << currentCfg.x << " " << currentCfg.y << " " << currentCfg.dir << "] -> " << maze[currentCfg.x][currentCfg.y] << std::endl;
        flowElevation();
        

        //end condition
        // it's a possibility that equally distant cells could have values of 0 afcter inverting the maze
        // check for configuration rather than cell value
        if(((currentCfg.x == 7 || currentCfg.x == 8) && (currentCfg.y == 7 || currentCfg.y == 8))) {
            break;
        }

        std::cerr << "Walls Array "<< walls[currentCfg.x][currentCfg.y].openN << walls[currentCfg.x][currentCfg.y].openS << walls[currentCfg.x][currentCfg.y].openE << walls[currentCfg.x][currentCfg.y].openW << std::endl;

        //1) Push the current cell location onto the stack
        cellStack.push(currentCfg);

        //2) Repeat while stack is not empty        
        while(!cellStack.empty()) {
            //pull the cell location from the stack
            poppedCfg = cellStack.top();
            cellStack.pop();

            // std::cerr << poppedCfg.x << " " << poppedCfg.y << " " << poppedCfg.dir << std::endl;

            checkNeigboringOpen(poppedCfg);
        }
        
        mazePrintout();


    }
    
    
}
