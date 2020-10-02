#include <algorithm>
#include <cstring>
#include <dirent.h>
#include <iostream>
#include <limits.h>
#include <sys/stat.h>
#include <unistd.h>
#include <vector>
using namespace std;

string getPermissionsInfo(struct stat &fileInfo) {
    string permissions;
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
        closedir(d);
    }
    return 0;
}
