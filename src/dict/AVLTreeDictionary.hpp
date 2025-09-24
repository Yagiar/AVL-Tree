#pragma once

#include <memory>
#include <optional>
#include <string>

#include "IDictionary.hpp"

namespace dict {

// Заглушка реализации словаря на основе AVL-дерева (самобалансирующееся БДП)
class AVLTreeDictionary : public IDictionary {
public:
    AVLTreeDictionary() = default;
    ~AVLTreeDictionary() override = default;

    bool insert(int key, const std::string &value) override;
    void upsert(int key, const std::string &value) override;
    bool erase(int key) override;
    std::optional<std::string> find(int key) const override;
    void clear() override;
    std::size_t size() const override;

private:
    struct Node {
        int key = 0;
        std::string value;
        int height = 1;
        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;
    };
    std::unique_ptr<Node> root_;
    std::size_t size_ = 0;
};

} // namespace dict


