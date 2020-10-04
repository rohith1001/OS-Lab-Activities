#include <string>
struct filerecord {
    std::string fileName;
    long long fileSize;
    std::string filePermissions;
    std::string fileLastModifiedTime;
    long long recordLength;
};