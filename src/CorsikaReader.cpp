#include "CorsikaReader.h"
#include <cstddef>
#include <cstdio>
#include <format>
#include <iostream>
#include <memory>
#include <type_traits>
#include <variant>
#include "LocalFileRead.h"
#include "XRootDFileReader.h"

CorsikaReader::CorsikaReader(const std::string& filepath,
                             const std::string& url) {
    if (filepath.find("/eos") == 0) {
        std::cout << std::format("Initilize the remote file ");
        file_reader_ = std::make_unique<XRootDFileReader>(url + filepath);
    } else {
        file_reader_ = std::make_unique<LocalFileReader>(filepath);
    }
    auto decompression_type = DetermineDecompressionType();
    if (decompression_type == "gzip") {
        decompressor_.emplace<GzipDecompressor>(*file_reader_);
    } else if (decompression_type == "zstd") {
        decompressor_.emplace<ZstdDecompressor>(*file_reader_);
    } else if (decompression_type == "raw") {
        decompressor_.emplace<std::monostate>();
    }
};
std::string CorsikaReader::DetermineDecompressionType() {
    unsigned char buf[4];
    file_reader_->read(reinterpret_cast<char*>(buf), 4);
    file_reader_->seek(0, SEEK_SET);
    if (buf[0] == 0x1F && buf[1] == 0x8B) {
        return "gzip";
    } else if (buf[0] == 0x28 && buf[1] == 0xB5 && buf[2] == 0x2F &&
               buf[3] == 0xFD) {
        return "zstd";
    } else {
        return "raw";
    }
}
size_t CorsikaReader::rawread(char* buffer, size_t size) {
    return std::visit(
        [buffer, size, this](auto& decompressor) -> size_t {
            using T = std::decay_t<decltype(decompressor)>;
            if constexpr (std::is_same_v<T, std::monostate>) {
                return file_reader_->read(buffer, size);
            } else {
                return decompressor.read(buffer, size);
            }
        },
        decompressor_);
}
