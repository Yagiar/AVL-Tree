#include "AVLTreeDictionary.hpp"

namespace dict {

// Определение Node перенесено в заголовок

bool AVLTreeDictionary::insert(int /*key*/, const std::string & /*value*/) {
    // Заглушка: пока всегда false (не вставлено)
    return false;
}

void AVLTreeDictionary::upsert(int /*key*/, const std::string & /*value*/) {
    // Заглушка: ничего не делает
}

bool AVLTreeDictionary::erase(int /*key*/) {
    // Заглушка: ничего не удалено
    return false;
}

std::optional<std::string> AVLTreeDictionary::find(int /*key*/) const {
    return std::nullopt;
}

void AVLTreeDictionary::clear() {
    root_.reset();
    size_ = 0;
}

std::size_t AVLTreeDictionary::size() const {
    return size_;
}

} // namespace dict


