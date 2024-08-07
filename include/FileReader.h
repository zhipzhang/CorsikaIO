#pragma once
#include <cstddef>
class FileReader {
   public:
    virtual ~FileReader() {}
    virtual size_t read(char* buffer, size_t size) = 0;
    //------------------------------------------------------------------------
    //! Similar to the C file seek method
    //!
    //! @param offset     The offset you want to set
    //! @param whence     Some situations you can set: SEEK_CUR and SEEK_SET
    //------------------------------------------------------------------------
    virtual void seek(size_t offset, int whence) = 0;
    virtual size_t tell() = 0;
};
