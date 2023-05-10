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
char curD = 'N';    // current direction

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

void update() {
  // call for sensor and update hWall& vWall;
}

std::vector<Node*> find_path(const Node* s, const Node* e) {
  // int start_x = s->x, start_y = s->y, end_x = g->x, end_y = g->y;
  Node* s_ = s;
  Node* e_ = e;
  std::vector<Node*> sv;
  std::vector<Node*> ev;
  while((s_->x != e_->x) || (s_->y!=e_->y)) {
    if (s_->g < e_->g ) {
      sv.push_back(s_);
      s_ = s_->parent;
    } else if (s_->g > e_->g) {
      ev.push_back(e_);
      e_ = e_->parent;
    } else {
      sv.push_back(s_);
      ev.push_back(e_);
      s_ = s_->parent;
      e_ = e_->parent;
    }
  }
  ev = std::reverse(ev.begin, ev.end);
  return vector1.insert(sv.end(), ev.begin(), ev.end());
}

struct Compare {
    bool operator() (const Node* a, const Node* b) {
        return h(a->x, a->y) + a->g > b->g + h(a->x, a->y);
    }
};

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
    neighbors.pushback(newNode);
  }
  nextY--;

  // E wall
  nextX++;
  if (validIndex(nextX, nextY) && !vWall[x][y]) {
    Node* newNode;
    newNode->x = nextX;
    newNode->y = nextY;
    neighbors.pushback(newNode);
  }
  nextX--;

  // S wall
  nextY--;
  if (validIndex(nextX, nextY) && !vWall[x][y-1]) {
    Node* newNode;
    newNode->x = nextX;
    newNode->y = nextY;
    neighbors.pushback(newNode);
  }
  nextY++;

  // E wall
  nextX--;
  if (validIndex(nextX, nextY) && !vWall[x-1][y]) {
    Node* newNode;
    newNode->x = nextX;
    newNode->y = nextY;
    neighbors.pushback(newNode);
  }
  nextX++;

  return neighbors;
}

std::priority_queue<array<int>> openPQ;

void loop() {

  priority_queue<Node, vector<Node>, Compare> pq;

  while (!pq.empty) {
    // pop top, and move there (how tf to move there when its far)

    // check front/left/right getNeighbor method-
    // enque them
  }
}