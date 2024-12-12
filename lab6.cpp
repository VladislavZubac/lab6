#include <iostream>
#include <cassert>
#include <algorithm>
#include <sstream>

template <typename T>
class Vector {
private:
    T *arr;
    size_t capacity;
    size_t len;

public:
    Vector(size_t size = 0, T val = T())
        : capacity(size), len(size) {
        arr = new T[capacity];
        std::fill(arr, arr + size, val);
    }

    Vector(const Vector &other)
        : capacity(other.capacity), len(other.len) {
        arr = new T[capacity];
        std::copy(other.arr, other.arr + len, arr);
    }

    Vector(Vector &&other) noexcept
        : arr(other.arr), capacity(other.capacity), len(other.len) {
        other.arr = nullptr;
        other.capacity = 0;
        other.len = 0;
    }

    Vector(std::initializer_list<T> init) {
        len = init.size();
        capacity = len;
        arr = new T[capacity];
        std::copy(init.begin(), init.end(), arr);
    }

    template <typename InputIterator>
    Vector(InputIterator first, InputIterator last) {
        len = std::distance(first, last);
        capacity = len;
        arr = new T[capacity];
        std::copy(first, last, arr);
    }

    ~Vector() {
        clear();
    }

    Vector &operator=(const Vector &other) {
        if (this != &other) {
            delete[] arr;
            capacity = other.capacity;
            len = other.len;

            arr = new T[capacity];
            std::copy(other.arr, other.arr + len, arr);
        }
        return *this;
    }

    Vector &operator=(Vector &&other) noexcept {
        if (this != &other) {
            delete[] arr;
            capacity = other.capacity;
            len = other.len;
            arr = other.arr;

            other.capacity = 0;
            other.len = 0;
            other.arr = nullptr;
        }
        return *this;
    }

    friend bool operator==(const Vector &lhs, const Vector &rhs) {
        if (lhs.size() != rhs.size()) return false;
        for (size_t i = 0; i < lhs.size(); ++i) {
            if (lhs.arr[i] != rhs.arr[i]) return false;
        }
        return true;
    }

    friend bool operator!=(const Vector &lhs, const Vector &rhs) {
        return !(lhs == rhs);
    }

    size_t size() const {
        return len;
    }

    bool empty() const {
        return len == 0;
    }

    void resize(size_t new_size) {
        T *new_arr = new T[new_size];
        size_t min_size = std::min(len, new_size);
        std::copy(arr, arr + min_size, new_arr);
        delete[] arr;
        arr = new_arr;
        capacity = new_size;
        if (len > new_size) {
            len = new_size;
        }
    }

    T &operator[](size_t index) {
        return arr[index];
    }

    const T &operator[](size_t index) const {
        return arr[index];
    }

    void push_back(const T &item) {
        if (len == capacity) {
            resize(capacity == 0 ? 1 : capacity * 2);
        }
        arr[len++] = item;
    }

    void pop_front() {
        if (len > 0) {
            std::copy(arr + 1, arr + len, arr);
            len--;
        }
    }

    T &front_element() const {
        return arr[0];
    }

    T &back_element() const {
        return arr[len - 1];
    }

    void clear() {
        delete[] arr;
        arr = nullptr;
        len = 0;
        capacity = 0;
    }

    void assign(size_t n, const T &value) {
        resize(n);
        std::fill(arr, arr + n, value);
    }

    friend std::istream &operator>>(std::istream &is, Vector &vec) {
        for (size_t i = 0; i < vec.size(); ++i) {
            is >> vec[i];
        }
        return is;
    }

    friend std::ostream &operator<<(std::ostream &os, const Vector &vec) {
        for (size_t i = 0; i < vec.size(); ++i) {
            os << vec[i] << " ";
        }
        return os;
    }

    class Iterator {
    private:
        T *ptr;

    public:
        using iterator_category = std::random_access_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T *;
        using reference = T &;

        Iterator(T *p) : ptr(p) {}

        T &operator*() const { return *ptr; }
        Iterator &operator++() {
            ++ptr;
            return *this;
        }

        Iterator operator++(int) {
            Iterator temp = *this;
            ++ptr;
            return temp;
        }

        Iterator &operator--() {
            --ptr;
            return *this;
        }

        Iterator operator--(int) {
            Iterator temp = *this;
            --ptr;
            return temp;
        }

        Iterator &operator+=(difference_type n) {
            ptr += n;
            return *this;
        }

        Iterator &operator-=(difference_type n) {
            ptr -= n;
            return *this;
        }

        Iterator operator+(difference_type n) const { return Iterator(ptr + n); }
        Iterator operator-(difference_type n) const { return Iterator(ptr - n); }
        difference_type operator-(const Iterator &other) const { return ptr - other.ptr; }

        bool operator==(const Iterator &other) const { return ptr == other.ptr; }
        bool operator!=(const Iterator &other) const { return ptr != other.ptr; }
        bool operator<(const Iterator &other) const { return ptr < other.ptr; }
        bool operator>(const Iterator &other) const { return ptr > other.ptr; }
        bool operator<=(const Iterator &other) const { return ptr <= other.ptr; }
        bool operator>=(const Iterator &other) const { return ptr >= other.ptr; }
    };

    Iterator begin() { return Iterator(arr); }
    Iterator begin() const { return Iterator(arr); }
    const Iterator cbegin() const noexcept { return Iterator(arr); }
    Iterator end() { return Iterator(arr + len); }
    Iterator end() const { return Iterator(arr + len); }
    const Iterator cend() const noexcept { return Iterator(arr + len); }

    using ReverseIterator = std::reverse_iterator<Iterator>;

    auto rbegin() { return ReverseIterator(end()); }
    auto rbegin() const { return ReverseIterator(end()); }
    auto rend() { return ReverseIterator(begin()); }
    auto rend() const { return ReverseIterator(begin()); }
    const auto crbegin() const noexcept { return ReverseIterator(cend()); }
    const auto crend() const noexcept { return ReverseIterator(cbegin()); }
};

template <typename T>
class PriorityQueue {
private:
    Vector<T> data;

public:
    ~PriorityQueue() = default;
    PriorityQueue() = default;
    PriorityQueue(const PriorityQueue &other) = default;
    PriorityQueue(PriorityQueue &&other) noexcept = default;
    PriorityQueue &operator=(const PriorityQueue &other) = default;
    PriorityQueue &operator=(PriorityQueue &&other) noexcept = default;

    PriorityQueue(const Vector<T> &cont) : data(cont) {
        std::sort(data.begin(), data.end(), std::greater<T>());
    }
    PriorityQueue(const T *first, const T *last) : data(first, last) {
        std::sort(data.begin(), data.end(), std::greater<T>());
    }

    size_t size() const {
        return data.size();
    }

    bool empty() const {
        return data.empty();
    }

    void swap(PriorityQueue &other) {
        std::swap(data, other.data);
    }

    void push(const T &value) {
        data.push_back(value);
        std::sort(data.begin(), data.end(), std::greater<T>());
    }

    void pop() {
        data.pop_front();
    }

    T &front() {
        return data.front_element();
    }

    T &back() {
        return data.back_element();
    }

    friend bool operator==(const PriorityQueue &lhs, const PriorityQueue &rhs) {
        return lhs.data == rhs.data;
    }

    friend bool operator!=(const PriorityQueue &lhs, const PriorityQueue &rhs) {
        return !(lhs == rhs);
    }

    friend bool operator<(const PriorityQueue &lhs, const PriorityQueue &rhs) {
        return lhs.data < rhs.data;
    }

    friend bool operator>(const PriorityQueue &lhs, const PriorityQueue &rhs) {
        return rhs < lhs;
    }

    friend bool operator<=(const PriorityQueue &lhs, const PriorityQueue &rhs) {
        return !(rhs < lhs);
    }

    friend bool operator>=(const PriorityQueue &lhs, const PriorityQueue &rhs) {
        return !(lhs < rhs);
    }

    friend std::istream &operator>>(std::istream &is, PriorityQueue &q) {
        is >> q.data;
        std::sort(q.data.begin(), q.data.end(), std::greater<T>());
        return is;
    }

    friend std::ostream &operator<<(std::ostream &os, const PriorityQueue &q) {
        os << q.data;
        return os;
    }
};

// Test code
int main() {
    Vector<int> a({2, 3, 5, 7});
    a.push_back(11);
    assert(11 == a[a.size() - 1]);
    a.assign(1, 1);
    assert(1 == a[0]);

    a.pop_front();

    Vector<char> b = {'a', 'b', 'r', 'a', 0};
    assert(0 == b.back_element());
    b.pop_front();
    b.push_back('c');

    b.push_back('a');

    b[0] = 'A';
    assert('A' == b.front_element());
    Vector<std::string> c({"Hello", "world"});
    c[0].append(",");
    c.push_back(" ");
    c[c.size() - 1].append("!");

    PriorityQueue<int> q;
    q.push(1);
    q.push(2);
    q.push(3);
    for (; q.size(); q.pop())
        std::cout << q.front() << " ";
    return 0;
}
