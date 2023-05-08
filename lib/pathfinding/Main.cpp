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
    
    
    
    configuration currentCfg;
    // global struct for keeping track of current pos/orientation

    currentCfg.x = 0;
    currentCfg.y = 0;
    currentCfg.dir = 'N';

    static openCells walls[N][N];
    //global array for keeping track of walls
    // borders
    for(int i = 0; i < 16; i++) {
        walls[i][0].openS = false; // move along south wall
        walls[i][15].openN = false; // move along north wall
        walls[0][i].openW = false; // move along west wall
        walls[15][i].openE = false; // move along east wall
        // std::cerr << "Hello";
    }

    static std::stack<configuration> cellStack;


    //Modified Flood Fill
    //https://marsuniversity.github.io/ece387/FloodFill.pdf
    


    while(true) {
        // checkOpenCells(currentCfg);

        // Micromouse moves from higher to lower elevations
        // If there are two open cells of equal elevation to go to,
        // prioritize the one in front that doesn't require a turn 
        
        std::cerr << "[" << currentCfg.x << " " << currentCfg.y << " " << currentCfg.dir << "] -> " << maze[currentCfg.x][currentCfg.y] << std::endl;
        flowElevation(&currentCfg, walls);
        
        std::cerr << "Walls Array "<< walls[currentCfg.x][currentCfg.y].openN << walls[currentCfg.x][currentCfg.y].openS << walls[currentCfg.x][currentCfg.y].openE << walls[currentCfg.x][currentCfg.y].openW << std::endl;


        //1) Push the current cell location onto the stack
        cellStack.push(currentCfg);

        //2) Repeat while stack is not empty

        std::cerr << "Size of stack: " << cellStack.size() <<std::endl;


        while(!cellStack.empty()) {
            //pull the cell location from the stack
            poppedCfg = cellStack.top();
            cellStack.pop();

            // std::cerr << poppedCfg.x << " " << poppedCfg.y << " " << poppedCfg.dir << std::endl;

            checkNeigboringOpen(poppedCfg, maze, walls, cellStack);
        }

        if(maze[currentCfg.x][currentCfg.y] == 0) {
            break;
        }
        
        log("\n");
        
        for(int i = 0; i < N; i++) {
            for(int j = 0; j < N; j++) {
                if(maze[i][j] < 10) std::cerr << " " << maze[i][j];
                else std::cerr << maze[i][j];

            }
            log("");
        }   
        
        log("\n");
    }

   
   
}
