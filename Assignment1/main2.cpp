#include <cerrno>
#include <cstring>
#include <ctime>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
using namespace std;

int main(int argc, char **argv) {
    struct stat fileInfo;

    if (argc < 2) {
        cout << "Usage: fileinfo <file name>\n";
        return (EXIT_FAILURE);
    }

    if (stat(argv[1], &fileInfo) != 0) { // Use stat() to get the info
        cerr << "Error: " << strerror(errno) << '\n';
        return (EXIT_FAILURE);
    }

    cout << "Type:         : ";
    if ((fileInfo.st_mode & S_IFMT) == S_IFDIR) { // From sys/types.h
        cout << "Directory\n";
    } else {
        cout << "File\n";
    }

    cout << "Size          : " << fileInfo.st_size << '\n';              // Size in bytes
    cout << "Device        : " << (char)(fileInfo.st_dev + 'A') << '\n'; // Device number
    cout << "Created       : " << ctime(&fileInfo.st_ctime);             // Creation time
    cout << "Modified      : " << ctime(&fileInfo.st_mtime);             // Last mod time
}