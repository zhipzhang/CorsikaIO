#pragma once
#include <zconf.h>
#include <zlib.h>
#include <cstdlib>
#include <cstring>
#include <vector>
#include "FileReader.h"

class GzipDecompressor {
   public:
    GzipDecompressor(FileReader& file_reader);
    ~GzipDecompressor() {
        inflateEnd(strm_);
        std::free(strm_);
    }
    size_t read(char* buffer, size_t size);  // read the bytes

   private:
    FileReader& file_reader_;
    z_stream* strm_;
    std::vector<char> leftoverBuffer_;
    size_t leftoverSize_ = 0;
    size_t leftoverPos_ = 0;
    std::vector<char> unprocessedData_;
    size_t unprocessedSize_ = 0;
    std::vector<char> inputBuffer_;
};
