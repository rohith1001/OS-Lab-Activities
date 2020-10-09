#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#include <vector>
using namespace std;

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

// void displayArrow(int x, struct termios &myTerm) {
//     moveCursor(x, 1);
//     setToCanonical(myTerm);
//     cout << ">>";
//     setToNonCanonical(myTerm);
// }

// void eraseArrowAbove(int x, struct termios &myTerm) {
//     if (x != 1) {
//         moveCursor(x - 1, 1);
//         setToCanonical(myTerm);
//         cout << "  ";
//         setToNonCanonical(myTerm);
//     }
// }

// void eraseArrow(int x, struct termios &myTerm) {
//     moveCursor(x, 1);
//     setToCanonical(myTerm);
//     cout << "  ";
//     setToNonCanonical(myTerm);
// }

// void eraseArrowBelow(int x, int numOfSteps, int max_rows, struct termios &myTerm) {
//     if ((x + numOfSteps) < max_rows) {
//         moveCursor(x + 1, 1);
//         setToCanonical(myTerm);
//         cout << "  ";
//         setToNonCanonical(myTerm);
//     }
// }

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

void moveCursorUpNonCanonical(int numOfSteps) { cout << "\033[" << numOfSteps << "A"; }

void moveCursorDownNonCanonical(int numOfSteps) { cout << "\033[" << numOfSteps << "B"; }

int main() {
    ofstream mylog;
    mylog.open("log.txt");
    vector<string> fileNames;
    struct winsize myTermSize;
    struct termios myTerm;
    int maxRows, maxCols;
    for (int i = 1; i < 10; i++) {
        string res;
        res += "-rwxrwxr-x 1 rohith rohith    17552 Oct  3 20:56 ";
        res += to_string(i);
        res += ".txt";
        fileNames.push_back(res);
    }
    calculateTermSize(myTermSize, maxRows, maxCols);
    getTermAttr(myTerm);
    int x = 1, y = 1;
    int index = 0;
    // setToNonCanonical(myTerm);
    clearScreen();
    moveCursor(1, 1);
    // setToCanonical(myTerm);
    for (auto s : fileNames) {
        cout << s << "\n";
    }
    moveCursor(1, 1);
    setToNonCanonical(myTerm);
    // take input only after making terminal non-canonical
    index = 0;
    while (true) {
        getCursor(&x, &y);
        calculateTermSize(myTermSize, maxRows, maxCols);
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
        } else if (ch == 27) {
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
            if (ch == 'k') {
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
    }
    return 0;
}