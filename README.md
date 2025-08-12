# 俄罗斯方块
我的ASCII/CLI版俄罗斯方块实现。

<img src=docs/out.gif width=500px>

## 安装

### 源码安装
Linux系统需要先安装依赖`libncursesw5-dev`。

```bash
git clone 'https://github.com/k-vernooy/tetris' && cd tetris
make
sudo make install
```
### Homebrew安装
```bash
brew tap k-vernooy/tap
brew install tetris
```

## 使用方法 

启动游戏只需在终端输入`tetris`。可通过`--start-level x`指定起始关卡(x为1-25的数字)。使用`--easy`参数可开启幽灵方块功能。

### 操作控制:
使用方向键控制下落方块:  
- <kbd>e</kbd> 开启/关闭幽灵方块
- <kbd>↑</kbd> 旋转下落方块
- <kbd>→</kbd> 向右移动
- <kbd>←</kbd> 向左移动
- <kbd>↓</kbd> 软降。当前软降功能存在已知bug，将在后续版本修复。如果程序出现段错误，很可能是这个原因。
- <kbd>空格</kbd> 硬降
- 随时按<kbd>CTRL</kbd>+<kbd>C</kbd>退出游戏
