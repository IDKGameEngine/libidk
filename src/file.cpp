#include "libidk/file.hpp"
#include "libidk/assert.hpp"
#include "libidk/log.hpp"

#include <cstring>


idk::FileReader::FileReader(const char *path)
:   path_(path), data_(nullptr), size_(0)
{
    std::FILE *fh = std::fopen(path, "r");
    IDK_ASSERT(fh!=NULL, "Failure loading \"{}\"", path);

    std::fseek(fh, 0, SEEK_END);
    size_ = std::ftell(fh);
    data_ = std::malloc(size_);

    std::fseek(fh, 0, SEEK_SET);
    IDK_ASSERT(size_==std::fread(data_, 1, size_, fh), "Failure loading \"{}\"", path);
    std::fclose(fh);
}


idk::FileReader::~FileReader()
{
    if (data_)
    {
        std::free(data_);
    }
}




idk::FileWriter::FileWriter(const char *path)
:   path_(path),
    fh_(std::fopen(path, "w"))
{
    IDK_ASSERT(fh_ != NULL, "Failure creating FileWriter \"{}\"", path);
}

idk::FileWriter::~FileWriter()
{
    std::fclose(fh_);
}

void idk::FileWriter::write(const void *src, size_t size)
{
    std::fwrite(src, 1, size, fh_);
}
