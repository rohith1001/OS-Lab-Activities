#include <algorithm>
#include <cmath>
#include <cstring>
#include <dirent.h>
#include <fstream>
#include <grp.h>
#include <iostream>
#include <limits.h>
#include <pwd.h>
#include <string>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>
#include <vector>
#define cursorUpward cout << "\033[A"
#define cursorDownward cout << "\033[B"
using namespace std;
using namespace std;

ofstream mylog;
char cwd[PATH_MAX];

vector<string> fileNames;
struct winsize myTermSize;
struct termios myTerm;
int maxRows, maxCols;
vector<string> fileNamesVector;

void getCursor(int *x, int *y) {
    cout << "\033[6n";
    scanf("\033[%d;%dR", x, y);
}

void setToNonCanonical(struct termios &myTerm) {
    myTerm.c_lflag &= ~(ICANON | ECHO);
    myTerm.c_cc[VMIN] = 1;  // minimum characters for non-canonical read
    myTerm.c_cc[VTIME] = 0; // timeout in deciseconds for non-canonical read
    tcsetattr(0, TCSANOW, &myTerm);
}

void setToCanonical(struct termios &myTerm) {
    myTerm.c_lflag |= (ICANON | ECHO); // enable canonical mode and echo input characters
    tcsetattr(0, TCSANOW, &myTerm);
}

void moveCursor(int x, int y) { cout << "\033[" << x << ";" << y << "H"; }

int calculateNumOfSteps(string fileName, int max_cols) {
    int ans = (int)ceil(fileName.size() / max_cols);
    return ans;
}

void calculateTermSize(struct winsize &myTermSize, int &maxRows, int &maxCols) {
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &myTermSize);
    maxRows = myTermSize.ws_row;
    maxCols = myTermSize.ws_col;
}

void getTermAttr(struct termios &myTerm) { tcgetattr(0, &myTerm); }

void clearScreen() { cout << "\033[2J"; }

void clearAndBeginScr() { cout << "\033[2J\033[1;1H"; }

void moveCursorUpNonCanonical(int numOfSteps) { cout << "\033[" << numOfSteps << "A"; }

void moveCursorDownNonCanonical(int numOfSteps) { cout << "\033[" << numOfSteps << "B"; }

string getPermissionsInfo(struct stat &fileInfo) {
    string permissions = "";
    mode_t perm = fileInfo.st_mode;
    permissions += (perm & S_IRUSR) ? 'r' : '-';
    permissions += (perm & S_IWUSR) ? 'w' : '-';
    permissions += (perm & S_IXUSR) ? 'x' : '-';
    permissions += (perm & S_IRGRP) ? 'r' : '-';
    permissions += (perm & S_IWGRP) ? 'w' : '-';
    permissions += (perm & S_IXGRP) ? 'x' : '-';
    permissions += (perm & S_IROTH) ? 'r' : '-';
    permissions += (perm & S_IWOTH) ? 'w' : '-';
    permissions += (perm & S_IXOTH) ? 'x' : '-';
    return permissions;
}

void displayAllFilesInDir() {
    mylog.open("log.txt");
    DIR *d;
    struct dirent *dir;
    d = opendir(cwd);
    if (d) {
        clearAndBeginScr();
        while ((dir = readdir(d)) != NULL) {
            fileNamesVector.push_back(dir->d_name);
        }
        sort(fileNamesVector.begin(), fileNamesVector.end());
        for (auto f : fileNamesVector) {
            struct stat fileInfo;
            char fileName[f.size() + 1];
            strcpy(fileName, f.c_str());
            if (stat(fileName, &fileInfo) != 0) {
                cout << "Error accessing file stat...Exiting" << endl;
                return;
            }
            cout << f << "\t";
            if (f != "." && f != "..") {
                cout << fileInfo.st_size << "\t";
                cout << getPermissionsInfo(fileInfo) << "\t";
                cout << ctime(&fileInfo.st_mtime);
            } else {
                cout << "\n";
            }
        }
        setToNonCanonical(myTerm);
    }
}

void getCurrentDirectory() {
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        cout << "Error in getting current directory path...Exiting" << endl;
    }
}

int main() {
    getCurrentDirectory();
    mylog.open("log.txt");
    mylog << "CWD got is: " << endl;
    // code to collect and display all files in the current directory
    displayAllFilesInDir();
    getTermAttr(myTerm);
    int x = 1, y = 1;
    int index = 0;
    moveCursor(1, 1);
    // for (auto s : fileNames) {
    //     cout << s << "\n";
    // }
    // moveCursor(1, 1);
    setToNonCanonical(myTerm);
    // take input only after making terminal non-canonical
    index = 0;
    while (true) {
        getCursor(&x, &y);
        // calculateTermSize(myTermSize, maxRows, maxCols);
        char ch;
        ch = cin.get();
        mylog << "ch is: " << ch << "\n";
        mylog << "x is: " << x << "\n";
        mylog << "index is: " << index << "\n";
        if (ch == 27) {
            char d, e;
            d = cin.get();
            e = cin.get();
            if (d == 91) {
                if (e == 65) {
                    // up case
                    mylog << "Up case\n";
                } else if (e == 66) {
                    // down case
                    mylog << "Down case\n";
                }
            }
            break;
        } else if (ch == 'k') {
            if (x <= 1) {
                moveCursor(maxRows, 1);
                setToCanonical(myTerm);
                cout << "You pressed k at first row...Refreshing logic!!!"
                     << "\n";
                mylog.close();
                return 1;
            }
            // eraseArrowBelow(x, numOfSteps, maxRows, myTerm);
            // eraseArrow(x, myTerm);
            index--;
            // int numOfSteps = calculateNumOfSteps(fileNames[index], maxCols);
            moveCursorUpNonCanonical(1);
            x -= 1;
            // displayArrow(x, myTerm);
        } else if (ch == 'l') {
            // eraseArrow(x, myTerm);
            // eraseArrowAbove(x, myTerm);
            // int numOfSteps = calculateNumOfSteps(fileNames[index], maxCols);
            if ((x) >= fileNames.size()) {
                moveCursor(1, 1);
                setToNonCanonical(myTerm);
                x = 1;
                index = 0;
                continue;
            }
            if ((x + 1) >= maxRows) {
                moveCursor(maxRows, 1);
                setToCanonical(myTerm);
                cout << "You pressed l at last row...Refreshing logic!!!"
                     << "\n";
                mylog.close();
                return 1;
            }
            moveCursorDownNonCanonical(1);
            index++;
            x += 1;
            // displayArrow(x, myTerm);
        } else {
            break;
        }
    }
    setToCanonical(myTerm);
    cout << "after break x: " << x << "y: " << y;
    mylog.close();
    return 0;
}