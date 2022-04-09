#include "bot_config.h"

// Motor Driver Standby Pin (Drive to HIGH to make motor driver function)
#define STBY 5

double leftTimer = 0;
double rightTimer = 0;

double rotateTimer = 0;

double goal_threshold = 45;
double time_threshold = 30;

double goal_threshold_deg = 10;

sensors_event_t g, ac, t, m;

struct pid_return {
  double motor_output;
  bool within_goal;
};

struct pid_return pid_retA;
struct pid_return pid_retB;
struct pid_return pid_retRA;
struct pid_return pid_retRB;

double timeStep = 10000;

Adafruit_Mahony imu_filter;
#define FILTER_UPDATE_RATE_HZ 100
#define PRINT_EVERY_N_UPDATES 10
extern elapsedMillis imu_filter_timestamp;

double zanglegyro = 0;
double zangleaccel = 0;

double rotation_offset = 0;

bool apple = false;

void resetRotation();
void rotate(int ang);
void straight(int dist);
void black_magic(int dist);
bool callStraight(double distance);
bool callBlackMagic(double dist);
bool callRotate(double ang);
struct pid_return* pid(int controller, double target, double kP, double kI, double kD);
struct pid_return* pid_rotate(int controller, double target, double kP, double kI, double kD);
struct pid_return* pid_drive_straight_with_rotate(int controller, double target, double kP, double kI, double kD);
void reset_pid();

std::vector<std::tuple<int,int>> A_star(int x_s, int y_s, char dir);

void setup() {
  Serial.begin(9600);

  delay(6500);

  resetRotation();
  reset_pid();

  Serial.println(0);
  IMU.begin_I2C();
  IMU.enableGyrolDLPF(true, ICM20X_GYRO_FREQ_361_4_HZ);
  IMU.setAccelRateDivisor(0);
  IMU.setGyroRateDivisor(0);
  accel = IMU.getAccelerometerSensor();
  gyro = IMU.getGyroSensor();
//   magn = IMU.getMagnetometerSensor();


  pinMode(PWMA, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);

  pinMode(STBY, OUTPUT);

  digitalWrite(STBY, HIGH);
}

void loop() {

  IMU.getEvent(&ac, &g, &t, &m);

  imu_filter.update((g.gyro.x-.0053) * SENSORS_RADS_TO_DPS, (g.gyro.y-.016) * SENSORS_RADS_TO_DPS, (g.gyro.z-.0117) * SENSORS_RADS_TO_DPS, ac.acceleration.x, ac.acceleration.y, ac.acceleration.z, 0, 0, 0);

  if (!apple) {
    reset_pid();
    //black_magic(20);
    A_star(0,0,'A');
  }
  apple = true;

}

void resetRotation() {
  rotation_offset = imu_filter.getYaw();
}

void rotate(int ang) {
  while(!callRotate(ang)) {
     IMU.getEvent(&ac, &g, &t, &m);

     imu_filter.update((g.gyro.x-.0053) * SENSORS_RADS_TO_DPS, (g.gyro.y-.016) * SENSORS_RADS_TO_DPS, (g.gyro.z-.0117) * SENSORS_RADS_TO_DPS, ac.acceleration.x, ac.acceleration.y, ac.acceleration.z, 0, 0, 0);

  }
  Serial.println("returned true");
  reset_pid();
}

void straight(int dist) {
  while(!callStraight(dist)) {
    
  }
  reset_pid();
}

void black_magic(int dist) {
  while (!callBlackMagic(dist)) {
    IMU.getEvent(&ac, &g, &t, &m);

    imu_filter.update((g.gyro.x-.0053) * SENSORS_RADS_TO_DPS, (g.gyro.y-.016) * SENSORS_RADS_TO_DPS, (g.gyro.z-.0117) * SENSORS_RADS_TO_DPS, ac.acceleration.x, ac.acceleration.y, ac.acceleration.z, 0, 0, 0);

  }
  reset_pid();
}

bool callStraight(double distance) {

  double ticks_per_inch = (360 / (1.57 * PI));

  struct pid_return* a;
  struct pid_return* b;
  
  double goal = distance * ticks_per_inch;

  a = pid(0, -goal, .6, .00000000, .0); //pid(0, -inch_goal,1.8,.00000000,.0);
  b = pid(1, goal, .6, .00000000, .0); //pid(1, inch_goal,1.8,.00000000,.0);

  if (a->motor_output < 0) {
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH);
  } else {
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);
  }

  if (b->motor_output > 0) {
    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, LOW);
  } else {
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, HIGH);
  }
  
  analogWrite(PWMA, abs(a->motor_output));
  analogWrite(PWMB, abs(b->motor_output));

  if (a->within_goal == 1 && b->within_goal == 1) {
    analogWrite(PWMA, 0);
    analogWrite(PWMB, 0);
    reset_pid();
    return true;
  }

  return false;

}

bool callBlackMagic(double dist) {

  struct pid_return* a;
  struct pid_return* b;
  
  double goal = dist;

  a = pid_drive_straight_with_rotate(0, goal, .6, .000, 50000);//a = pid_rotate(0, goal, .6, .001, 50000); //.00000000
  b = pid_drive_straight_with_rotate(1, goal, .6, .000, 50000);//b = pid_rotate(1, goal, .6, .001, 50000);
  b->motor_output *= -1;

  if (a->motor_output < 0) {
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH);
  } else {
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);
  }

  if (b->motor_output > 0) {
    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, LOW);
  } else {
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, HIGH);
  }

  analogWrite(PWMA, abs(a->motor_output));
  analogWrite(PWMB, abs(b->motor_output));

  if (a->within_goal == 1 && b->within_goal == 1) {
    Serial.println("exiting");
    analogWrite(PWMA, 0);
    analogWrite(PWMB, 0);
    reset_pid();
    return true;
  }
  return false;

}

bool callRotate(double ang) {

  struct pid_return* a;
  struct pid_return* b;
  
  double goal = ang;

  a = pid_rotate(0, goal, .6, .000, 50000);//a = pid_rotate(0, goal, .6, .001, 50000); //.00000000
  b = pid_rotate(1, goal, .6, .000, 50000);//b = pid_rotate(1, goal, .6, .001, 50000);
  b->motor_output *= -1;

  if (a->motor_output < 0) {
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH);
  } else {
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);
  }

  if (b->motor_output > 0) {
    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, LOW);
  } else {
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, HIGH);
  }

  analogWrite(PWMA, abs(a->motor_output));
  analogWrite(PWMB, abs(b->motor_output));

  if (a->within_goal == 1 && b->within_goal == 1) {
    Serial.println("exiting");
    analogWrite(PWMA, 0);
    analogWrite(PWMB, 0);
    reset_pid();
    return true;
  }
  return false;

}

double last_errorA = 0.0;
double errorA = 0.0;
double total_errorA = 0.0;

double last_errorB = 0.0;
double errorB = 0.0;
double total_errorB = 0.0;

double last_errorRA = 0.0;
double errorRA = 0.0;
double total_errorRA = 0.0;

double last_errorRB = 0.0;
double errorRB = 0.0;
double total_errorRB = 0.0;


struct pid_return* pid(int controller, double target, double kP, double kI, double kD) {
  // CONTROLLER: 1 == LEFT 2 == RIGHT

  bool within_goal = 0;
  double return_value = 0;

  if (controller == 0) {
    last_errorA = errorA;
    errorA = (target - ENCA.read());
    total_errorA += errorA;

    Serial.print("errorA: ");
    Serial.println(errorA);

    return_value = 255 * tanh((kP * errorA + kD * (errorA - last_errorA) / (timeStep) + kI * total_errorA) / 100);

    if (abs(errorA) < goal_threshold) {
      leftTimer++;
    } else {
      leftTimer = 0;
    }

    if (leftTimer > time_threshold) {
      within_goal = 1;
    }


  } else {
    last_errorB = errorB;
    errorB = -(target - ENCB.read());
    total_errorB += errorB;
    
    Serial.print("errorB: ");
    Serial.println(errorB);

    return_value = 255 * tanh((kP * errorB + kD * (errorB - last_errorB) / (timeStep) + kI * total_errorB) / 100);


    if (abs(errorB) < goal_threshold) {
      rightTimer++;
    } else {
      rightTimer = 0;
    }

    if (rightTimer > time_threshold) {
      within_goal = 1;
    }
  }

  if (controller == 0) {
    pid_retA.motor_output = return_value;
    pid_retA.within_goal = within_goal;
    return &pid_retA;
  } else {
    pid_retB.motor_output = return_value;
    pid_retB.within_goal = within_goal;
    return &pid_retB;
  }
}

struct pid_return* pid_rotate(int controller, double target, double kP, double kI, double kD) {
  // CONTROLLER: 1 == LEFT 2 == RIGHT

  if (controller == 0) {
    bool within_goal = 0;
    double return_value = 0;
  
    last_errorRA = errorRA;
    errorRA = remainder((target*0.70 - (imu_filter.getYaw() - rotation_offset)),360.0);
    Serial.print("target: ");
    Serial.println(target);
    Serial.print("rotation offset: ");
    Serial.println(rotation_offset);
    Serial.print("pre mod: ");
    Serial.println(target*0.70 - imu_filter.getYaw() - rotation_offset);
    total_errorRA += errorRA;
  
    return_value = -255 * tanh((kP * errorRA + kD * (errorRA - last_errorRA) / (timeStep) + kI * total_errorRA) / 100);
  
    Serial.print("errorA: ");
    Serial.println(errorRA);
    Serial.print("returnA: ");
    Serial.println(return_value);
    Serial.print("preturnA: ");
    Serial.println(kP * errorRA * -2.55);
    Serial.print("dreturnA: ");
    Serial.println(kD * (errorRA - last_errorRA) / (timeStep) * -2.55);
  
    if (abs(errorRA) < goal_threshold_deg) {
      rotateTimer++;
      Serial.println("A in threshold");
    } else {
      rotateTimer = 0;
    }
  
    if (rotateTimer > time_threshold) {
      within_goal = 1;
    }
  
    pid_retRA.motor_output = return_value;
    pid_retRA.within_goal = within_goal;
    return &pid_retRA;
  } else {

    bool within_goal = 0;
    double return_value = 0;
  
    last_errorRB = errorRB;
    errorRB = remainder((target*0.70 - (imu_filter.getYaw() - rotation_offset)),360.0);
    total_errorRB += errorRB;
  
    return_value = -255 * tanh((kP * errorRB + kD * (errorRB - last_errorRB) / (timeStep) + kI * total_errorRB) / 100);
  
    Serial.print("errorB: ");
    Serial.println(errorRB);
    Serial.print("returnB: ");
    Serial.println(return_value);
    Serial.print("preturnB: ");
    Serial.println(kP * errorRB * -2.55);
    Serial.print("dreturnB: ");
    Serial.println(kD * (errorRB - last_errorRB) / (timeStep) * -2.55);
    Serial.print("angle: ");
    Serial.println(imu_filter.getYaw());
    Serial.print("corrected angle: ");
    Serial.println(imu_filter.getYaw() - rotation_offset);

  
    if (abs(errorRB) < goal_threshold_deg) {
      rotateTimer++;
      Serial.println("A in threshold");
    } else {
      rotateTimer = 0;
    }
  
    if (rotateTimer > time_threshold) {
      within_goal = 1;
    }
  
    pid_retRB.motor_output = return_value;
    pid_retRB.within_goal = within_goal;
    return &pid_retRB;
  }
}

struct pid_return* pid_drive_straight_with_rotate(int controller, double target, double kP, double kI, double kD) {
  // CONTROLLER: 1 == LEFT 2 == RIGHT
  
  if (controller == 0) {
    bool within_goal = 0;
    double return_value = 0;
  
    last_errorRA = errorRA;
    errorRA = remainder((90*0.70 - (imu_filter.getYaw() - rotation_offset)),360.0);
    Serial.print("target: ");
    Serial.println(target);
    Serial.print("rotation offset: ");
    Serial.println(rotation_offset);
    Serial.print("pre mod: ");
    Serial.println(target*0.70 - imu_filter.getYaw() - rotation_offset);
    total_errorRA += errorRA;
  
    return_value = -255 * tanh((kP * errorRA + kD * (errorRA - last_errorRA) / (timeStep) + kI * total_errorRA) / 100);
  
    Serial.print("errorA: ");
    Serial.println(errorRA);
    Serial.print("returnA: ");
    Serial.println(return_value);
    Serial.print("preturnA: ");
    Serial.println(kP * errorRA * -2.55);
    Serial.print("dreturnA: ");
    Serial.println(kD * (errorRA - last_errorRA) / (timeStep) * -2.55);
  
    if (abs(errorRA) < goal_threshold_deg) {
      rotateTimer++;
      Serial.println("A in threshold");
    } else {
      rotateTimer = 0;
    }
  
    if (rotateTimer > time_threshold) {
      within_goal = 1;
    }
  
    pid_retRA.motor_output = return_value;
    pid_retRA.within_goal = within_goal;
    return &pid_retRA;
  } else {

    bool within_goal = 0;
    double return_value = 0;
  
    last_errorRB = errorRB;
    errorRB = remainder((-90*0.70 - imu_filter.getYaw() - rotation_offset),360.0);
    total_errorRB += errorRB;
  
    return_value = -255 * tanh((kP * errorRB + kD * (errorRB - last_errorRB) / (timeStep) + kI * total_errorRB) / 100);
  
    Serial.print("errorB: ");
    Serial.println(errorRB);
    Serial.print("returnB: ");
    Serial.println(return_value);
    Serial.print("preturnB: ");
    Serial.println(kP * errorRB * -2.55);
    Serial.print("dreturnB: ");
    Serial.println(kD * (errorRB - last_errorRB) / (timeStep) * -2.55);
    Serial.print("angle: ");
    Serial.println(imu_filter.getYaw());
    Serial.print("corrected angle: ");
    Serial.println(imu_filter.getYaw() - rotation_offset);

  
    if (abs(errorRB) < goal_threshold_deg) {
      rotateTimer++;
      Serial.println("A in threshold");
    } else {
      rotateTimer = 0;
    }
  
    if (rotateTimer > time_threshold) {
      within_goal = 1;
    }
  
    pid_retRB.motor_output = return_value;
    pid_retRB.within_goal = within_goal;
    return &pid_retRB;
  }
}

void reset_pid() {
  total_errorA = 0.0;
  total_errorB = 0.0;

  ENCA.write(0);
  ENCB.write(0);
  total_errorRA = 0.0;
  total_errorRB = 0.0;

  resetRotation();
}


// ---------------------------------------------------------------------------------
using namespace std;
const int n = 16;
int EMPTY   = 0b0000;
int TOP     = 0b1000;
int BOT     = 0b0100;
int LEFT    = 0b0010;
int RIGHT   = 0b0001;

int wall[n][n] = {{0}};
bool inQ[n][n] = {{false}};
vector<vector<tuple<int,int>>> from(n, vector<tuple<int,int>>(n));
int  g_[n][n] = {{2147483646}};
int  f[n][n] = {{2147483646}};

bool* check_wall(){
    bool* ret = new bool[3];
    ret[0] = (L_ToF.readRangeContinuousMillimeters() < 90);
    ret[1] = (F_ToF.read() < 100);
    ret[2] = (R_ToF.readRangeContinuousMillimeters() < 90);
    return ret;
}

bool move_f(){
    straight(18);
}
bool turn_l(){
    rotate(-90);
    return true;
}
bool turn_r(){
    rotate(90);
    return true;
}


struct gcmp{
    bool operator()( tuple<int, int> a, tuple<int, int> b ){
        return  f[get<0>(a)][get<1>(a)] 
                            >
                f[get<0>(b)][get<1>(b)] ;
    }
};

char update(int x, int y, char dir, bool front, bool left, bool right){
    if( dir == 'n'){
        if(front) wall[x][y] = wall[x][y] | TOP;
        if(left)  wall[x][y] = wall[x][y] | LEFT;
        if(right) wall[x][y] = wall[x][y] | RIGHT;
    }
    if( dir == 'e'){
        if(front) wall[x][y] = wall[x][y] | RIGHT;
        if(left)  wall[x][y] = wall[x][y] | TOP;
        if(right) wall[x][y] = wall[x][y] | BOT;
    }
    if( dir == 's'){
        if(front) wall[x][y] = wall[x][y] | BOT;
        if(left)  wall[x][y] = wall[x][y] | RIGHT;
        if(right) wall[x][y] = wall[x][y] | LEFT;
    }
    if( dir == 'w'){
        if(front) wall[x][y] = wall[x][y] | LEFT;
        if(left)  wall[x][y] = wall[x][y] | BOT;
        if(right) wall[x][y] = wall[x][y] | TOP;
    }
    return dir;
}

char move1grid(char cur_dir, char goal){
    if(      //move forward
        cur_dir == goal 
    ){move_f();}
    else if( //turn 180
        ( cur_dir == 'n' && goal == 's')||
        ( cur_dir == 's' && goal == 'n')||
        ( cur_dir == 'e' && goal == 'w')||
        ( cur_dir == 'w' && goal == 'e')
    ){turn_l(); turn_l(); move_f(); }
    else if( //turn left
        ( cur_dir == 'n' && goal == 'w')||
        ( cur_dir == 's' && goal == 'e')||
        ( cur_dir == 'e' && goal == 'n')||
        ( cur_dir == 'w' && goal == 's')
    ){turn_l(); move_f();}
    else if( //turn right
        ( cur_dir == 'n' && goal == 'e')||
        ( cur_dir == 's' && goal == 'w')||
        ( cur_dir == 'e' && goal == 's')||
        ( cur_dir == 'w' && goal == 'n')
    ){turn_r(); move_f();}
    return goal;
}

bool is_goal(int x, int y){
    return (x==n/2||x==n/2-1)&&(y==n/2||y==n/2-1);
}

bool in_range(int x, int y){
    return x>=0 && x<n && y>=0 && y<n;
}

char to_xy( char dir_s, int x_s, int y_s, int x_g, int y_g){
    if( x_s == x_g && y_s==y_g) return dir_s;
    
    vector<tuple<int, int>> cur_path;
    int tmp_x = x_s;
    int tmp_y = y_s;
    while( tmp_x!=-1 && tmp_y!=-1){
        tuple<int,int> tmp = from[tmp_x][tmp_y];
        tmp_x = get<0>(tmp);
        tmp_y = get<1>(tmp);
        cur_path.insert(cur_path.begin(), make_tuple(tmp_x, tmp_y) );
    }

    vector<tuple<int, int>> goal_path;
    tmp_x = x_g;
    tmp_y = y_g;
    while( tmp_x!=-1 && tmp_y!=-1){
        tuple<int,int> tmp = from[tmp_x][tmp_y];
        tmp_x = get<0>(tmp);
        tmp_y = get<1>(tmp);
        goal_path.insert(goal_path.begin(), make_tuple(tmp_x, tmp_y) );
    }


    int i = 0;
    while (i<min( goal_path.size(), cur_path.size()) && goal_path[i]==cur_path[i]) i++;
    i--;

    // std::cout << "---------------------------------\n";
    goal_path = {goal_path.begin()+i+1, goal_path.end() };
    cur_path = {cur_path.begin()+i, cur_path.end() };
    cur_path.insert(cur_path.end(), goal_path.rbegin(), goal_path.rend());

    // following cur_path which leads to x_g
    char di = dir_s;
    for (auto next_grid:cur_path){
        int goal_x = get<0>(next_grid);
        int goal_y = get<1>(next_grid);
        // std::cout << "( " << goal_x << ", " << goal_y << " )\n";
        int x_d = goal_x - x_s;
        int y_d = goal_y - y_s;
        
        if (x_d==0 && y_d==0){
            continue;
        }else if(x_d==1 && y_d==0){
            di = move1grid(di, 'n');
        }else if(x_d==-1 && y_d==0){
            di = move1grid(di, 's');
        }else if(x_d==0 && y_d==1){
            di = move1grid(di, 'e');
        }else if(x_d==0 && y_d==-1){
            di = move1grid(di, 'w');
        }
    }
    // std::cout << "+++++++++++++++++++++++++++++++\n";

    return di;
}

float h(int x, int y){
    int dx = min( std::abs(n/2 - x), std::abs(n/2-1 - x));
    int dy = min( std::abs(n/2 - y), std::abs(n/2-1 - y));
    // return sqrt( pow( min(dx, dy), 2)*2 ) + std::abs(dx-dy);
    return dx+ dy;
}

vector<tuple<int,int>> A_star(int x_s, int y_s, char dir){
    int x,y;
    priority_queue< tuple<int, int>,  vector<tuple<int, int>>,  gcmp > pq;
    for(int i = 0; i<n; i++)for(int j=0; j<n; j++){ 
        f[i][j] = 2147483646; 
        g_[i][j] = 2147483646;
        from[i][j] = make_tuple(-1,-1);
    }

    g_[x_s][y_s] = 0;
    f[x_s][y_s] = h(x_s, y_s);
    pq.push( make_tuple(x_s, y_s ) );
    inQ[x_s][y_s] = true;

    x = x_s;
    y = y_s;
    while (!pq.empty()){
        auto cur = pq.top();
        int x_top = get<0>(cur); 
        int y_top = get<1>(cur);
        pq.pop();
        inQ[x_top][x_top] = false;

        // go from (x,y) to (x_top,y_top) 
        // std::cout <<"going from " << x << ", " <<  y << " to "<< x_top << ", " << y_top <<"\n" ;
        dir = to_xy(dir, x, y, x_top, y_top);
        x = x_top;
        y = y_top;
        
        // if goal
        if( is_goal(x, y)) {
            // std::cout << "is goal\n";
            break;
        }
        // for each neighbor:        
        int t_gs = g_[x][y]+1;
        // front
        int neighbor_x = x;
        int neighbor_y = y;

        auto walls = check_wall();

        if( walls[1] ){
            // std::cout << "front open\n" ;
                 if(dir == 'n')neighbor_y = y+1;
            else if(dir == 's')neighbor_y = y-1;
            else if(dir == 'e')neighbor_x = x+1;
            else if(dir == 'w')neighbor_x = x-1;
            
            if(in_range(neighbor_x,neighbor_y) && t_gs < g_[neighbor_x][neighbor_y] ){
                g_[neighbor_x][neighbor_y] = t_gs;
                f[neighbor_x][neighbor_y] = t_gs + h(neighbor_x, neighbor_y);
                //if not in pq add to it
                if( !inQ[neighbor_x][neighbor_y] ){
                    tuple<int, int> nei(neighbor_x, neighbor_y);
                    from[neighbor_x][neighbor_y] = make_tuple(x,y);
                    inQ[neighbor_x][neighbor_y] = true;
                    pq.push(nei);
                }
            }
        }

        // left 
        neighbor_x = x;
        neighbor_y = y;
        if( walls[0] ){
            // std::cout << "left open\n" ;
                 if(dir == 'n')neighbor_x = x-1;
            else if(dir == 's')neighbor_x = x+1;
            else if(dir == 'e')neighbor_y = y+1;
            else if(dir == 'w')neighbor_y = y-1;

            if(in_range(neighbor_x,neighbor_y) && t_gs < g_[neighbor_x][neighbor_y] ){
                g_[neighbor_x][neighbor_y] = t_gs;
                f[neighbor_x][neighbor_y] = t_gs + h(neighbor_x, neighbor_y);
                //if not in pq add to it
                if( !inQ[neighbor_x][neighbor_y] ){
                    tuple<int, int> nei(neighbor_x, neighbor_y);
                    from[neighbor_x][neighbor_y] = make_tuple(x,y);
                    inQ[neighbor_x][neighbor_y] = true;
                    pq.push(nei);
                }
            }
        }

        // right
        neighbor_x = x;
        neighbor_y = y;
        if( walls[2] ){
            // std::cout << "right open\n" ;
                 if(dir == 'n')neighbor_x = x+1;
            else if(dir == 's')neighbor_x = x-1;
            else if(dir == 'e')neighbor_y = y-1;
            else if(dir == 'w')neighbor_y = y+1;
            if(in_range(neighbor_x,neighbor_y) && t_gs < g_[neighbor_x][neighbor_y] ){
                g_[neighbor_x][neighbor_y] = t_gs;
                f[neighbor_x][neighbor_y] = t_gs + h(neighbor_x, neighbor_y);
                
                //if not in pq add to it
                if( !inQ[neighbor_x][neighbor_y] ){
                    tuple<int, int> nei(neighbor_x, neighbor_y);
                    from[neighbor_x][neighbor_y] = make_tuple(x,y);
                    inQ[neighbor_x][neighbor_y] = true;
                    pq.push(nei);
                }
            }
            // std::cout << "end of loop\n" ;
        }
    }
    vector<tuple<int, int>> final_path;
    
    int tmp_x = x;
    int tmp_y = y;
    while( tmp_x!=-1 && tmp_y!=-1){
        tuple<int,int> tmp = from[tmp_x][tmp_y];
        tmp_x = get<0>(tmp);
        tmp_y = get<1>(tmp);
        final_path.insert(final_path.begin(), make_tuple(tmp_x, tmp_y) );
    }
    
    return final_path;
}