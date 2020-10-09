#include <bits/stdc++.h>
#include <dirent.h>
#include <grp.h>
#include <pwd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>
using namespace std;

#define KEY_ESCAPE 0x001b
#define KEY_ENTER 0x000a
#define KEY_UP 0x0105
#define KEY_DOWN 0x0106
#define KEY_LEFT 0x0107
#define KEY_RIGHT 0x0108

#define cursorUpward(x) printf("\033[%dA", (x))
#define cursorDownward(x) printf("\033[%dB", (x))

char cwd[PATH_MAX];
struct dirent *dir;
vector<string> fileNamesVector;
struct stat fileInfo;
int gX, gY;
const int batch_size = 10;
struct termios myTerm, oterm;

void handleUpCase(int &x, int n, int batch_number);
void handleDownCase(int &x, int n, int batch_number);
void setToCanonical();
void setToNonCanonical();

void getCurrentDirectory() {
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        cout << "Error in getting current directory path...Exiting" << endl;
    }
}

int getch() {
    int c = 0;
    tcgetattr(0, &oterm);
    memcpy(&myTerm, &oterm, sizeof(myTerm));
    myTerm.c_lflag &= ~(ICANON | ECHO);
    myTerm.c_cc[VMIN] = 1;
    myTerm.c_cc[VTIME] = 0;
    tcsetattr(0, TCSANOW, &myTerm);
    c = getchar();
    tcsetattr(0, TCSANOW, &oterm);
    return c;
}

int kbhit(void) {
    int c = 0;
    tcgetattr(0, &oterm);
    memcpy(&myTerm, &oterm, sizeof(myTerm));
    myTerm.c_lflag &= ~(ICANON | ECHO);
    myTerm.c_cc[VMIN] = 0;
    myTerm.c_cc[VTIME] = 1;
    tcsetattr(0, TCSANOW, &myTerm);
    c = getchar();
    tcsetattr(0, TCSANOW, &oterm);
    if (c != -1)
        ungetc(c, stdin);
    return ((c != -1) ? 1 : 0);
}

int kbesc(void) {
    int c;

    if (!kbhit())
        return KEY_ESCAPE;
    c = getch();
    if (c == '[') {
        switch (getch()) {
        case 'A':
            c = KEY_UP;
            break;
        case 'B':
            c = KEY_DOWN;
            break;
        case 'C':
            c = KEY_LEFT;
            break;
        case 'D':
            c = KEY_RIGHT;
            break;
        default:
            c = 0;
            break;
        }
    } else {
        c = 0;
    }
    if (c == 0)
        while (kbhit())
            getch();
    return c;
}

static int kbget(void) {
    int c;
    c = getch();
    return (c == KEY_ESCAPE) ? kbesc() : c;
}

void clearScreen() { cout << "\033[2J"; }

void moveCursor(int x, int y) { cout << "\033[" << x << ";" << y << "H"; }

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

void getUserAndGroup(string &userName, string &groupName) {
    struct passwd *pw = getpwuid(fileInfo.st_uid);
    struct group *gr = getgrgid(fileInfo.st_gid);
    string s1(pw->pw_name); // for user
    string s2(gr->gr_name); // for group
    userName = s1;
    groupName = s2;
}

void simplyPrintInBatches(vector<string> &fileRecords, int from, int to) {
    for (int i = from; i < to; i++) {
        cout << fileRecords[i];
    }
}

void setToCanonical() {
    oterm.c_lflag |= (ICANON | ECHO);
    tcsetattr(0, TCSANOW, &oterm);
}

void setToNonCanonical() {
    myTerm.c_lflag &= ~(ICANON | ECHO);
    // myTerm.c_cc[VMIN] = 1;  // minimum characters for non-canonical read
    // myTerm.c_cc[VTIME] = 0; // timeout in deciseconds for non-canonical read
    tcsetattr(0, TCSANOW, &myTerm);
}

void displayFileRecordsInBatches(vector<string> &fileRecords, int &batch_number) {
    clearScreen();
    moveCursor(1, 1);
    int n = fileRecords.size();
    int from = (batch_number - 1) * batch_size;
    int to = (batch_number * batch_size);
    if (to > n) {
        to = n;
    }
    simplyPrintInBatches(fileRecords, from, to);
}

string convetLmtToString(time_t mt) {
    time_t t = mt;
    struct tm lt;
    localtime_r(&t, &lt);
    char timbuf[80];
    strftime(timbuf, sizeof(timbuf), "%c", &lt);
    string s(timbuf);
    return s;
}

// void accept_input(vector<string> &fileRecords, int &batch_number, int &x) {
//     int n = fileRecords.size();
//     int n_batches = ceil(n / batch_size);
//     while (true) {
//         char ch;
//         ch = cin.get();
//         if (ch == '\033') {
//             char d, e;
//             d = cin.get();
//             e = cin.get();
//             if (e == 'A') {
//                 handleUpCase(x, n, batch_number);
//             } else if (e == 'B') {
//                 handleDownCase(x, n, batch_number);
//             }
//         } else if (ch == 'k') {
//             batch_number++;
//             batch_number = (batch_number > n_batches) ? 1 : batch_number;
//             break;
//         } else if (ch == 'l') {
//             batch_number--;
//             batch_number = (batch_number < 1) ? n_batches : 1;
//             break;
//         } else {
//             break;
//         }
//     }
// }

void accept_input(vector<string> &fileRecords, int &batch_number, int &x) {
    int n = fileRecords.size();
    int n_batches = ceil(n / batch_size);
    while (true) {
        int c;
        c = kbget();
        if (c == KEY_UP) {
            cursorUpward(1);
        } else if (c == KEY_DOWN) {
            cursorDownward(1);
        } else if (c == 107) { // for pressing k
            batch_number++;
            batch_number = (batch_number > n_batches) ? 1 : batch_number;
            break;
        } else if (c == 108) { // for pressing l
            batch_number--;
            batch_number = (batch_number < 1) ? n_batches : 1;
            break;
        }
    }
}

void normal_mode(vector<string> fileRecords) {
    int x = 1, batch_number = 1;
    displayFileRecordsInBatches(fileRecords, batch_number);
    moveCursor(1, 1);
    // setToNonCanonical();
    accept_input(fileRecords, batch_number, x);
    // setToCanonical();
    cout << "The accepted input is: x = " << x << "batch_number = " << batch_number << endl;
}

void collectAllFilesInDir() {
    vector<string> fileRecords;
    DIR *d;
    d = opendir(cwd);
    if (d) {
        clearScreen();
        moveCursor(1, 1);
        while ((dir = readdir(d)) != NULL) {
            fileNamesVector.push_back(dir->d_name);
        }
        sort(fileNamesVector.begin(), fileNamesVector.end());
        for (auto f : fileNamesVector) {
            char fileName[f.size() + 1];
            strcpy(fileName, f.c_str());
            string userName = "", groupName = "";
            string fileRecordString = "";
            if (stat(fileName, &fileInfo) != 0) {
                cout << "Error accessing file stat...Exiting" << endl;
                return;
            }
            fileRecordString += getPermissionsInfo(fileInfo) + "\t";
            fileRecordString += to_string(fileInfo.st_size) + "\t";
            fileRecordString += convetLmtToString(fileInfo.st_mtime) + "\t";
            fileRecordString += f + "\n";
            fileRecords.push_back(fileRecordString);
            fileRecordString = "";
        }
    }
    normal_mode(fileRecords);
}

void getTermAttr() {
    tcgetattr(0, &myTerm);
    tcgetattr(0, &oterm);
}

void getCursor(int *x, int *y) {
    cout << "\033[6n";
    scanf("\033[%d;%dR", x, y);
}

void moveCursorUp() { cout << "\033[1A"; }

void moveCursorDown() { cout << "\033[1B"; }

void handleUpCase(int &x, int n, int batch_number) {
    x--;
    int from = (batch_number - 1) * batch_size;
    int to = (batch_number * batch_size);
    if (to > n) {
        to = n;
    }
    int curr_batch_size = from - to + 1;

    if (x < 1) {
        moveCursor(curr_batch_size, 1);
        x = curr_batch_size;
    } else {
        moveCursorUp();
    }
}

void handleDownCase(int &x, int n, int batch_number) {
    x++;
    int from = (batch_number - 1) * batch_size;
    int to = (batch_number * batch_size);
    if (to > n) {
        to = n;
    }
    int curr_batch_size = from - to + 1;

    if (x > curr_batch_size) {
        moveCursor(1, 1);
        x = 1;
    } else {
        moveCursorDown();
    }
}

int main() {
    // getTermAttr();
    getCurrentDirectory();
    collectAllFilesInDir();
    return 0;
}
