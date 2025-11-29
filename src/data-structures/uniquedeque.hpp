#pragma once

#include <algorithm>
#include <deque>
#include <unordered_set>

template <typename T>
class UniqueDeque {
private:
    std::deque<T> m_deque;
    std::unordered_set<T> m_set;

public:
    bool push_back(const T &value);
    bool push_front(const T &value);

    T &operator[](size_t index);
    const T &operator[](size_t index) const;

    size_t size() const;

    bool empty() const;

    auto begin();
    auto end();
    auto cbegin() const;
    auto cend() const;

    bool erase(const T &value);

    void clear();

    template <typename Predicate>
    void erase_if(Predicate pred);
};

template <typename T>
bool UniqueDeque<T>::push_back(const T &value) {
    if (m_set.insert(value).second) {
        m_deque.push_back(value);
        return true;
    }
    return false;
}

template <typename T>
bool UniqueDeque<T>::push_front(const T &value) {
    if (m_set.insert(value).second) {
        m_deque.push_front(value);
        return true;
    }
    return false;
}

template <typename T>
T &UniqueDeque<T>::operator[](size_t index) {
    return m_deque[index];
}

template <typename T>
const T &UniqueDeque<T>::operator[](size_t index) const {
    return m_deque[index];
}

template <typename T>
size_t UniqueDeque<T>::size() const {
    return m_deque.size();
}

template <typename T>
bool UniqueDeque<T>::empty() const {
    return m_deque.empty();
}

template <typename T>
auto UniqueDeque<T>::begin() {
    return m_deque.begin();
}

template <typename T>
auto UniqueDeque<T>::end() {
    return m_deque.end();
}

template <typename T>
auto UniqueDeque<T>::cbegin() const {
    return m_deque.cbegin();
}

template <typename T>
auto UniqueDeque<T>::cend() const {
    return m_deque.cend();
}

template <typename T>
bool UniqueDeque<T>::erase(const T &value) {
    if (m_set.erase(value)) {
        auto it = std::find(m_deque.begin(), m_deque.end(), value);
        if (it != m_deque.end()) {
            m_deque.erase(it);
            return true;
        }
    }
    return false;
}

template <typename T>
void UniqueDeque<T>::clear() {
    m_deque.clear();
    m_set.clear();
}

template <typename T>
template <typename Predicate>
void UniqueDeque<T>::erase_if(Predicate pred) {
    for (auto it = m_deque.begin(); it != m_deque.end();) {
        if (pred(*it)) {
            m_set.erase(*it);
            it = m_deque.erase(it);
        } else {
            ++it;
        }
    }
}
