#include <iostream>
#include <string>
#include <stack>
#include <Arduino.h>
#include <SPI.h>
#include "lib\Motors.h"
#include "lib\PIDRotate.h"
#include "lib\PIDStraight.h"
#include "lib\PIDMagicStraight.h"
#include "lib\tof.h"
#include "lib\IMU.h"
PIDstraight* pidstraight_instance = new PIDstraight();
//#include "API.h"

/*void log(const std::string& text) {
    std::cerr << text << std::endl;
}*/

enum direction {
    NORTH,
    SOUTH,
    EAST,
    WEST
};

/*void printDirection(int direction) {
    if (direction == NORTH) {
        log("NORTH");
    }
    if (direction == SOUTH) {
        log("SOUTH");
    }
    if (direction == EAST) {
        log("EAST");
    }
    if (direction == WEST) {
        log("WEST");
    }
}*/

void changeDirectionRight(int* d) {
    if (*d == NORTH) {
        *d = EAST;
    } else if (*d == EAST) {
        *d = SOUTH;
    } else if (*d == SOUTH) {
        *d = WEST;
    } else if (*d == WEST) {
        *d = NORTH;
    }
    //printDirection(*d);
}

void changeDirectionLeft(int* d) {
    if (*d == NORTH) {
        *d = WEST;
    } else if (*d == WEST) {
        *d = SOUTH;
    } else if (*d == SOUTH) {
        *d = EAST;
    } else if (*d == EAST) {
        *d = NORTH;
    }
    //printDirection(*d);
}

int mazetable[16][16] = {
    {14,13,12,11,10,9,8,7,7,8,9,10,11,12,13,14},
    {13,12,11,10,9,8,7,6,6,7,8,9,10,11,12,13},
    {12,11,10,9,8,7,6,5,5,6,7,8,9,10,11,12},
    {11,10,9,8,7,6,5,4,4,5,6,7,8,9,10,11},
    {10,9,8,7,6,5,4,3,3,4,5,6,7,8,9,10},
    {9,8,7,6,5,4,3,2,2,3,4,5,6,7,8,9},
    {8,7,6,5,4,3,2,1,1,2,3,4,5,6,7,8},
    {7,6,5,4,3,2,1,0,0,1,2,3,4,5,6,7},
    {7,6,5,4,3,2,1,0,0,1,2,3,4,5,6,7},
    {8,7,6,5,4,3,2,1,1,2,3,4,5,6,7,8},
    {9,8,7,6,5,4,3,2,2,3,4,5,6,7,8,9},
    {10,9,8,7,6,5,4,3,3,4,5,6,7,8,9,10},
    {11,10,9,8,7,6,5,4,4,5,6,7,8,9,10,11},
    {12,11,10,9,8,7,6,5,5,6,7,8,9,10,11,12},
    {13,12,11,10,9,8,7,6,6,7,8,9,10,11,12,13},
    {14,13,12,11,10,9,8,7,7,8,9,10,11,12,13,14}
};

int southwalls[16][16] = {
     {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
     {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
     {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
     {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
     {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
     {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
     {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
     {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
     {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
     {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
};
int northwalls[16][16] = {
     {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
     {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
     {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
     {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
     {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
     {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
     {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
     {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
     {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
     {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
};
int eastwalls[16][16] = {
     {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
     {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
     {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
     {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
     {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
     {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
     {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
     {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
     {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
     {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
};
int westwalls[16][16] = {
     {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
     {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
     {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
     {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
     {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
     {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
     {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
     {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
     {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
     {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
};
int ypos = 0;
int xpos = 0;

int botvisited[16][16] = {
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
};

void updateWallPositions(int d) {
    if (d == NORTH) {
        if (tof_instance->readF() < 100) {
            northwalls[xpos][ypos] = 1;
        }
        if (tof_instance->readL() < 100) {
            westwalls[xpos][ypos] = 1;
        }
        if (tof_instance->readR() < 100) {
            eastwalls[xpos][ypos] = 1;
        }
    } else if (d == SOUTH) {
        if (tof_instance->readF() < 100) {
            southwalls[xpos][ypos] = 1;
        }
        if (tof_instance->readL() < 100) {
            eastwalls[xpos][ypos] = 1;
        }
        if (tof_instance->readR() < 100) {
            westwalls[xpos][ypos] = 1;
        }
    } else if (d == EAST) {
        if (tof_instance->readF() < 100) {
            eastwalls[xpos][ypos] = 1;
        }
        if (tof_instance->readL() < 100) {
            northwalls[xpos][ypos] = 1;
        }
        if (tof_instance->readR() < 100) {
            southwalls[xpos][ypos] = 1;
        }
    } else if (d == WEST) {
        if (tof_instance->readF() < 100) {
            westwalls[xpos][ypos] = 1;
        }
        if (tof_instance->readL() < 100) {
            southwalls[xpos][ypos] = 1;
        }
        if (tof_instance->readR() < 100) {
            northwalls[xpos][ypos] = 1;
        }
    }
}


std::stack<int> xposes; 
std::stack<int> yposes;

void updatePositions(int d, int* x, int* y) {
    if (d == NORTH) {
        *y += 1;
    }
    if (d == SOUTH) {
        *y -= 1;
    }
    if (d == EAST) {
        *x += 1;
    }
    if (d == WEST) {
        *x -= 1;
    }
}

void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);

    //timeofflight = new tof();
    //timeofflight->checkAddresses();  
    // Serial.println(timeofflight->readDistance().left);
    //Serial.println("hello world");
    //delay(5000);
    //Serial.print("left dist: ");
    motors_instance->enableMotors();
}

void loop() {
    //log("Running...");
    //API::setColor(0, 0, 'G');
    //API::setText(0, 0, "abc");

    /*for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            API::setText(i,j,std::to_string(mazetable[i][j]));
        }
    }*/
    // while(1){
    //     Serial.printf("left: %d\t right: %d\n",tof_instance->readL(),tof_instance->readR());
    // }

    int direction = NORTH;
    while (true) {

        delay(500);
        pidrotate_instance->rotate_to_angle(round(imu_instance->getHeading()/90.0)*90.0, 1.25, 1.0, 0.02);
        delay(500);

        int tempX = xpos; int tempY = ypos;


        int lMinVal = 1000;
        int fMinVal = 1000;
        int rMinVal = 1000;

        int minValue = 1000;

        xposes.push(xpos);
        yposes.push(ypos);
        
        botvisited[xpos][ypos] = 1;

        if (mazetable[xpos][ypos] == 0) {
            //log("WE FUCKIN DID IT LESS GO");
            break;
        }

        updateWallPositions(direction);

        if (tof_instance->readF() > 100) {
            //log("can go front");
            updatePositions(direction, &tempX, &tempY);
            //log("tempY: " + std::to_string(tempY));
            //log("Ypos: " + std::to_string(ypos));

            //log("changeDist: " + std::to_string(mazetable[tempX][tempY]));
            //log("origDist: " + std::to_string(mazetable[xpos][ypos]));
            fMinVal = mazetable[tempX][tempY];
            if (fMinVal < minValue) {
                minValue = fMinVal;
            }

        /*if (!API::wallFront()) {
            log("x: " + std::to_string(xpos));
            log("y: " + std::to_string(ypos));
            log("dist: " + std::to_string(mazetable[xpos][ypos]));

            API::moveForward();
            updatePositions(direction, &xpos, &ypos);
        } else if (!API::wallRight()) {
            API::turnRight();
            changeDirectionRight(&direction);
           // API::moveForward();
        } else {
            API::turnLeft();
            changeDirectionLeft(&direction);
           // API::moveForward();
        }*/
        }

        tempX = xpos; tempY = ypos;
        if (tof_instance->readL() > 100) {
            //log("can go left");
            changeDirectionLeft(&direction);
            updatePositions(direction, &tempX, &tempY);
            //log("tempY: " + std::to_string(tempY));
            //log("Ypos: " + std::to_string(ypos));

            //log("changeDist: " + std::to_string(mazetable[tempX][tempY]));
            //log("origDist: " + std::to_string(mazetable[xpos][ypos]));
            lMinVal = mazetable[tempX][tempY];
            changeDirectionRight(&direction);
            if (lMinVal < minValue) {
                minValue = lMinVal;
            }
        }

        tempX = xpos; tempY = ypos;
        if (tof_instance->readR() > 100) {
            //log("can go right");
            changeDirectionRight(&direction);
            updatePositions(direction, &tempX, &tempY);
            rMinVal = mazetable[tempX][tempY];
            changeDirectionLeft(&direction);
            if (rMinVal < minValue) {
                minValue = rMinVal;
            }
        }

        //printDirection(direction);

        /*log("minValue: " + std::to_string(minValue));
        log("fMinVal: " + std::to_string(fMinVal));
        log("lMinVal: " + std::to_string(lMinVal));
        log("rMinVal: " + std::to_string(rMinVal));*/

        if (minValue < mazetable[xpos][ypos]) {
            if (fMinVal == minValue) {
                //log("x: " + std::to_string(xpos));
                //log("y: " + std::to_string(ypos));
                //log("dist: " + std::to_string(mazetable[xpos][ypos]));

                //API::moveForward();
                pidstraight_instance->drive_to_position(6);
                updatePositions(direction, &xpos, &ypos);
                continue;
            }

            if (lMinVal == minValue) {
                //log("x: " + std::to_string(xpos));
                //log("y: " + std::to_string(ypos));
                //log("dist: " + std::to_string(mazetable[xpos][ypos]));

                changeDirectionLeft(&direction);
                //API::turnLeft();
                pidrotate_instance->rotate_to_angle(imu_instance->getHeading()-90, 1.25, 5.0, 0.0002);
                //API::moveForward();
                pidstraight_instance->drive_to_position(6);
                updatePositions(direction, &xpos, &ypos);
                continue;
            }
            
            if (rMinVal == minValue) {

                changeDirectionRight(&direction);
                //API::turnRight();
                pidrotate_instance->rotate_to_angle(imu_instance->getHeading()+90, 1.25, 5.0, 0.0002);
                //API::moveForward();
                pidstraight_instance->drive_to_position(6);
                updatePositions(direction, &xpos, &ypos);
                continue;
            }
        } else if (minValue == 1000) {
            // no possible turns, should 180
            changeDirectionRight(&direction);
            //API::turnRight();
            pidrotate_instance->rotate_to_angle(imu_instance->getHeading()+90, 1.25, 5.0, 0.0002);
            changeDirectionRight(&direction);
            //API::turnRight();
            pidrotate_instance->rotate_to_angle(imu_instance->getHeading()+90, 1.25, 5.0, 0.0002);
            continue;
        } else {
            // no options, must flood
            //log("flooding");

            /*//shitty inefficient way
        
            for (int i = xpos; i < 16; i++) {
                for (int j = ypos; j < 16; j++) {
                    int fMin = 1000;
                    int lMin = 1000;
                    int rMin = 1000;
                    int bMin = 1000;
                    int min = 1000;

                    int cur = mazetable[xpos][ypos];

                    if (xpos - 1 > 0 && ypos - 1 > 0 && !API::wallFront() && mazetable[xpos][ypos+1] < cur) {
                        fMin = mazetable[xpos][ypos+1];
                        if (fMin < min) {
                            min =fMin;
                        }
                    }
                    if (xpos - 1 > 0 && ypos - 1 > 0 && !API::wallLeft() && mazetable[xpos-1][ypos] < cur) {
                        lMin = mazetable[xpos-1][ypos];
                        if (lMin < min) {
                            min = lMin;
                        }
                    }
                    if (xpos - 1 > 0 && ypos - 1 > 0 && !API::wallRight() && mazetable[xpos+1][ypos] < cur) {
                        rMin = mazetable[xpos+1][ypos];
                        if (rMin < min) {
                            min = rMin;
                        }
                    }
                    if (xpos - 1 > 0 && ypos - 1 > 0 && mazetable[xpos][ypos-1] < cur) {
                        bMin = mazetable[xpos][ypos-1];
                        if (bMin < min) {
                            min = bMin;
                        }
                    }

                    if (min < cur) {
                        log("updating value");
                        mazetable[i][j] = min+1;
                        API::setText(i,j,std::to_string(mazetable[i][j]));
                    }
                }
            }
            for (int i = xpos; i > 0; i--) {
                for (int j = ypos; j > 0; j--) {
                    int fMin = 1000;
                    int lMin = 1000;
                    int rMin = 1000;
                    int bMin = 1000;
                    int min = 1000;

                    int cur = mazetable[xpos][ypos];

                    if (xpos - 1 > 0 && ypos - 1 > 0 && !API::wallFront() && mazetable[xpos][ypos+1] < cur) {
                        fMin = mazetable[xpos][ypos+1];
                        if (fMin < min) {
                            min =fMin;
                        }
                    }
                    if (xpos - 1 > 0 && ypos - 1 > 0 && !API::wallLeft() && mazetable[xpos-1][ypos] < cur) {
                        lMin = mazetable[xpos-1][ypos];
                        if (lMin < min) {
                            min = lMin;
                        }
                    }
                    if (xpos - 1 > 0 && ypos - 1 > 0 && !API::wallRight() && mazetable[xpos+1][ypos] < cur) {
                        rMin = mazetable[xpos+1][ypos];
                        if (rMin < min) {
                            min = rMin;
                        }
                    }
                    if (xpos - 1 > 0 && ypos - 1 > 0 && mazetable[xpos][ypos-1] < cur) {
                        bMin = mazetable[xpos][ypos-1];
                        if (bMin < min) {
                            min = bMin;
                        }
                    }

                    if (min < cur) {
                        log("updating value");
                        if (fMin == min) {
                            mazetable[i][j+1] = min+1;
                            API::setText(i,j+1,std::to_string(mazetable[i][j+1]));
                        }
                        if (lMin == min) {
                            mazetable[i-1][j] = min+1;
                            API::setText(i-1,j,std::to_string(mazetable[i-1][j]));
                        }
                        if (rMin == min) {
                            mazetable[i+1][j] = min+1;
                            API::setText(i+1,j,std::to_string(mazetable[i+1][j]));
                        }
                        if (bMin == min) {
                            mazetable[i][j-1] = min+1;
                            API::setText(i,j-1,std::to_string(mazetable[i][j-1]));
                        }

                        
                    }
                }
            }*/



            
            int visited[16][16] = {
                {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
                {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
                {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
                {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
                {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
                {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
                {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
                {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
                {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
                {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
                {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
                {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
                {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
                {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
                {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
                {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
            };

            mazetable[xpos][ypos] = minValue + 1;
            visited[xpos][ypos] = 1;
            //API::setText(xpos,ypos,std::to_string(mazetable[xpos][ypos]));

            while (!xposes.empty()) {
                int tmpX = xposes.top(); int tmpY = yposes.top();
                xposes.pop(); yposes.pop();
                visited[tmpX][tmpY] = 1;

                /*log("testing value");
                log("tmpX: " + std::to_string(tmpX));
                log("tmpY: " + std::to_string(tmpY));*/

                int fMin = 1000;
                int lMin = 1000;
                int rMin = 1000;
                int bMin = 1000;
                int min = 1000;

                int cur = mazetable[tmpX][tmpY];

                if (/*tmpX - 1 > 0 && tmpY - 1 > 0*/tmpY+1 < 16 && !northwalls[tmpX][tmpY] && visited[tmpX][tmpY] == 0) {
                    if (tmpY+1 < 16 && visited[tmpX][tmpY+1] == 0 && botvisited[tmpX][tmpY+1] == 1) {
                        xposes.push(tmpX);
                        yposes.push(tmpY+1);
                    }
                    if (mazetable[tmpX][tmpY+1] > cur) {
                        fMin = mazetable[tmpX][tmpY+1];
                        if (fMin < min) {
                            min = fMin;
                        }
                    }
                }
                if (tmpX - 1 > 0/* && tmpY - 1 > 0*/ && !westwalls[tmpX][tmpY] && visited[tmpX-1][tmpY] == 0 && botvisited[tmpX][tmpY] == 1) {
                    if (tmpX-1 > 0 && visited[tmpX-1][tmpY] == 0) {
                        xposes.push(tmpX-1);
                        yposes.push(tmpY);
                    }
                    if (mazetable[tmpX-1][tmpY] > cur) {
                        lMin = mazetable[tmpX-1][tmpY];
                        if (lMin < min) {
                            min = lMin;
                        }
                    }
                }
                if (tmpX + 1 < 16 /*&& tmpY - 1 > 0*/ && !eastwalls[tmpX][tmpY] && visited[tmpX+1][tmpY] == 0 && botvisited[tmpX][tmpY] == 1) {
                    if (tmpX+1 < 16 && visited[tmpX+1][tmpY] == 0) {
                        xposes.push(tmpX+1);
                        yposes.push(tmpY);
                    }
                    if (mazetable[tmpX+1][tmpY] > cur) {
                        rMin = mazetable[tmpX+1][tmpY];
                        if (rMin < min) {
                            min = rMin;
                        }
                    }
                    
                }
                if (/*tmpX - 1 > 0 && */tmpY - 1 > 0 && !southwalls[tmpX][tmpY] && visited[tmpX][tmpY-1] == 0 && botvisited[tmpX][tmpY] == 1) {
                    if (tmpY-1 > 0 && visited[tmpX][tmpY-1] == 0) {
                        xposes.push(tmpX);
                        yposes.push(tmpY-1);
                    }
                    if (mazetable[tmpX][tmpY-1] > cur) {
                        bMin = mazetable[tmpX][tmpY-1];
                        if (bMin < min) {
                            min = bMin;
                        }
                    }
                }
                /*log("fMin: " + std::to_string(fMin));
                log("lMin: " + std::to_string(lMin));
                log("rMin: " + std::to_string(rMin));
                log("bMin: " + std::to_string(bMin));*/

                if (min > cur && min != 1000) {
                    /*log("updating value");
                    log("tmpX: " + std::to_string(tmpX));
                    log("tmpY: " + std::to_string(tmpY));*/
                    mazetable[tmpX][tmpY] = min+1;
                    //API::setText(tmpX,tmpY,std::to_string(mazetable[tmpX][tmpY]));
                }
            }


            continue;
        }

        
    }
}