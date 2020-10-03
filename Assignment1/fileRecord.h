#include <string>
struct filerecord {
    char *fileName;
    long long fileSize;
    std::string filePermissions;
    std::string fileLastModifiedTime;
};