#include "GzipDecompressor.h"
#include <cstddef>

GzipDecompressor::GzipDecompressor(FileReader& file_reader)
    : file_reader_(file_reader) {
    strm_ = static_cast<z_stream*>(std::malloc(sizeof(z_stream)));
    if (strm_ == nullptr) {
        throw "";
    }
    strm_->zalloc = Z_NULL;
    strm_->zfree = Z_NULL;
    strm_->opaque = Z_NULL;
    int ret = inflateInit2(strm_, 16 + MAX_WBITS);
    if (ret != Z_OK) {
        std::free(strm_);
        throw "";
    }
    leftoverBuffer_.resize(4096);
    unprocessedData_.resize(4096);
    inputBuffer_.resize(4096);
    leftoverSize_ = 0;
}

size_t GzipDecompressor::read(char* buffer, size_t size) {
    size_t totalRead = 0;
    if (leftoverSize_ > 0) {
        size_t toCopy = std::min(leftoverSize_, size);
        std::memcpy(buffer, leftoverBuffer_.data() + leftoverPos_, toCopy);
        totalRead += toCopy;
        leftoverPos_ += toCopy;
        leftoverSize_ -= toCopy;
        if (totalRead == size) {
            return totalRead;
        }
    }
    bool continueInflating = true;
    while (totalRead < size && continueInflating) {
        std::memcpy(inputBuffer_.data(), unprocessedData_.data(),
                    unprocessedSize_);
        size_t bytesread =
            file_reader_.read(inputBuffer_.data() + unprocessedSize_,
                              inputBuffer_.size() - unprocessedSize_);
        if (bytesread < inputBuffer_.size() - unprocessedSize_) {
            continueInflating = false;
        }
        strm_->avail_in = bytesread + unprocessedSize_;
        strm_->next_in = reinterpret_cast<Bytef*>(inputBuffer_.data());
        strm_->avail_out = size - totalRead;
        strm_->next_out = reinterpret_cast<Bytef*>(buffer + totalRead);
        int ret = inflate(strm_, Z_NO_FLUSH);
        if (ret == Z_STREAM_ERROR || ret == Z_DATA_ERROR ||
            ret == Z_MEM_ERROR) {
            throw "";
        }
        size_t bytesRead = size - totalRead - strm_->avail_out;
        totalRead += bytesRead;
        if (ret == Z_STREAM_END) {
            continueInflating = false;
        }
        if (strm_->avail_out == 0 && strm_->avail_in > 0) {
            unprocessedSize_ = strm_->avail_in;
            std::memcpy(unprocessedData_.data(), strm_->next_in,
                        unprocessedSize_);
            strm_->avail_in = 0;
        }
    }
    return totalRead;
}
