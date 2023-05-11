/*
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


    // // check for walls to the front, right, or left
    // static bool wallFront();
    // static bool wallRight();
    // static bool wallLeft();

    // //allow the mouse to move
    // static void moveForward(int distance = 1);
    // static void turnRight();
    // static void turnLeft();

    // //sample calls
    // API::wallLeft();
    // API:turnLeft();

    // //sample wall following algorithm
    
    // if (!API::wallLeft()) {
    //     API::turnLeft();
    // }
    // while (API::wallFront()) {
    //     API::turnRight();
    // }
    // API::moveForward();




// Flood fill algorithm
// mouse tries to approach smaller values of the maze
// as the mouse explores, it updates the values along its path. 





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
    runMaze('c');
    backTrack();
    runMaze('c');
    backTrack();
    runMaze('c');
    backTrack();
    
}

*/