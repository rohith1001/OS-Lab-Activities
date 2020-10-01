#include <iostream>
#include <limits.h>
#include <unistd.h>
using namespace std;

int main() {
    char cwd[4096];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        cout << "Current working dir:" << cwd << endl;
    } else {
        cout << "Error" << endl;
        return 1;
    }
    return 0;
}
