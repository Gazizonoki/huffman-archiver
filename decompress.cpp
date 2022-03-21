#include "decompress.h"
#include "writer.h"

#include <map>

struct CodeWithLength {
    CodeWithLength(size_t code, int length) : code(code), length(length) {};
    size_t code = 0;
    int length = 0;
    bool operator<(const CodeWithLength& another) const {
        return std::tie(length, code) < std::tie(another.length, another.code);
    }
};

void GetCharCodes(Reader &input, std::map<CodeWithLength, uint32_t> &char_codes) {
    std::vector<uint32_t> chars;
    size_t symbols_count = input.ReadBytesToInt(9);
    for (size_t i = 0; i < symbols_count; ++i) {
        chars.push_back(static_cast<uint32_t> (input.ReadBytesToInt(9)));
    }
    size_t current_count = 0;
    size_t current_code = 0;
    int current_len = 1;
    while (current_count < symbols_count) {
        size_t cnt = input.ReadBytesToInt(9);
        for (size_t i = current_count; i < current_count + cnt; ++i) {
            char_codes[CodeWithLength(ReverseNumber(current_code, current_len), current_len)] = chars[i];
            ++current_code;
        }
        ++current_len;
        current_count += cnt;
        current_code <<= 1;
    }
}

bool DecompressOneFile(Reader &input) {
    std::map<CodeWithLength, uint32_t> char_codes;
    GetCharCodes(input, char_codes);
    size_t current_code = 0;
    size_t last_bit = 0;
    std::string filename;
    while (true) {
        if (input.ReadBytesToInt(1)) {
            current_code += (1 << last_bit);
        }
        ++last_bit;
        if (char_codes.find(CodeWithLength(current_code, static_cast<int>(last_bit))) != char_codes.end()) {
            uint32_t cur_char = char_codes[CodeWithLength(current_code, static_cast<int>(last_bit))];
            if (cur_char == 256) {
                break;
            }
            filename += static_cast<char>(cur_char);
            current_code = 0;
            last_bit = 0;
        }
    }
    current_code = 0;
    last_bit = 0;
    Writer output(filename);
    bool is_end = false;
    while (true) {
        if (input.ReadBytesToInt(1)) {
            current_code += (1 << last_bit);
        }
        ++last_bit;
        if (char_codes.find(CodeWithLength(current_code, static_cast<int>(last_bit))) != char_codes.end()) {
            uint32_t cur_char = char_codes[CodeWithLength(current_code, static_cast<int>(last_bit))];
            if (cur_char == 257) {
                break;
            }
            if (cur_char == 258) {
                is_end = true;
                break;
            }
            output.WriteBits(cur_char, 8);
            current_code = 0;
            last_bit = 0;
        }
    }

    return is_end;
}

void Decompress(Reader &input) {
    while (!DecompressOneFile(input));
}