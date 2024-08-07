#pragma once
#include <zstd.h>
#include <cstdio>
#include <cstring>
#include <stdexcept>
#include <vector>
#include "FileReader.h"
class ZstdDecompressor {
   public:
    ZstdDecompressor(FileReader& file_reader);
    ~ZstdDecompressor() { ZSTD_freeDStream(dctx_); }
    size_t read(char* buffer, size_t size);

   private:
    FileReader& file_reader_;
    ZSTD_DStream* dctx_;  // decompression context
    std::vector<char> leftoverBuffer_;
    size_t leftoverSize_ = 0;
    size_t leftoverPos_ = 0;
};
