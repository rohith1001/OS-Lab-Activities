#include <dirent.h>
#include <iostream>
using namespace std;
int main() {
    DIR *dir;
    dirent *pdir;
    dir = opendir(".");
    while ((pdir = readdir(dir)) != NULL) {
        cout << pdir->d_name << endl;
    }
    closedir(dir);
    return 0;
}