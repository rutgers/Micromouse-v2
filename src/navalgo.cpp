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

const int N = 16;   // maze size
int curX = 0;
int curY = 0;
char curD = 'n';    // current direction

// idk why i have this, it can stay for funzzys
std::vector<std::vector<bool>> maze(N, std::vector<bool>(N,false));

// ---
std::vector<std::vector<bool>> hWall(N, std::vector<bool>(N-1,false));

// ||||
std::vector<std::vector<bool>> vWall(N-1, std::vector<bool>(N,false));

// used for tree structure, honesty g and h are not that useful? but why not
struct Node {
  int x, y;
  float g, h;
  Node* parent;
};

struct Compare {
    bool operator() (Node* a, Node* b) {
        return h(a->x, a->y) + a->g > b->g + h(a->x, a->y);
    }
};

// heuristic function
int h(int x, int y){
  return (int) (floor(abs(7.5-x)) + floor(abs(7.5-y)));
}

// update front/left/right walls info into vWall/hWall
void update() {
  if (curD == 'n'){
    if (/*front no wall*/&&(curY!=N-1)){
      hWall[curX][curY] = true;
    }
    if (/*right no wall*/&&(curX!=N-1)){
      vWall[curX][curY] = true;
    }
    if (/*left no wall*/&&(curX!=0)){
      vWall[curX-1][curY] = true;
    }
    
  }else if(curD == 's'){
    if (/*front no wall*/&&(curY!=0)){
      hWall[curX][curY-1] = true;
    }
    if (/*right no wall*/&&(curX!=0)){
      vWall[curX-1][curY] = true;
    }
    if (/*left no wall*/&&(curX!=N-1)){
      vWall[curX][curY] = true;
    }
  }else if(curD == 'e'){
    if (/*front no wall*/&&(curX!=N-1)){
      vWall[curX][curY] = true;
    }
    if (/*right no wall*/&&(curX!=0)){
      hWall[curX][curY-1] = true;
    }
    if (/*left no wall*/&&(curX!=N-1)){
      hWall[curX][curY] = true;
    }
  }else{
    if (/*front no wall*/&&(curX!=0)){
      vWall[curX-1][curY] = true;
    }
    if (/*right no wall*/&&(curX!=N-1)){
      hWall[curX][curY] = true;
    }
    if (/*left no wall*/&&(curX!=0)){
      hWall[curX][curY-1] = true;
    }
  }
}

// move 1 block towards given abs direction.
void move( char movD ){
  if (movD == curD){
    // nothing
  } else if ((movD == 's' && curD == 'n') || (movD == 'n' && curD == 's') || (movD == 'e' && curD == 'w') || (movD == 'w' && curD == 'e')) {
    // turn 180
    pidrotate_instance->rotate_to_angle(imu_instance->getHeading()+180);
  } else if ((movD == 'e' && curD == 'n') || (movD == 's' && curD == 'e') || (movD == 'w' && curD == 's') || (movD == 'n' && curD == 'w')) {
    // turn right 90
    pidrotate_instance->rotate_to_angle(imu_instance->getHeading()+90);
  } else {
    // turn left 90
    pidrotate_instance->rotate_to_angle(imu_instance->getHeading()-90);
  }
  // move forward
  pidstraight_instance->drive_to_position(7);

  curD = movD;
}

// give which direction to move (< 1 block)
char block_move(int s_x, int s_y, int e_x, int e_y){
  if(s_x == e_x)
    if(s_y < e_y)
      return 'n';
    else
      return 's';
  else
    if(s_x < e_x)
      return 'e';
    else
      return 'w';
}

std::vector<Node*> find_path(Node* s, Node* e) {
  // int start_x = s->x, start_y = s->y, end_x = g->x, end_y = g->y;
  Node* s_ = s;
  Node* e_ = e;
  std::vector<Node*> sv;
  std::vector<Node*> ev;
  while((s_->x != e_->x) || (s_->y!=e_->y)) {
    if (s_->g > e_->g ) {
      sv.push_back(s_);
      s_ = s_->parent;
    } else if (s_->g < e_->g) {
      ev.push_back(e_);
      e_ = e_->parent;
    } else {
      sv.push_back(s_);
      ev.push_back(e_);
      s_ = s_->parent;
      e_ = e_->parent;
    }
  }
  std::reverse(ev.begin(), ev.end());
  sv.insert(sv.end(), ev.begin(), ev.end());
  curX = e->x;
  curY = e->y;
  return sv;
}


bool validIndex(int x, int y) {
  return (x >= 0) && (x < N) && (y >= 0) && (y < N);
}

std::vector<Node*> getNeighbors(Node* node, std::vector<std::vector<bool>>& maze) {
  std::vector<Node*> neighbors;
  int x = node->x, y = node->y;
  int nextX = x;
  int nextY = y;

  // N wall
  nextY++;
  if (validIndex(nextX, nextY) && !hWall[x][y]) {
    Node* newNode;
    newNode->x = nextX;
    newNode->y = nextY;
    neighbors.push_back(newNode);
  }
  nextY--;

  // E wall
  nextX++;
  if (validIndex(nextX, nextY) && !vWall[x][y]) {
    Node* newNode;
    newNode->x = nextX;
    newNode->y = nextY;
    neighbors.push_back(newNode);
  }
  nextX--;

  // S wall
  nextY--;
  if (validIndex(nextX, nextY) && !vWall[x][y-1]) {
    Node* newNode;
    newNode->x = nextX;
    newNode->y = nextY;
    neighbors.push_back(newNode);
  }
  nextY++;

  // E wall
  nextX--;
  if (validIndex(nextX, nextY) && !vWall[x-1][y]) {
    Node* newNode;
    newNode->x = nextX;
    newNode->y = nextY;
    neighbors.push_back(newNode);
  }
  nextX++;

  return neighbors;
}


void loop() {

  std::priority_queue<Node, std::vector<Node>, Compare> pq;

  while (!pq.empty()) {
    // pop top, and move there (how tf to move there when its far)

    // check front/left/right getNeighbor method-
    // enque them
  }
}