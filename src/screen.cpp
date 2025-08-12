#include "../include/tetris.hpp"

Screen::Screen(int startLevel) {
    // 将屏幕字符串转换为屏幕数组
    startLevel = startLevel;
    level = startLevel;

    string line;
    stringstream f(screenstr);
    int lineNum = 0;

    while (getline(f, line)) {
        // get number of lines in string
        lineNum++;
    }

    for ( int x = 0; x < lineNum; x++ ) {
        vector<string> rowVec;
        string line = readLine(screenstr, x);
        for ( int i = 0; i < line.length(); i++ ) {
            rowVec.push_back(line.substr(i,1));
        }
        window.push_back(rowVec);
    }
}

void Screen::top() {
    // 打印屏幕首行以覆盖形状
    move(0,0);
    printw(readLine(screenstr, 0).c_str());
}

void Screen::draw() {
    // 遍历窗口并打印
    for ( int i = 0; i < window.size(); i++ ) {
        for ( int j = 0; j < window[i].size(); j++) {

            string cur = window[i][j]; // the current character

            if ( (( i >= 1 && i < 19 ) && ( j > 4 && j < 25)) || ( ( i >= 3 && i < 6 ) && ( j > 33 && j < 43) )) {

                // 处于游戏窗口内
// 需要检查整数打印键

                if ( cur != " ") {
                    int colorNum;
                    string printChar;

                    if (stoi(cur) % 2 == 0) {
                        colorNum = stoi(cur) / 2 - 1;
                        printChar = "█";
                    }
                    else {
                        colorNum = floor(stoi(cur) / 2);
                        printChar = "█";
                    }

                    int color = colors[colorNum];

                    // set the appropriate color
                    init_pair(colorNum + 3, color, -1);
                    attrset(COLOR_PAIR(colorNum + 3));
                    // print the character
                    printw(printChar.c_str());
                    // reset to default white color
                    init_pair(1, COLOR_WHITE, -1);
                    attrset(COLOR_PAIR(1));
                }
                else {
                    // just print a space
                    printw(string(" ").c_str());
                }
            }
            else {
                printw(window[i][j].c_str());
            }
        }
        printw(string("\n").c_str());
    }
}

void Screen::addNext(vector<vector<bool> > shape, vector<int> colors) {
    // 将下一个形状添加到小窗口
    
    // default positions for the window
    int x = 34;
    int y = 3;

    for ( int i = 0; i < 3; i++ ) {
        // loop through window
        vector<bool> line = shape[i];
        for ( int j = 0; j < 4; j++ ) {
            if ( line[j] ) {
                // print appropriate characters with appropriate color
                window[y + i][x + ( 2 * j )] = to_string(colors[0]);
                window[y + i][x + ( 2 * j ) + 1] = to_string(colors[1]);
            }
            else {
                // print two spaces
                window[y + i][x + ( 2 * j )] = " ";
                window[y + i][x + ( 2 * j ) + 1] = " ";               
            }
        }
    }
}

void Screen::updateIntDisplays(int score, int x, int y) {
    // 将分数写入小窗口

    window[x][y] = to_string(score);

    // add or remove spaces for centering
    for ( int z = 37; z < 42; z++ )
        window[x][z] = " ";

    for ( int i = 1; i < to_string(score).length(); i++ )
        window[x][y + i] = "";
}

void Screen::addShape(Shape shape) {
    // 游戏结束时将形状对象添加到屏幕
    // to be stored in the screen object
    for ( int i = 0; i < shape.selected.size(); i++ ) {
        for ( int x = 0; x < shape.selected[i].size(); x++ ) {
            if ( shape.selected[i][x] ) {
                window[i + shape.trCoord[0] + shape.defaultPos[1]][(2 * x - 1) + shape.trCoord[1] + shape.defaultPos[0] + 3] = to_string(shape.chosenchars[0]);
                window[i + shape.trCoord[0] + shape.defaultPos[1]][(2 * x) + shape.trCoord[1] + shape.defaultPos[0] + 3] = to_string(shape.chosenchars[1]);
            }
        }
    }
}

bool Screen::points() {
    // 分数管理
// 返回是否进入下一关

    // add lines gained this round to line score
    vector<int> fullLines = pointCheck();    
    int linesNew = fullLines.size();
    lines += linesNew;

    // amounts of points gained at different number of clears 
    vector<int> pointsPerClear = { 40 * (level), 100 * (level), 300 * (level), 1200 * level};

    // add points gained this round to point score
    int point;
    if ( fullLines.size() == 0 )
        point = 0;
    else
        point = pointsPerClear[fullLines.size() - 1];
    score += point;

    // print the stats
    updateIntDisplays(score, 10, 36);
    updateIntDisplays(lines, 16, 36);

    // move the lines down
    shiftLines(fullLines);

    // logic for when to advance levels
    if ( level == startLevel ) {
        if ( lines > startLevel * 10 + 10) {
            window[0][0] = to_string(0);
            advanceLevel();
            return true;
        }
    }
    else if ( (lines - startLevel * 10 + 10) - ((level - startLevel) * 10) >= 10 ) {
        window[0][0] = to_string(0);
        advanceLevel();
        return true;
    }

   return false; // not advancing level
}

void Screen::advanceLevel() {
    // 游戏升级
    level++;
    advancingLevel = true; // in order to reduce framerate in mainloop

    // redefine colors
    colors = { COLOR_WHITE, COLOR_WHITE, COLOR_BLUE, COLOR_RED, COLOR_RED, COLOR_BLUE, COLOR_WHITE };
}
