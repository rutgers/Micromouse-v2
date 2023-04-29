

struct configuration {
    int x;
    int y;
    char dir;
};

struct openCells {
    bool openN; 
    bool openS;
    bool openE;
    bool openW;
};

class Flood {

public:
    // static configuration flowElevation(configuration* currentCfg, int maze[16][16]);
    // static openCells checkOpenCells(configuration poppedCfg);
    // static void checkNeigboringOpen(configuration poppedCfg, int maze[16][16], stack<configuration> stack);
    static void move(configuration* currentCfg, char direction);
};
