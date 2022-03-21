#include "compress.h"
#include "heap.h"

#include <algorithm>
#include <unordered_map>

struct CharWithLength {
    bool operator<(const CharWithLength &another) const {
        return std::tie(this->length, c) < std::tie(another.length, another.c);
    }

    uint32_t c;
    size_t length;
};

void GetCharFrequency(const std::string &filename, Reader &input, std::unordered_map<uint32_t, size_t> &count) {
    for (unsigned char c: filename) {
        ++count[c];
    }
    while (!input.Eof()) {
        unsigned char c = input.ReadChar();
        if (!input.Eof()) {
            ++count[c];
        }
    }
    ++count[256];
    ++count[257];
    ++count[258];
}

std::shared_ptr<Trie> GetHuffmanCodes(const std::unordered_map<uint32_t , size_t> &count) {
    Heap trie_queue;
    for (auto pair: count) {
        trie_queue.AddElement(std::make_shared<Trie>(pair.second, nullptr, nullptr, true, pair.first));
    }
    while (trie_queue.Size() > 1) {
        std::shared_ptr<Trie> first_ptr = trie_queue.GetMin();
        trie_queue.RemoveElement();
        std::shared_ptr<Trie> second_ptr = trie_queue.GetMin();
        trie_queue.RemoveElement();
        trie_queue.AddElement(
                std::make_shared<Trie>(first_ptr->priority + second_ptr->priority, first_ptr, second_ptr, false, '\0'));
    }
    return trie_queue.GetMin();
}

void GetHuffmanCodeLength(const std::shared_ptr<Trie> &vertex, std::unordered_map<uint32_t , size_t> &code_length,
                          size_t depth = 0) {
    if (vertex == nullptr) {
        return;
    }
    if (vertex->is_terminate) {
        code_length[vertex->c] = depth;
    }
    GetHuffmanCodeLength(vertex->l, code_length, depth + 1);
    GetHuffmanCodeLength(vertex->r, code_length, depth + 1);
}

void WriteData(const std::string &filename, std::unordered_map<uint32_t , size_t> &code_length,
               std::unordered_map<uint32_t, size_t> &char_code, const std::vector<size_t> &len_count,
               const std::vector<CharWithLength> &chars,
               Reader &input, Writer &output) {
    output.WriteBits(char_code.size(), 9);
    for (auto c: chars) {
        output.WriteBits(static_cast<size_t>(c.c), 9);
    }
    for (size_t i = 1; i < len_count.size(); ++i) {
        output.WriteBits(len_count[i], 9);
    }
    for (auto c: filename) {
        output.WriteBits(char_code[c], code_length[c]);
    }
    output.WriteBits(char_code[256], code_length[256]);
    input.Open(filename);
    while (!input.Eof()) {
        uint32_t c = input.ReadChar();
        if (!input.Eof()) {
            output.WriteBits(char_code[c], code_length[c]);
        }
    }
}

void Compress(const std::string &filename, Reader &input, Writer &output, bool is_last) {
    std::unordered_map<uint32_t, size_t> count, code_length, char_code;
    GetCharFrequency(filename, input, count);
    GetHuffmanCodeLength(GetHuffmanCodes(count), code_length);
    std::vector<CharWithLength> chars;
    size_t max_symbol_code_size = 0;
    for (auto pair: code_length) {
        max_symbol_code_size = std::max(max_symbol_code_size, pair.second);
        chars.push_back({pair.first, pair.second});
    }
    std::sort(chars.begin(), chars.end());
    std::vector<size_t> len_count(max_symbol_code_size + 1);
    uint32_t current_code = 0;
    for (size_t i = 0; i < chars.size(); ++i) {
        ++len_count[chars[i].length];
        if (i > 0 && chars[i].length > chars[i - 1].length) {
            current_code <<= chars[i].length - chars[i - 1].length;
        }
        char_code[chars[i].c] = ReverseNumber(current_code, static_cast<int> (chars[i].length));
        ++current_code;
    }
    WriteData(filename, code_length, char_code, len_count, chars, input, output);
    if (is_last) {
        output.WriteBits(char_code[258], code_length[258]);
    }
    else {
        output.WriteBits(char_code[257], code_length[257]);
    }
}