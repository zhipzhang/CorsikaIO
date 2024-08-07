#pragma once
#include <memory>
#include <variant>
#include "FileReader.h"
#include "GzipDecompressor.h"
#include "ZstdDecompressor.h"

class CorsikaReader {
   public:
    CorsikaReader(const std::string& filepath,
                  const std::string& url = "root://eos01.ihep.ac.cn");

   private:
    std::string DetermineDecompressionType();
    size_t rawread(char* buffer, size_t size);
    std::unique_ptr<FileReader> file_reader_;
    std::variant<std::monostate, GzipDecompressor, ZstdDecompressor>
        decompressor_;
};
