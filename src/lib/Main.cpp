#include "API.h"
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <cmath>
#include <algorithm>
#include <sstream>

void log(const std::string& text) {
    std::cerr << text << std::endl;
}

const int N = 16;   // maze size
char curD = 'n';    // current direction

std::vector<std::vector<bool>> open(N, std::vector<bool>(N,false));
std::vector<std::vector<bool>> closed(N, std::vector<bool>(N,false));
std::vector<std::vector<int>> g(N, std::vector<int>(N,2147483647));

// ---
std::vector<std::vector<bool>> hWall(N, std::vector<bool>(N-1,false));

// ||||
std::vector<std::vector<bool>> vWall(N-1, std::vector<bool>(N,false));

// used for tree structure
struct Node {
  int x, y;
  Node* parent;
};
Node* cur ;

// heuristic function
int h(int x, int y){
  return (int) (floor(abs(7.5-x)) + floor(abs(7.5-y)));
}

struct Compare {
    bool operator() ( Node* a, Node* b) {
      return g[a->x][a->y]+h((a->x),(a->y)) > g[b->x][b->y]+h((b->x),(b->y));
    }
};

// update front/left/right walls info into vWall/hWall
void update() {
  if (curD == 'n'){
    if (API::wallFront()&&(cur->y!=N-1)){
      hWall[cur->x][cur->y] = true;
    }
    if (API::wallRight()&&(cur->x!=N-1)){
      vWall[cur->x][cur->y] = true;
    }
    if (API::wallLeft()&&(cur->x!=0)){
      vWall[cur->x-1][cur->y] = true;
    }
    
  }else if(curD == 's'){
    if (API::wallFront()&&(cur->y!=0)){
      hWall[cur->x][cur->y-1] = true;
    }
    if (API::wallRight()&&(cur->x!=0)){
      vWall[cur->x-1][cur->y] = true;
    }
    if (API::wallLeft()&&(cur->x!=N-1)){
      vWall[cur->x][cur->y] = true;
    }
  }else if(curD == 'e'){
    if (API::wallFront()&&(cur->x!=N-1)){
      vWall[cur->x][cur->y] = true;
    }
    if (API::wallRight()&&(cur->y!=0)){
      hWall[cur->x][cur->y-1] = true;
    }
    if (API::wallLeft()&&(cur->y!=N-1)){
      hWall[cur->x][cur->y] = true;
    }
  }else{
    if (API::wallFront()&&(cur->x!=0)){
      vWall[cur->x-1][cur->y] = true;
    }
    if (API::wallRight()&&(cur->y!=N-1)){
      hWall[cur->x][cur->y] = true;
    }
    if (API::wallLeft()&&(cur->y!=0)){
      hWall[cur->x][cur->y-1] = true;
    }
  }
}

// move 1 block towards given abs direction.
void move( char movD ){
  if (movD == curD){
    // nothing
  } else if ((movD == 's' && curD == 'n') || (movD == 'n' && curD == 's') || (movD == 'e' && curD == 'w') || (movD == 'w' && curD == 'e')) {
    // turn 180
    API::turnRight();
    API::turnRight();
    // pidrotate_instance->rotate_to_angle(imu_instance->getHeading()+180);
  } else if ((movD == 'e' && curD == 'n') || (movD == 's' && curD == 'e') || (movD == 'w' && curD == 's') || (movD == 'n' && curD == 'w')) {
    // turn right 90
    API::turnRight();
    // pidrotate_instance->rotate_to_angle(imu_instance->getHeading()+90);
  } else {
    // turn left 90
    API::turnLeft();
    // pidrotate_instance->rotate_to_angle(imu_instance->getHeading()-90);
  }
  // move forward
  // pidstraight_instance->drive_to_position(7);
    API::moveForward();

  curD = movD;
}

// give which direction to move (< 1 block)
char find_direction(int s_x, int s_y, int e_x, int e_y){
  if(s_x == e_x){
    if(s_y < e_y)
      return 'n';
    else
      return 's';
  }else{
    if(s_x < e_x)
      return 'e';
    else
      return 'w';
  }
  return 'x';
}

std::vector<Node*> find_path(Node* s, Node* e) {
  Node* s_ = s;
  Node* e_ = e;
  std::vector<Node*> sv;
  std::vector<Node*> ev;
  
  sv.push_back(s_);
  ev.push_back(e_);



  while( sv.back()->x!=ev.back()->x || sv.back()->y!=ev.back()->y ) {
    if (s_== nullptr || e_== nullptr){
      break;
    }
    if (g[s_->x][s_->y] > g[e_->x][e_->y] ) {
      // log("test1"+ std::to_string(g[s_->x][s_->y])+std::to_string(g[e_->x][e_->y]));
      sv.push_back(s_); 
      s_ = s_->parent;
    } else if (g[s_->x][s_->y] < g[e_->x][e_->y]) {
      // log("test2"+ std::to_string(g[s_->x][s_->y])+std::to_string(g[e_->x][e_->y]));
      ev.push_back(e_);
      e_ = e_->parent;
    } else {
      // log("test3"+ std::to_string(g[s_->x][s_->y])+std::to_string(g[e_->x][e_->y]));
      sv.push_back(s_);
      ev.push_back(e_);
      s_ = s_->parent;
      e_ = e_->parent;
    }
  }
  // for(auto x:sv){
  //   log("sv: "+ std::to_string(x->x) + std::to_string(x->y));
  // }
  // for(auto x:ev){
  //   log("ev: "+ std::to_string(x->x) + std::to_string(x->y));
  // }

  ev.pop_back();
  sv.erase(sv.begin());
  ev.erase(ev.begin());
  
  std::reverse(ev.begin(), ev.end());
  sv.insert(sv.end(), ev.begin(), ev.end());
  // for(auto x:sv){
  //   log("fv: "+ std::to_string(x->x) + std::to_string(x->y));
  // }
  return sv;
}

int move_path(Node* s, Node* e){
  auto path = find_path(s,e);
  auto prev = path[0];

  for(int i=1; i<path.size(); i++){
    char step_direction = find_direction( prev->x, prev->y, path[i]->x, path[i]->y);
    std::string sd( 1, step_direction );
    // log("stepping towards: "+sd);
    move(step_direction);
    prev = path[i];
  }

  return 1;
}

bool validIndex(int x, int y) {
  return (x >= 0) && (x < N) && (y >= 0) && (y < N);
}

std::vector<Node*> getNeighbors(Node* node) {
  std::vector<Node*> neighbors;
  int x = node->x, y = node->y;
  int nextX = x;
  int nextY = y;

  // N wall
  nextY++;
  if (validIndex(nextX, nextY) && !hWall[x][y]) {
    Node* newNode = new Node();
    newNode->x = nextX;
    newNode->y = nextY;
    newNode->parent=node;
    neighbors.push_back(newNode);
  }
  nextY--;

  // E wall
  nextX++;
  if (validIndex(nextX, nextY) && !vWall[x][y]) {
    Node* newNode = new Node();
    newNode->x = nextX;
    newNode->y = nextY;
    newNode->parent=node;
    neighbors.push_back(newNode);
  }
  nextX--;

  // S wall
  nextY--;
  if (validIndex(nextX, nextY) && !hWall[x][y-1]) {
    Node* newNode = new Node();
    newNode->x = nextX;
    newNode->y = nextY;
    newNode->parent=node;
    neighbors.push_back(newNode);
  }
  nextY++;

  // W wall
  nextX--;
  if (validIndex(nextX, nextY) && !vWall[x-1][y]) {
    Node* newNode = new Node();
    newNode->x = nextX;
    newNode->y = nextY;
    newNode->parent=node;
    neighbors.push_back(newNode);
  }
  nextX++;

  return neighbors;
}


int main(int argc, char* argv[]) {
    log("Running...");
    API::setColor(0, 0, 'G');
    API::setText(0, 0, "abc");
    std::vector<Node*> q;

    cur = new Node();
    Node* origin = cur;
    open[0][0] = true;
    closed[0][0] = true;
    g[0][0] = 0;
    cur->x = 0;
    cur->y = 0;
    
    q.push_back(cur);
    update();

    // for(auto x:q){
    //   log( std::to_string(x->x) +  std::to_string(x->y));
    // }

while (!q.empty()) {
  // log("----------------------------------------");
  //   for (const auto& x : g) {
  //       std::ostringstream oss;
  //       for (size_t i = 0; i < x.size(); ++i) {
  //         int cp = x[i];
  //         if (cp== 2147483647) cp = -1;
  //           oss << cp;
  //           if (i != x.size() - 1) {
  //               oss << "\t";
  //           }
  //       }
  //       std::string str = oss.str();
  //       log(str);
  //   }
  //   log("----------------------------------------");
  Node* node_exploring;
  float lowest_f = std::numeric_limits<float>::max(); // Use numeric_limits to get the maximum float value
  int lowest_index = 0;
  
  for (int i = 0; i < q.size(); i++) {
    auto x = q[i];
    // log("in q: " + std::to_string(x->x) + std::to_string(x->y));
    float cur_f = g[x->x][x->y] + h(x->x, x->y);
    
    if (cur_f < lowest_f) {
      lowest_f = cur_f;
      node_exploring = x;
      lowest_index = i;
    }
  }
  
  // Move only if the current and exploring nodes are different
  if (cur != node_exploring) {
    // log("moving path: " + std::to_string(cur->x) + std::to_string(cur->y) + " to " + std::to_string(node_exploring->x) + std::to_string(node_exploring->y));
    move_path(cur, node_exploring);
    cur = node_exploring;
  }
  q.erase(q.begin() + lowest_index);
  
  update();
  // Check for the goal
  if ((cur->x == 8 || cur->x == 7) && (cur->y == 8 || cur->y == 7)) {
    log("foun");
    // shortest path:
    Node* temp = cur;
    while( temp->x!=0 || temp->y!=0 ){
      log(std::to_string(temp->x)+std::to_string(temp->y));
      temp = temp->parent;
    }
    move_path(cur, origin);
    return 1;
  }
  
  std::vector<Node*> neighbors = getNeighbors(cur);
  
  for (auto neighbor : neighbors) {
    int neighbor_x = neighbor->x;
    int neighbor_y = neighbor->y;
    
    if (open[neighbor_x][neighbor_y]) {
      if (g[cur->x][cur->y] + 1 >= g[neighbor_x][neighbor_y]) continue;
    } else if (closed[neighbor_x][neighbor_y]) {
      if (g[cur->x][cur->y] + 1 >= g[neighbor_x][neighbor_y]) continue;
      
      neighbor->parent = cur;
      q.push_back(neighbor);
      open[cur->x][cur->y] = true;
      closed[cur->x][cur->y] = false;
    } else {
      neighbor->parent = cur;
      q.push_back(neighbor);
      open[neighbor_x][neighbor_y] = true;
    }
    
    g[neighbor_x][neighbor_y] = g[cur->x][cur->y] + 1;
  }
  
  closed[cur->x][cur->y] = true;
}

}