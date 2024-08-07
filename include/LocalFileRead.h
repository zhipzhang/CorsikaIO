#pragma once
#include <fstream>
#include <ios>
#include "FileReader.h"
class LocalFileReader : public FileReader {
   public:
    LocalFileReader(const std::string& filepath) {
        file_.open(filepath, std::ios::binary);
    }
    size_t read(char* buffer, size_t size) override {
        file_.read(buffer, size);
        return file_.gcount();
    }
    void seek(size_t offset, int whence) override {
        file_.seekg(offset, static_cast<std::ios_base::seekdir>(whence));
    }
    size_t tell() override { return file_.tellg(); }

   private:
    std::ifstream file_;
};
