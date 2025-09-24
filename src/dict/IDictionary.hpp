#pragma once

#include <optional>
#include <string>

namespace dict {

// Базовый интерфейс словаря: ключ — целое число, значение — строка
class IDictionary {
public:
    virtual ~IDictionary() = default;

    // Вставка: если ключ отсутствует — добавить, если присутствует — вернуть false
    virtual bool insert(int key, const std::string &value) = 0;

    // Добавление/обновление: установить значение по ключу (создать или обновить)
    virtual void upsert(int key, const std::string &value) = 0;

    // Удаление по ключу; вернуть true, если элемент был удалён
    virtual bool erase(int key) = 0;

    // Поиск значения; std::nullopt, если ключ отсутствует
    virtual std::optional<std::string> find(int key) const = 0;

    // Очистка структуры
    virtual void clear() = 0;

    // Количество элементов
    virtual std::size_t size() const = 0;
};

} // namespace dict


