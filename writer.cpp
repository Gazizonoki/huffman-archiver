#include "writer.h"

Writer::Writer(const std::string &filename) {
    out_.open(filename, std::ios::binary);
    buffer_ = 0;
    last_index_ = 0;
}

Writer::~Writer() {
    if (last_index_ != 0) {
        FlushToFile();
    }
    out_.close();
}

void Writer::Open(const std::string &filename) {
    if (last_index_ != 0) {
        FlushToFile();
    }
    out_.close();
    out_.open(filename, std::ios::binary);
    buffer_ = 0;
    last_index_ = 8;
}

void Writer::WriteBits(size_t c, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        if (last_index_ == 8) {
            FlushToFile();
        }
        if ((c >> i) & 1) {
            buffer_ += 1 << last_index_;
        }
        ++last_index_;
    }
}

size_t ReverseNumber(size_t value, int len) {
    size_t ans = 0;
    for (int i = len - 1; i >= 0; --i) {
        if ((value >> i) & 1) {
            ans += 1 << (len - 1 - i);
        }
    }
    return ans;
}

void Writer::FlushToFile() {
    out_.put(static_cast<char>(buffer_));
    buffer_ = 0;
    last_index_ = 0;
}