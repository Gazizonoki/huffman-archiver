#pragma once

#include <fstream>
#include <vector>

class Reader {
public:
    explicit Reader(const std::string &filename);

    ~Reader();

    void Open(const std::string &filename);

    std::vector<bool> ReadBits(size_t cnt);

    std::vector<bool> ReadBitsToEnd(size_t cnt);

    size_t ReadBytesToInt(size_t cnt);

    unsigned char ReadChar();

    bool Eof();

private:
    std::ifstream in_;
    unsigned char buffer_ = 0;
    int last_index_ = 8;
};