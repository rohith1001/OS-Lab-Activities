#include <cmath>
#include <iostream>
#include <string>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#include <vector>
#define KEY_UP 72
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_DOWN 80
using namespace std;

void getCursor(int *x, int *y) {
    cout << "\033[6n";
    // cout << "\e[6n";
    // better to use scanf here
    scanf("\033[%d;%dR", x, y);
}

void moveCursorNonCanonical(int x, int y) { cout << "\033[" << x << ";" << y << "H"; }

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
int main() {
    // cout << "Hello"
    //      << "\n";
    // cout << "Hello"
    //      << "\n";
    // cout << "Hello"
    //      << "\n";
    // cout << "Hello"
    //      << "\n";
    // cout << "Hello"
    //      << "\n";

    // int x, y;
    char c, d, e;
    int a;
    struct termios myTerm;
    tcgetattr(0, &myTerm);
    setToNonCanonical(myTerm);
    cin >> c;
    setToCanonical(myTerm);
    // getCursor(&x, &y);
    // moveCursorNonCanonical(1, 1);
    // cout << "x is: " << x << endl;
    // cout << "y is: " << y << endl;
    return 0;
}