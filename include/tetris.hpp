// 俄罗斯方块头文件

// 包含大量库文件:
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <ncurses.h>
#include <fstream>
#include <unistd.h>
#include <cstdlib>
#include <stdio.h>
#include <random>
#include <cstring>
#include <algorithm>
#include <unistd.h>
#include <errno.h>
#include <cstring>

using namespace std;

// 函数声明
string readLine(string str, int n);
bool isNumber(const string& s);
void checkNext(int startLevel, bool easy, string basename);
int randNum(int start, int end);

// 定义形状和屏幕类
class Shape {
    public: //! 可能需要更好地使用封装
        Shape(); // 空构造函数

        // 形状属性
        int defaultPos[2]; // 形状的默认位置
        int trCoord[2]; // 形状当前右上角坐标
        int shapeHeight = 0;

        vector<vector<bool> > selected; // 当前选中的形状
        vector<vector<bool> > nextUp; // 下一个形状
        
        vector<int> chosenchars; // 选中的打印键集合
        vector<int> nextchars;   // 下一组打印键

        vector<vector<vector<bool> > > shapecoords = { // 形状向量
            {   // the 'o' block
                {0,0,0,0},
                {0,1,1,0},
                {0,1,1,0},
                {0,0,0,0}
            },
            {   // the long block
                {0,0,0,0},
                {1,1,1,1},
                {0,0,0,0},
                {0,0,0,0}
            },
            {   // the l block
                {0,1,1,0},
                {0,1,0,0},
                {0,1,0,0},
                {0,0,0,0}
            },
            {   // the j block
                {0,1,1,0},
                {0,0,1,0},
                {0,0,1,0},
                {0,0,0,0}
            },
            {   // the zag block
                {0,0,1,0},
                {0,1,1,0},
                {0,1,0,0},
                {0,0,0,0}
            },
            {   // the zig block
                {0,1,0,0},
                {0,1,1,0},
                {0,0,1,0},
                {0,0,0,0}
            },
            {   // the t block
                {0,0,0,0,0},
                {0,0,1,0,0},
                {0,1,1,1,0},
                {0,0,0,0,0},
                {0,0,0,0,0},
            }
        };

        vector<vector<int> > chars = { // 每个形状的打印键
            {1, 2},
            {3, 4},
            {5, 6},
            {7, 8},
            {9, 10},
            {11, 12},
            {13, 14}
        };

        vector<vector<string> > currentWin;
        
        // vector<int> colors = { COLOR_WHITE, COLOR_WHITE, COLOR_BLUE, COLOR_RED, COLOR_RED, COLOR_BLUE, COLOR_WHITE };
        vector<int> colors = { COLOR_YELLOW, COLOR_CYAN, COLOR_BLUE, COLOR_WHITE, COLOR_RED, COLOR_GREEN, COLOR_MAGENTA };
        int color; // 数组中选中的颜色

        int shapetype[2];

        // 游戏状态
        bool gameover = false;
        bool cannotMove = false;
        bool dead = false;
        int shapeRotation = 1;
        int isdropping;

        // 创建新形状
        void generate(vector<vector<string> > window);
        
        // 改变形状位置的方法
        void draw();
        void drop();
        void fall();
        void rotate();
        void move(int movetype);
        void ground(int framerate);
        vector<int> charCoords(vector<vector<bool> > shape);

        // 幽灵方块方法
        void showGround();
        void groundDraw(int down);
        vector<int> groundCoords(vector<vector<bool> > shape, int down);
        
        // 更新游戏状态
        void checkDeath();
};



class Screen {
    // game states
    // bool gameover = false;
    int defaultPos[2] = {1,1};

    // game stats
    int score = 0;
    int lines = 0;
    int level = 0;

    public:

        // game states
        int startLevel = 1;
        bool advancingLevel = false;

        // display variables
        // we might not want to hardcode the border
        const string screenstr = "  ┏━━k-vernooy/tetris━━┓\n  ┃                    ┃\n  ┃                    ┃   ┏━━next━━━┓\n  ┃                    ┃   ┃         ┃\n  ┃                    ┃   ┃         ┃\n  ┃                    ┃   ┃         ┃\n  ┃                    ┃   ┗━━━━━━━━━┛\n  ┃                    ┃\n  ┃                    ┃   ┏━━score━━┓\n  ┃                    ┃   ┃         ┃\n  ┃                    ┃   ┃  0      ┃\n  ┃                    ┃   ┃         ┃\n  ┃                    ┃   ┗━━━━━━━━━┛\n  ┃                    ┃\n  ┃                    ┃   ┏━━lines━━┓\n  ┃                    ┃   ┃         ┃\n  ┃                    ┃   ┃  0      ┃\n  ┃                    ┃   ┃         ┃\n  ┃                    ┃   ┗━━━━━━━━━┛\n  ┗━━━━━━━━━━━━━━━━━━━━┛\n                 ";
        vector<vector<string> > window; 
        vector<int> colors = { COLOR_YELLOW, COLOR_CYAN, COLOR_BLUE, COLOR_WHITE, COLOR_RED, COLOR_GREEN, COLOR_MAGENTA };

        Screen(int startlevel); // 构造函数，生成数组并设置起始关卡

        // graphics methods for updating the screen
        void draw();
        void top();
        void updateIntDisplays(int score, int x, int y);
        string getChar(int x, int y); // 返回屏幕坐标处的字符串

        void addStartLevel(int startlevel); // update level variables
        void advanceLevel(); // level up the game

        // void gameOver();

        void addShape(Shape shape);
        vector<int> pointCheck();
        void shiftLines(vector<int> lines);
        bool points();
        void addNext(vector<vector<bool> > shape, vector<int> color);
};

void game(Shape shape, Screen screen, int startLevel, bool easy, string basename);