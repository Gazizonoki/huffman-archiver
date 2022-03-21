#include "reader.h"

Reader::Reader(const std::string &filename) {
    in_.open(filename, std::ios::binary);
    buffer_ = 0;
    last_index_ = 8;
}

Reader::~Reader() {
    in_.close();
}

void Reader::Open(const std::string &filename) {
    in_.close();
    in_.open(filename, std::ios::binary);
    buffer_ = 0;
    last_index_ = 8;
}

std::vector<bool> Reader::ReadBits(size_t cnt) {
    std::vector<bool> answer;
    while (cnt > 0 && !in_.eof()) {
        if (last_index_ == 8) {
            last_index_ = 0;
            char c;
            in_.read(&c, 1);
            if (in_.eof()) {
                break;
            }
            buffer_ = static_cast<unsigned char>(c);
        }
        --cnt;
        answer.push_back(buffer_ & 1);
        buffer_ >>= 1;
        ++last_index_;
    }
    while (cnt > 0) {
        answer.push_back(false);
        --cnt;
    }
    return answer;
}

std::vector<bool> Reader::ReadBitsToEnd(size_t cnt) {
    std::vector<bool> answer;
    while (cnt > 0 && !in_.eof()) {
        if (last_index_ == 8) {
            last_index_ = 0;
            char c;
            in_.read(&c, 1);
            if (in_.eof()) {
                break;
            }
            buffer_ = static_cast<unsigned char>(c);
        }
        --cnt;
        answer.push_back(buffer_ & 1);
        buffer_ >>= 1;
        ++last_index_;
    }
    return answer;
}

unsigned char Reader::ReadChar() {
    std::vector<bool> answer = ReadBits(8);
    unsigned char c = 0;
    for (int i = 0; i < 8; ++i) {
        if (answer[i]) {
            c += 1 << i;
        }
    }
    return c;
}

size_t Reader::ReadBytesToInt(size_t cnt) {
    std::vector <bool> answer = ReadBits(cnt);
    size_t n = 0;
    for (int i = 0; i < cnt; ++i) {
        if (answer[i]) {
            n += 1 << i;
        }
    }
    return n;
}

bool Reader::Eof() {
    return in_.eof();
}