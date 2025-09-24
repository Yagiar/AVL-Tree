#include <cassert>
#include <string>

#include "dict/AVLTreeDictionary.hpp"

int main() {
    dict::AVLTreeDictionary d;
    // Smoke: пока все методы-заглушки возвращают значения по умолчанию
    d.clear();
    assert(d.size() == 0);
    (void)d;
    return 0;
}


