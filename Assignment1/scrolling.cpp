#include <cmath>
#include <iostream>
#include <string>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#include <vector>
using namespace std;

void getCursor(int *x, int *y) {
    cout << "\033[6n";
    // cout << "\e[6n";
    // better to use scanf here
    scanf("\033[%d;%dR", x, y);
}

void displayArrow(int x, struct termios &myTerm) {
    if (x != 1) {
        cout << "\033[" << x << ";" << 1 << "H";
        myTerm.c_lflag |= (ICANON | ECHO);
        tcsetattr(0, TCSANOW, &myTerm);
        cout << ">>";
        myTerm.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(0, TCSANOW, &myTerm);
    }
}

void eraseArrowAbove(int x, struct termios &myTerm) {
    if (x != 1) {
        cout << "\033[" << x - 1 << ";" << 1 << "H";
        myTerm.c_lflag |= (ICANON | ECHO);
        tcsetattr(0, TCSANOW, &myTerm);
        cout << "  ";
        myTerm.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(0, TCSANOW, &myTerm);
    }
}

void eraseArrowBelow(int x, int max_rows, struct termios &myTerm) {
    if (x != max_rows) {
        cout << "\033[" << x + 1 << ";" << 1 << "H";
        myTerm.c_lflag |= (ICANON | ECHO);
        tcsetattr(0, TCSANOW, &myTerm);
        cout << "  ";
        myTerm.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(0, TCSANOW, &myTerm);
    }
}

int calculateNumOfSteps(string fileName, int max_cols) {
    int ans = (int)ceil(fileName.size() / max_cols);
    return ans;
}

int main() {
    vector<string> fileNames;
    for (int i = 10; i < 100; i++) {
        string res = "   llllllllllllllllllllllllllo0";
        res += to_string(i);
        fileNames.push_back(res);
    }
    struct winsize myTermSize;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &myTermSize);
    int max_rows = myTermSize.ws_row;
    int max_cols = myTermSize.ws_col;
    struct termios myTerm;
    tcgetattr(0, &myTerm);
    myTerm.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(0, TCSANOW, &myTerm);
    int x = 1, y = 1;
    int index = 0;
    for (auto s : fileNames) {
        cout << s << endl;
    }
    // Place the cursor at top most position
    cout << "\033[1;1H";
    index = 0;
    while (true) {
        getCursor(&x, &y);
        char ch;
        ch = cin.get();
        if (ch == 'k') {
            cout << "\033[A";
            if (x == 1) {
                cout << "\033[" << max_rows << ";" << 1 << "H";
                cout << "Refreshing logic!!!" << endl;
                return 1;
            }
            getCursor(&x, &y);
            eraseArrowAbove(x, myTerm);
            eraseArrowBelow(x, max_rows, myTerm);
            index--;
            int numOfSteps = calculateNumOfSteps(fileNames[index], max_cols);
            cout << "\033" << numOfSteps << "[A";
            displayArrow(x, myTerm);
            // cout << "\033[" << x << ";" << 1 << "H";
            // myTerm.c_lflag |= (ICANON | ECHO);
            // cout << ">>";
            // myTerm.c_lflag &= ~(ICANON | ECHO);
        } else if (ch == 'l') {
            if (x == max_rows) {
                cout << "\033[" << max_rows << ";" << 1 << "H";
                cout << "Refreshing logic!!!" << endl;
                return 1;
            }
            getCursor(&x, &y);
            eraseArrowAbove(x, myTerm);
            eraseArrowBelow(x, max_rows, myTerm);
            displayArrow(x, myTerm);
            int numOfSteps = calculateNumOfSteps(fileNames[index], max_cols);
            cout << "\033" << numOfSteps << "[B";
            displayArrow(x, myTerm);
            index++;
            // cout << "\033[" << x << ";" << 1 << "H";
            // myTerm.c_lflag |= (ICANON | ECHO);
            // cout << ">>";
            // myTerm.c_lflag &= ~(ICANON | ECHO);
        } else {
            getCursor(&x, &y);
            cout << "\033[" << x << ";" << y << "H";
            break;
        }
    }
    myTerm.c_lflag |= (ICANON | ECHO);
    tcsetattr(0, TCSANOW, &myTerm);
    cout << "after break x: " << x << "y: " << y;

    return 0;
}
