#include "../include/tetris.hpp"

Shape::Shape() {
    // 构造函数生成随机形状

    random_device rd; 
    mt19937 eng(rd()); 
    uniform_int_distribution<> distr(1, 7);

    int rand = distr(eng) - 1;
    shapetype[0] = 0;
    shapetype[1] = rand;
}


void Shape::generate(vector<vector<string> > window) {
    // 将下一个随机状态转移到当前状态
// 并生成新的随机状态
    
    // 修复死亡状态
    dead = false;
    // move old data to new spot
    shapetype[0] = shapetype[1];
    currentWin = window;

    int rand = randNum(1, 7);

    while ( rand == shapetype[0] )
        // 避免重复形状
        rand = randNum(1, 7);

    shapetype[1] = rand;
    nextUp = shapecoords[shapetype[1]];
    nextchars = chars[shapetype[1]];

    selected = shapecoords[shapetype[0]];
    chosenchars = chars[shapetype[0]];
    color = colors[shapetype[0]];       

    // find the height of the shape
    shapeHeight = 0;

    for ( int i = 0; i < 4; i++ ) {
        vector<bool> line = selected[i];
        bool found = false;

        for ( int x = 0; x < line.size(); x++ )
            if ( i == 1 )
                found = true;

        if ( found )
            shapeHeight++;
    }

    defaultPos[0] = 0;
    defaultPos[1] = 2;
    trCoord[0] = 0;
    trCoord[1] = 9;

    // can prob add something here to center shape; ie shapeWidth;
    trCoord[0] -= shapeHeight;
    trCoord[0] -= 2;
    isdropping = shapeHeight;
}

string Screen::getChar(int x, int y) {
    // get character at coordinates
    x += defaultPos[0];
    y += defaultPos[1];

    return window[x][y];
}

vector<int> Screen::pointCheck() {
    // checks for full lines on board, returns
    // an array of indexes of full lines

    vector<int> fullLines;

    for ( int i = 1; i < 19; i++ ) {
        // loop through lines of board
        bool full = true;

        // add full lines to array
        for ( int k = 5; k < 25; k++ ) {
            const char * chosen = window[i][k].c_str();
            for ( int i = 0; i < strlen(chosen); i++ ) {
                if ( !isdigit(chosen[i]) )
                    full = false;
                if ( !full )
                    break;
            }
            if ( !full )
                break;
        }
        if ( full )
            fullLines.push_back(i);
    }

    return fullLines;
}

void Screen::shiftLines(vector<int> lines) {
    // moves lines above full lines down

    for ( int i = 0; i < lines.size(); i++ )
        // empty full lines
        for ( int x = 5; x < 25; x++)
            window[lines[i]][x] = " ";
    
    usleep(45000);
    move(0,0);
    refresh();

    // print fullblocks for flashing effect
    if ( lines.size() > 0  ) {
        for ( int i = 0; i < lines.size(); i++ )
            for ( int x = 3; x < 23; x++)
                mvprintw(lines[i],x,"█");

        move(0,0);
        refresh();
        usleep(45000);
        draw();
        move(0,0);
        refresh();
        usleep(25000);

        for ( int i = 0; i < lines.size(); i++ )
            for ( int x = 3; x < 23; x++)
                mvprintw(lines[i],x,"█");

        move(0,0);
        refresh();
        usleep(45000);
    }

    // actually move lines down
    for ( int i = 0; i < lines.size(); i++ ) {
        vector<string> line = window[lines[i]];
        for ( int j = lines[i] - 1; j > 0; j-- )
            for ( int x = 5; x < 25; x++ )
                window[j + 1][x] = window[j][x];
    }
}

vector<int> Shape::charCoords(vector<vector<bool> > shape) {
    vector<int> coords;
    int currentPos[2] = { trCoord[0] + defaultPos[1] + 1, trCoord[1] + defaultPos[0]};
    // top  coord is pos[2]
    for ( int i = 0; i < shape.size(); i++ ) {
        for ( int j = 0; j < shape[i].size(); j++ ) {
            if ( shape[i][j] ) {
                int thisPos[2] = { currentPos[0] + i, currentPos[1] + ( 2 * j ) };
                coords.push_back(thisPos[0]);
                coords.push_back(thisPos[1]);
                // add this coord to the array
            }
        }
    }
    return coords;
}

void Shape::drop() {
    bool cannotDrop = false;
    vector<int> coords = charCoords(selected);

    for ( int i = 0; i < coords.size(); i = i + 2) {
        string check = currentWin[coords[i]][coords[i + 1]];
        if ( check != " " && isNumber(check) )
            cannotDrop = true;
    }

    if ( cannotDrop )
        gameover = true;
    else {
        trCoord[0]++;
        isdropping--;
    }

}


void Shape::rotate() { 
    // 如果可能，将'selected'形状数组逆时针旋转

    vector<vector<bool> > temp;

    for ( int i = 0; i < selected.size(); i++ ) {
        vector<bool> row;
        for ( int j = 0; j < selected.size(); j++)
            row.push_back(0);
        temp.push_back(row);
    }

    int count = 0; // count variable
    int count2 = 0; // 2nd count variable

    for (count = 0; count < selected.size(); count++) {
        for (count2 = 0; count2 < selected[0].size(); count2++) {
            // 逆时针旋转
            temp[temp.size() - count2 - 1][count] = selected[count][count2];
            
            // 顺时针旋转
            // temp[count2][temp[0].size() - count - 1] = selected[count][count2];
        }
    }

    vector<int> coords = charCoords(temp);
    bool rotate = true;
    for ( int i = 0; i < coords.size(); i += 2 ) {
        string chosen = currentWin[coords[i] - 1][coords[i + 1] + 2];
        if ( chosen != " " )
            rotate = false;
    }

    if ( rotate )
        selected = temp;

    // 重置形状高度变量
    shapeHeight = 0;
    for ( int row = 0; row < selected.size(); row++ ) {
        bool found = false;
        for ( int cell = 0; cell < selected[row].size(); cell++ ) {
            if (selected[row][cell] == 1) {
                found = true;
            }
        }
        if ( found )
            shapeHeight++;
    }
}

void Shape::draw( ) {
    int currentPos[2] = { trCoord[0] + defaultPos[1], trCoord[1] + defaultPos[0]};

    init_pair(2, color, -1);
    attrset(COLOR_PAIR(2));
    for ( int i = 0; i < selected.size(); i++  ) {
        // for each line;
        vector<bool> line = selected[i];
        for ( int i = 0; i < 4; i++ ) {
            // for each el in line;
            if ( line[i] ) {
                // need to draw two side by side fullblocks;
                mvprintw(currentPos[0], currentPos[1], string("██").c_str());
            }
            else {
                mvprintw(currentPos[0], currentPos[1], string("").c_str());
            }
            currentPos[1] += 2;
        }
        currentPos[0] += 1;
        currentPos[1] = trCoord[1] + defaultPos[0];
    }
    init_pair(1, COLOR_WHITE, -1);
    attrset(COLOR_PAIR(1));
}

void Shape::checkDeath() {
    bool cannotFall = false;
    vector<int> coords = charCoords(selected);
    for ( int i = 0; i < coords.size(); i = i + 2) {
        string check = currentWin[coords[i]][coords[i + 1] + 3];
        if ( check != " ") {
            cannotFall = true;
        };
    }

    dead = cannotFall;
}

void Shape::fall() {
    trCoord[0]++;
}

void Shape::move(int movetype) {
    int currentPos[2] = { trCoord[0] + defaultPos[1], trCoord[1] + defaultPos[0]};

    vector<int> coords = charCoords(selected);
    
    bool move = true;

    if ( movetype == 1 ) {
        // move left
        for ( int i = 0; i < coords.size(); i += 2 ) {
            // check the box left of coords[i], coords[i + 1]
            if ( currentWin[coords[i] - 1][coords[i + 1]] != " " ) {
                move = false;
            }
        }

        if ( move ) {
            trCoord[1] -= 2;
        }
        else {
            //cannot move, error noise
            // beep();
        }
    }
    else if ( movetype == 2 ) {
        // move rigjt
        for ( int i = 0; i < coords.size(); i += 2 ) {
            if ( currentWin[coords[i] - 1][coords[i + 1] + 4] != " " ) {
                move = false;
            }
        }

        if ( move ) {
            trCoord[1] += 2;
        }
        else {
            //cannot move, error noise
            // beep();
        }
    }
    else if ( movetype == 3) {
        // move down
        for ( int i = 0; i < coords.size(); i += 2 ) {
            int y = coords[i + 1];
            if ( y >= 18 || currentWin[coords[i] + 1][y + 2] != " " ) {
                move = false;
                break;
            }
        }

        if ( move ) {
            trCoord[0] += 1;
        }
    }
}

void Shape::ground(int framerate) {
    bool moveDown = true;
    while (moveDown) {
        vector<int> coords = charCoords(selected);
        for ( int i = 0; i < coords.size(); i += 2) {
            if ( currentWin[coords[i] + 1][coords[i + 1] + 2] != " " ) {
                moveDown = false;
            }
        }
        
        if (moveDown) {
            trCoord[0] += 1;
        }
    }
}

vector<int> Shape::groundCoords(vector<vector<bool> > shape, int down) {
    vector<int> coords;
    int currentPos[2] = { trCoord[0] + defaultPos[1] + down, trCoord[1] + defaultPos[0]};
    // top  coord is pos[2]
    for ( int i = 0; i < shape.size(); i++ ) {
        for ( int j = 0; j < shape[i].size(); j++ ) {
            if ( shape[i][j] ) {
                
                int thisPos[2] = { currentPos[0] + i, currentPos[1] + ( 2 * j ) };
                coords.push_back(thisPos[0]);
                coords.push_back(thisPos[1]);
                // add this coord to the array
            }
        }
    }
    return coords;
}

void Shape::groundDraw(int down)  {
    mvprintw(0,0,"5");
    int currentPos[2] = { trCoord[0] + defaultPos[1] + down, trCoord[1] + defaultPos[0]};

    init_pair(2, color, -1);
    attrset(COLOR_PAIR(2));
    for ( int i = 0; i < selected.size(); i++  ) {
        // for each line;
        vector<bool> line = selected[i];
        for ( int i = 0; i < 4; i++ ) {
            // for each el in line;
            if ( line[i] ) {
                // need to draw two side by side fullblocks;
                mvprintw(currentPos[0], currentPos[1], string("░░").c_str());
            }
            else {
                mvprintw(currentPos[0], currentPos[1], string("").c_str());
            }
            currentPos[1] += 2;
        }
        currentPos[0] += 1;
        currentPos[1] = trCoord[1] + defaultPos[0];
    }
    init_pair(1, COLOR_WHITE, -1);
    attrset(COLOR_PAIR(1));

}

void Shape::showGround() {
    bool moveDown = true;
    int down = 0;
    while (moveDown) {
        vector<int> coords = groundCoords(selected, down);
        for ( int i = 0; i < coords.size(); i += 2) {
            if ( currentWin[coords[i] + 1][coords[i + 1] + 2] != " " ) {
                moveDown = false;
            }
        }
        
        if (moveDown) {
            down += 1;
        }
    }

    groundDraw(down);
}