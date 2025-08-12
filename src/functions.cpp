
#include "../include/tetris.hpp"

using namespace std;

void checkNext(int startLevel, bool easy, string basename) {
    // 读取用户(y/n)输入
// 根据输入重启游戏或退出

    int ch = getch();
    
    // 创建包含CLI选项的字符串
// 用于再次调用程序
    string systring = basename;
    systring += " --start-level ";
    systring += to_string(startLevel);
    if (easy)
        systring += " --easy";
    
    // 根据输入执行操作
    if ( ch == 'y' ) {
        // call binary
        if (system(systring.c_str()) == -1) {
            cerr << "Error calling '" << systring << "': "
                 << strerror(errno) << endl;
            exit(EXIT_FAILURE);
        }
    }
    else if ( ch == 'n' )
        return; // exit out of program
    else // did not input y or n, try again
        checkNext(startLevel, easy, basename);
}

string readLine(string str, int n) {
    // 返回字符串的第n行
    stringstream f(str);
    string s;
    
    for (int i = 0; i < n; i++) {
        getline(f, s);
    }

    getline(f,s);
    return s; 
}

bool isNumber(const string& str) {
    // 判断输入字符串是否为数字
    string::const_iterator it = str.begin();
    while (it != str.end() && isdigit(*it))
        ++it;

    // 如果迭代器到达字符串末尾
// 且字符串不为空，返回true
    return !str.empty() && it == str.end();
}

int randNum(int start, int end) {
    // 返回start和end之间的随机数
    random_device rd; 
    mt19937 eng(rd()); 
    uniform_int_distribution<> distr(start, end);
    return distr(eng) - 1;  
}