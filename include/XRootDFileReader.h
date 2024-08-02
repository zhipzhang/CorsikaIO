#pragma once

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include "FileReader.h"
#include "XrdCl/XrdClFile.hh"
#include "XrdCl/XrdClFileSystem.hh"
#include "XrdCl/XrdClXRootDResponses.hh"

class XRootDFileReader : public FileReader {
   public:
    XRootDFileReader(const std::string& filepath) {
        file_ = std::make_shared<XrdCl::File>();
        auto flag = file_->Open(filepath, XrdCl::OpenFlags::Read);
        if (!flag.IsOK()) {
            throw std::runtime_error("Failed to open XrootD file:" + filepath);
        }
    }

    size_t read(char* buffer, size_t size) override {
        XrdCl::XRootDStatus status;
        uint32_t bytesRead;
        status = file_->Read(current_offset_, static_cast<uint32_t>(size),
                             static_cast<void*>(buffer), bytesRead);
        if (!status.IsOK()) {
            throw std::runtime_error("Failed to read Xrootd file !");
        }
        current_offset_ += bytesRead;
        return static_cast<size_t>(bytesRead);
    }
    void seek(size_t offset, int whence) override {
        XrdCl::XRootDStatus status;
        uint64_t newPosition;
        switch (whence) {
            case SEEK_SET:
                current_offset_ = offset;
                break;
            case SEEK_CUR:
                current_offset_ += offset;
                break;
            default:
                throw std::invalid_argument("Invalid Argument for whence");
        }
    }

    size_t tell() override { return static_cast<size_t>(current_offset_); }

   private:
    std::shared_ptr<XrdCl::File> file_;
    uint64_t current_offset_;
};
