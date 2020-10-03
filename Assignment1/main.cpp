#include <algorithm>
#include <cstring>
#include <dirent.h>
#include <iostream>
#include <limits.h>
#include <sys/stat.h>
#include <termios.h>
#include <unistd.h>
#include <vector>
#define cursorUpward cout << "\033[A"
#define cursorDownward cout << "\033[B"
using namespace std;

void changeFromCanonical(struct termios &myTerm) {
    myTerm.c_lflag &= ~(ICANON | ECHO);
    myTerm.c_cc[VMIN] = 1;
    myTerm.c_cc[VTIME] = 0;
    tcsetattr(0, TCSANOW, &myTerm);
}

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

int main() {
    char cwd[PATH_MAX];
    struct termios myTerm, copyTerm;
    tcgetattr(0, &myTerm);
    memcpy(&copyTerm, &myTerm, sizeof(copyTerm));

    vector<string> fileNamesVector;
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        cout << "Error in getting current directory path...Exiting" << endl;
        return 1;
    }
    DIR *d;
    struct dirent *dir;
    d = opendir(cwd);
    if (d) {
        cout << "\033[2J\033[1;1H";
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
                return 1;
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
        changeFromCanonical(myTerm);
        cout << "\033[1;1H";
        while (true) {
            char ch;
            ch = cin.get();
            if (ch == 'k') {
                cursorUpward;
            } else if (ch == 'l') {
                cursorDownward;
            } else if (ch == '\n') {
                tcsetattr(0, TCSANOW, &copyTerm);
                cout << "You have pressed enter";
            }
        }
        tcsetattr(0, TCSANOW, &copyTerm);
        closedir(d);
    }
    return 0;
}
