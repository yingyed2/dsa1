#ifndef SIMPLELIST_HPP
#define SIMPLELIST_HPP

#include <string>
#include <list>
#include <stdexcept>
#include <utility>

template <typename T>
class SimpleList {
protected:
    struct Node {
        T data;
        Node* next;
        explicit Node(const T& d, Node* n = nullptr) : data(d), next(n) {}
        explicit Node(T&& d, Node* n = nullptr) : data(std::move(d)), next(n) {}
    };

    Node* head_ = nullptr;
    Node* tail_ = nullptr;
    std::size_t size_ = 0;
    std::string name_;

    void insert_front(const T& value) {
        Node* n = new Node(value, head_);
        head_ = n;
        if (size_++ == 0) tail_ = head_;
    }
    void insert_front(T&& value) {
        Node* n = new Node(std::move(value), head_);
        head_ = n;
        if (size_++ == 0) tail_ = head_;
    }

    void insert_back(const T& value) {
        Node* n = new Node(value, nullptr);
        if (size_++ == 0) {
            head_ = tail_ = n;
        } else {
            tail_->next = n;
            tail_ = n;
        }
    }
    void insert_back(T&& value) {
        Node* n = new Node(std::move(value), nullptr);
        if (size_++ == 0) {
            head_ = tail_ = n;
        } else {
            tail_->next = n;
            tail_ = n;
        }
    }

    T remove_front() {
        if (size_ == 0) {
            throw std::underflow_error("pop from empty SimpleList");
        }
        Node* n = head_;
        head_ = head_->next;
        if (--size_ == 0) tail_ = nullptr;
        T value = std::move(n->data);
        delete n;
        return value;
    }

public:
    explicit SimpleList(std::string name)
        : name_(std::move(name)) {}

    virtual ~SimpleList() { clear(); }

    SimpleList(const SimpleList&) = delete;
    SimpleList& operator=(const SimpleList&) = delete;
    SimpleList(SimpleList&&) = delete;
    SimpleList& operator=(SimpleList&&) = delete;

    const std::string& getName() const noexcept { return name_; }
    bool empty() const noexcept { return size_ == 0; }
    std::size_t size() const noexcept { return size_; }

    void clear() noexcept {
        Node* cur = head_;
        while (cur) {
            Node* next = cur->next;
            delete cur;
            cur = next;
        }
        head_ = tail_ = nullptr;
        size_ = 0;
    }

    virtual void push(const T& value) = 0;
    virtual T pop() = 0;
};

template <typename T>
class Stack : public SimpleList<T> {
    using Base = SimpleList<T>;
public:
    explicit Stack(const std::string& name) : Base(name) {}

    void push(const T& value) override { this->insert_front(value); }
    T pop() override { return this->remove_front(); }
};

template <typename T>
class Queue : public SimpleList<T> {
    using Base = SimpleList<T>;
public:
    explicit Queue(const std::string& name) : Base(name) {}

    void push(const T& value) override { this->insert_back(value); }
    T pop() override { return this->remove_front(); }
};

template <typename T>
SimpleList<T>* findByName(const std::list<SimpleList<T>*>& lists,
                          const std::string& target) {
    for (auto* p : lists) {
        if (p->getName() == target) return p;
    }
    return nullptr;
}

#endif
