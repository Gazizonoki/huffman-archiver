#pragma once

#include <fstream>

size_t ReverseNumber(size_t value, int len);

class Writer {
public:
    explicit Writer(const std::string &filename);

    ~Writer();

    void Open(const std::string &filename);

    void WriteBits(size_t c, size_t len);

private:

    void FlushToFile();

    unsigned char buffer_ = 0;
    int last_index_ = 0;
    std::ofstream out_;
};