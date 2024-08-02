#pragma  once
#include <cstddef>
class FileReader {
   public:
    virtual ~FileReader() {}
    virtual size_t read(char* buffer, size_t size) = 0;
    virtual void seek(size_t offset, int whence) = 0;
    virtual size_t tell() = 0;
};
