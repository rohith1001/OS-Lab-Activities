#include <cmath>
#include <fstream>
#include <iostream>
#include <limits.h>
#include <string>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#include <vector>

ofstream mylog;
char cwd[PATH_MAX];
mylog.open("log.txt");
vector<string> fileNames;
struct winsize myTermSize;
struct termios myTerm;
int maxRows, maxCols;
int x = 1, y = 1;
int index = 0;