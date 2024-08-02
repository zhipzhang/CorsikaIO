#pragma  once
#include <zstd.h>
#include <cstdio>
#include <cstring>
#include <stdexcept>
#include <vector>
#include "FileReader.h"
class ZstdDecompressor {
   public:
    ZstdDecompressor(FileReader& file_reader) : file_reader_(file_reader) {
        dctx_ = ZSTD_createDStream();
        if (dctx_ == nullptr) {
            throw std::runtime_error("Failed to create ZSTD_DStream");
        }
        size_t const initResult = ZSTD_initDStream(dctx_);
        if (ZSTD_isError(initResult)) {
            throw "";
        }
        leftoverBuffer_.resize(ZSTD_DStreamOutSize());
    }
    ~ZstdDecompressor() { ZSTD_freeDStream(dctx_); }
    size_t read(char* buffer, size_t size) {
        size_t totalRead = 0;
        if (leftoverSize_ > 0) {
            size_t toCopy = std::min(leftoverSize_, size);
            std::memcmp(buffer, leftoverBuffer_.data() + leftoverPos_, toCopy);
            totalRead += toCopy;
            leftoverPos_ += toCopy;
            leftoverSize_ -= toCopy;
            if (totalRead == size) {
                return totalRead;
            }
        }
        std::vector<char> inputBuffer(ZSTD_DStreamInSize());
        std::vector<char> outputBuffer(ZSTD_DStreamOutSize());
        ZSTD_inBuffer inBuffer = {inputBuffer.data(), 0, 0};
        ZSTD_outBuffer outBuffer = {outputBuffer.data(), outputBuffer.size(),
                                    0};
        while (totalRead < size) {
            if (inBuffer.pos == inBuffer.size) {
                inBuffer.size =
                    file_reader_.read(inputBuffer.data(), inputBuffer.size());
                inBuffer.pos = 0;
                if (inBuffer.size == 0) {
                    break;
                }
            }
            size_t const ret =
                ZSTD_decompressStream(dctx_, &outBuffer, &inBuffer);
            if (ZSTD_isError(ret)) {
                throw "";
            }
            size_t toCopy = std::min(outBuffer.pos, size - totalRead);
            std::memcpy(buffer + totalRead, outputBuffer.data(), toCopy);
            totalRead += toCopy;
            if (outBuffer.pos > toCopy) {
                leftoverSize_ = outBuffer.pos - toCopy;
                std::memcpy(leftoverBuffer_.data(),
                            outputBuffer.data() + toCopy, leftoverSize_);
                leftoverPos_ = 0;
            } else {
                leftoverSize_ = 0;
            }
            if (ret == 0 && inBuffer.pos == inBuffer.size) {
                break;
            }
        }
        return totalRead;
    }

   private:
    FileReader& file_reader_;
    ZSTD_DStream* dctx_;  // decompression context
    std::vector<char> leftoverBuffer_;
    size_t leftoverSize_ = 0;
    size_t leftoverPos_ = 0;
};
