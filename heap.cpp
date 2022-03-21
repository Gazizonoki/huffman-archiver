#include "heap.h"

void Heap::SiftDown() {
    size_t index = 0;
    while (2 * index + 1 < value_.size()) {
        size_t left = 2 * index + 1;
        size_t right = 2 * index + 2;
        size_t next_index = left;
        if (right < value_.size() &&
            value_[right]->priority <= value_[next_index]->priority) {
            next_index = right;
        }
        if (value_[index]->priority <= value_[next_index]->priority) {
            break;
        }
        std::swap(value_[index], value_[next_index]);
        index = next_index;
    }
}

void Heap::SiftUp(size_t index) {
    if (index > value_.size()) {
        return;
    }
    while (index > 0 &&
           value_[index]->priority < value_[(index - 1) / 2]->priority) {
        std::swap(value_[index], value_[(index - 1) / 2]);
        index = (index - 1) / 2;
    }
}

std::shared_ptr<Trie> Heap::GetMin() {
    if (value_.empty()) {
        return nullptr;
    }
    return value_[0];
}

void Heap::RemoveElement() {
    if (value_.empty()) {
        return;
    }
    value_[0] = value_.back();
    value_.pop_back();
    SiftDown();
}

void Heap::AddElement(const std::shared_ptr<Trie> &elem) {
    value_.push_back(elem);
    SiftUp(value_.size() - 1);
}

size_t Heap::Size() const { return value_.size(); }