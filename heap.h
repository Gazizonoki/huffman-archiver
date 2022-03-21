#pragma once

#include "trie.cpp"

#include <vector>

class Heap {
public:
    void AddElement(const std::shared_ptr<Trie> &elem);

    void RemoveElement();

    std::shared_ptr<Trie> GetMin();

    size_t Size() const;

private:
    void SiftUp(size_t index);

    void SiftDown();

    std::vector<std::shared_ptr<Trie>> value_;
};
