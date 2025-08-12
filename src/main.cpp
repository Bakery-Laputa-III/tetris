#include "../include/tetris.hpp"

using namespace std;

int main(int argc, char ** argv) {

    // curses窗口设置
    setlocale(LC_CTYPE, "");
    initscr();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    curs_set(0);
	start_color();
    use_default_colors();

    // 设置和修改用户配置
    int startLevel = 8;
    bool easy = false;

    for ( int i = 1; i < argc; i++ ) {
        if ( argv[i] == string("--start-level") )
            startLevel = stoi(argv[i + 1]);
        else if ( argv[i] == string("--easy"))
            easy = true;
    }

    // 初始化屏幕和形状
    Screen screen(startLevel);
    Shape shape;
    
    // 记录用户如何调用程序
    string basename = argv[0];
    
    // 调用主循环
    game(shape, screen, startLevel, easy, basename);

    // curses清理
    endwin();
    return 0;
}
