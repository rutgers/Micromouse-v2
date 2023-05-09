#include <Arduino.h>
#include <SPI.h>
#include "lib\Motors.h"
#include "lib\PIDRotate.h"
#include "lib\PIDStraight.h"
#include "lib\PIDMagicStraight.h"
#include "lib\tof.h"
#include "lib\IMU.h"
#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <algorithm>

// A* 

const int N = 16;
int curX = 0;
int curY = 0;
char curD = 'N';



std::vector<std::vector<bool>> maze(N, std::vector<bool>(N,false));
// ---
std::vector<std::vector<bool>> hWall(N, std::vector<bool>(N-1,false));

// ||||
std::vector<std::vector<bool>> vWall(N-1, std::vector<bool>(N,false));


void move( char movD ){
  if (movD == curD){
    // nothing
  }else if ( (movD=='s'&&curD=='n') || (movD=="n"&&curD=='s') || (movD=='e'&&curD=='w') || (movD=='w'&&curD=='e') ){
    // turn 180
  }else if( (movD=='e'&&curD=='n') || (movD=="s"&&curD=='e') || (movD=='w'&&curD=='s') || (movD=='n'&&curD=='w') ){
    // turn right 90
  }else{
    // turn left 90
  }
  // move forward
  curD = movD;
}

void update(){
  // call for sensor and update hWall& vWall;
}

struct Node {
  int x, y;
  float g, h;
  Node* parent;

};

struct Compare {
    bool operator()(const Node* a, const Node* b) {
        return h(a->x, a->y) + a->g > b->g + h(a->x, a->y);
    }
};

bool validIndex(int x, int y) {
  return x >= 0 && x < N && y >= 0 && y < N;
}

std::vector<Node*> getNeighbors(Node* node, std::vector<std::vector<bool>>& maze) {
  std::vector<Node*> neighbors;
  int x = node->x, y = node->y;
  int nextX = x;
  int nextY = y;

  // N wall
  nextY ++;
  if ( validIndex(nextX, nextY) && !hWall[x][y] ){
    Node* newNode;
    newNode->x = newX;
    newNode->y = newY;
    neighbors.pushback( newNode );
  }
  nextY --;

  // E wall
  nextX ++;
  if ( validIndex(nextX, nextY) && !vWall[x][y] ){
    Node* newNode;
    newNode->x = newX;
    newNode->y = newY;
    neighbors.pushback( newNode );
  }
  nextX--;

  // S wall
  nextY --
  if ( validIndex(nextX, nextY) && !vWall[x][y-1] ){
    Node* newNode;
    newNode->x = newX;
    newNode->y = newY;
    neighbors.pushback( newNode );
  }
  nextY++;


  // E wall
  nextX --;
  if ( validIndex(nextX, nextY) && !vWall[x-1][y] ){
    Node* newNode;
    newNode->x = newX;
    newNode->y = newY;
    neighbors.pushback( newNode );
  }
  nextX++;

  return neighbors;
}


std::priority_queue<array<int>> openPQ;




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
  
  //Serial.println("looping");

  /*Serial.print("encoder A positon: ");
  Serial.println(ENCA.read());
  
  Serial.print("encoder B positon: ");
  Serial.println(ENCB.read());*/

  //motors_instance->setMotorsSpeed((int)floor(pidrotate_instance->rotate_to_angle(180)));
  //motors_instance->setMotorsSpeed((int)floor(pidstraight_instance->drive_to_position(10)));
  //pidmagicstraight_instance->drive_straight_with_magic(30);

  
  //if (!pidrotate_instance->exited) {
  //  pidrotate_instance->rotate_to_angle(90);
  //}
  

  /*if (!pidmagicstraight_instance->exited) {
    Serial.println("exited = false");
    pidmagicstraight_instance->drive_straight_with_magic(10);
  }*/

while (true) {
  
  /*Serial.print("left dist: ");
  Serial.println(tof_instance->readL());
  Serial.print("front dist: ");
  Serial.println(tof_instance->readF());
  Serial.print("right dist: ");
  Serial.println(tof_instance->readR());*/


  if (tof_instance->readF() > 100) {
    pidstraight_instance->drive_to_position(7);
  } else if (tof_instance->readL() > 100) {
    pidrotate_instance->rotate_to_angle(imu_instance->getHeading()-90);
  } else if (tof_instance->readR() > 100) {
    pidrotate_instance->rotate_to_angle(imu_instance->getHeading()+90);
  }

  delay(500);
  pidrotate_instance->rotate_to_angle(round(imu_instance->getHeading()/90.0)*90.0);
  delay(500);

}




  /*pidmagicstraight_instance->drive_straight_with_magic(10);
  delay(500);
  pidrotate_instance->rotate_to_angle(-45);
  delay(500);
  pidrotate_instance->rotate_to_angle(45);
  delay(500);
  pidrotate_instance->rotate_to_angle(-45);
  delay(500);
  pidrotate_instance->rotate_to_angle(45);
  delay(500);
  pidrotate_instance->rotate_to_angle(0);
  delay(500);
  pidmagicstraight_instance->drive_straight_with_magic(-10);
  delay(5000);*/
  // put your main code here, to run repeatedly:
  // motors->setLeftMotorDirection(true);
  // motors->setRightMotorDirection(true);

  // motors->setLeftMotorSpeed(64);
  // motors->setRightMotorSpeed(64);

  priority_queue<Node, vector<Node>, Compare> pq;

  while( !pq.empty ){
    // pop top, and move there (how tf to move there when its far)

    // check front/left/right getNeighbor method-
    // enque them
  }
}