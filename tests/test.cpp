#include <gtest/gtest.h>

#include "../compress.h"
#include "../decompress.h"
#include "../heap.h"

TEST(Reader, test1) {
    Reader input("mock/test1.txt");
    std::string expected_data = "cpp is my life\n"
                                "best language ever\n"
                                "lol";
    std::string my_data;
    while (!input.Eof()) {
        unsigned char c = input.ReadChar();
        if (input.Eof()) {
            break;
        }
        my_data += static_cast<char>(c);
    }
    ASSERT_EQ(expected_data, my_data);
}

TEST(Writer, test2) {
    Writer output("mock/output_test2.txt");
    std::string text = "this is my text\n"
                       "is it text\n\3\6\1\2\6";
    for (auto c : text) {
        output.WriteBits(static_cast<size_t>(c), 8);
    }
    output.~Writer();
    Reader input("mock/output_test2.txt");
    std::string my_data;
    while (!input.Eof()) {
        unsigned char c = input.ReadChar();
        if (input.Eof()) {
            break;
        }
        my_data += static_cast<char>(c);
    }
    ASSERT_EQ(my_data, text);
}

TEST(Heap, test3) {
    Heap priority_queue;
    priority_queue.AddElement(std::make_shared<Trie>
            (20, nullptr, nullptr, false, '\0'));
    priority_queue.AddElement(std::make_shared<Trie>
                                      (100, nullptr, nullptr, false, '\0'));
    ASSERT_EQ(20, priority_queue.GetMin()->priority);
    priority_queue.AddElement(std::make_shared<Trie>
                                     (10, nullptr, nullptr, false, '\0'));
    priority_queue.AddElement(std::make_shared<Trie>
                                     (18, nullptr, nullptr, false, '\0'));
    priority_queue.RemoveElement();
    ASSERT_EQ(18, priority_queue.GetMin()->priority);
    priority_queue.RemoveElement();
    priority_queue.RemoveElement();
    ASSERT_EQ(100, priority_queue.GetMin()->priority);
}

TEST(Compress, test4) {
    Reader input("mock/test4.txt");
    std::vector<bool> expected_data = input.ReadBitsToEnd(1000);
    input.Open("mock/test4.txt");
    Writer output("mock/compressed_test4.o");
    Compress("mock/test4.txt", input, output, true);
    output.~Writer();
    input.Open("mock/compressed_test4.o");
    Decompress(input);
    input.Open("mock/test4.txt");
    std::vector<bool> my_data = input.ReadBitsToEnd(1000);
    ASSERT_EQ(expected_data, my_data);
}

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}