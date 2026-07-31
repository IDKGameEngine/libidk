#pragma once

#include "libidk/types.hpp"
#include <cstdio>

namespace idk
{
    class FileReader;
    class FileWriter;
}


class idk::FileReader: public idk::NonCopyable, public idk::NonMovable
{
private:
    const char *path_;
    void  *data_;
    size_t size_;

public:
    FileReader(const char *filepath);
    ~FileReader();
    const void *getData() const { return data_; }
    size_t getSize() const { return size_; }
};


class idk::FileWriter: public idk::NonCopyable, public idk::NonMovable
{
private:
    const char *path_;
    std::FILE *fh_;

public:
    FileWriter(const char *filepath);
    ~FileWriter();
    void write(const void *src, size_t size);
};
