#include <utility>
#include <memory>

struct Trie {
    Trie(size_t priority, std::shared_ptr<Trie> l, std::shared_ptr<Trie> r, bool is_terminate, uint32_t c)
            : priority(priority), l(std::move(l)), r(std::move(r)), is_terminate(is_terminate), c(c) {};
    size_t priority = 0;
    std::shared_ptr<Trie> l = nullptr, r = nullptr;
    bool is_terminate = false;
    uint32_t c;
};
