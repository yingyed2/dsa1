// Ying Ye Deng
// Implmentation of stack & queue data structures via singly linked list,
// commands processed from an input file and written to an output file

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <list>
#include <stdexcept>
#include <utility>

using namespace std;


template <typename T>
class SimpleList {
private:
    struct Node {   // singly linked list node
        T data;
        Node* next;
        explicit Node(const T& d, Node* n = nullptr) : data(d), next(n) {}
        explicit Node(T&& d, Node* n = nullptr) : data(std::move(d)), next(n) {}
    };

    Node* head = nullptr;
    Node* tail = nullptr;
    size_t size = 0;
    string name;

protected:
    void insert_front(const T& value) {
        Node* n = new Node(value, head);
        head = n;
        if (size++ == 0) {
            tail = head;
        }
    }

    void insert_back(const T& value) {
        Node* n = new Node(value, nullptr);
        if (size++ == 0) {
            head = tail = n;
        } else {
            tail->next = n;
            tail = n;
        }
    }

    T remove_front() {
        if (size == 0) {
            throw underflow_error("pop from empty SimpleList");
        }
        Node* n = head;
        head = head->next;
        if (--size == 0) {
            tail = nullptr;
        }
        T value = std::move(n->data);
        delete n;
        return value;
    }

public:
    explicit SimpleList(string n)
        : head(nullptr),
          tail(nullptr),
          size(0),
          name(std::move(n)) {}

    virtual ~SimpleList() { clear(); }

    SimpleList(const SimpleList&) = delete;
    SimpleList& operator=(const SimpleList&) = delete;
    SimpleList(SimpleList&&) = delete;
    SimpleList& operator=(SimpleList&&) = delete;

    const string& getName() const noexcept { return name; }
    bool empty() const noexcept { return size == 0; }
    size_t getSize() const noexcept { return size; }

    void clear() noexcept {
        Node* current = head;
        while (current) {
            Node* next = current->next;
            delete current;
            current = next;
        }
        head = tail = nullptr;
        size = 0;
    }

    virtual void push(const T& value) = 0;
    virtual T pop() = 0;
};


template <typename T>   // stack - push/pop from the front (lifo behavior)
class Stack : public SimpleList<T> {
    using Base = SimpleList<T>;
public:
    explicit Stack(const string& name) : Base(name) {}

    void push(const T& value) override { this->insert_front(value); }
    T pop() override { return this->remove_front(); }
};


template <typename T>   // queue - push to back, pop from front (fifo behavior)
class Queue : public SimpleList<T> {
    using Base = SimpleList<T>;
public:
    explicit Queue(const string& name) : Base(name) {}

    void push(const T& value) override { this->insert_back(value); }
    T pop() override { return this->remove_front(); }
};


template <typename T>   // finding list by name
SimpleList<T>* findByName(const list<SimpleList<T>*>& lists,
                          const string& target) {
    for (auto* p : lists) {
        if (p->getName() == target) {
            return p;
        }
    }
    return nullptr;
}


int main() {

    string inputFileName;
    string outputFileName;

    cout << "Enter name of input file: ";
    cin >> inputFileName;
    cout << "Enter name of output file: ";
    cin >> outputFileName;

    ifstream in(inputFileName);
    ofstream out(outputFileName);

    if (!in) {
        cerr << "Error: could not open input file.\n";
        return 1;
    }
    if (!out) {
        cerr << "Error: could not open output file.\n";
        return 1;
    }

    // initializing containers for each type
    
    list<SimpleList<int>*> listSLi;      // container for int structures
    list<SimpleList<double>*> listSLd;   // container for double structures
    list<SimpleList<string>*> listSLs;   // container for string structures
    

    string line;
    while (getline(in, line)) {     // processing commands from input file
        if (line.empty()) {
            continue;
        }

        out << "PROCESSING COMMAND: " << line << '\n';

        istringstream iss(line);
        string command;
        string listName;

        iss >> command >> listName;

        // determine data type from first character of list name
        char typeChar = listName[0]; // 'i' = int, 'd' = double, 's' = string

        if (command == "create") {  // create a new stack or queue
            string structureType;
            iss >> structureType;

            if (typeChar == 'i') {
                SimpleList<int>* existing = findByName(listSLi, listName);
                if (existing) {
                    out << "ERROR: This name already exists!" << '\n';
                } else {
                    SimpleList<int>* p = nullptr;
                    if (structureType == "stack") {
                        p = new Stack<int>(listName);
                    } else {
                        p = new Queue<int>(listName);
                    }
                    listSLi.push_front(p);
                }
            } else if (typeChar == 'd') {
                SimpleList<double>* existing = findByName(listSLd, listName);
                if (existing) {
                    out << "ERROR: This name already exists!" << '\n';
                } else {
                    SimpleList<double>* p = nullptr;
                    if (structureType == "stack") {
                        p = new Stack<double>(listName);
                    } else {
                        p = new Queue<double>(listName);
                    }
                    listSLd.push_front(p);
                }
            } else if (typeChar == 's') {
                SimpleList<string>* existing = findByName(listSLs, listName);
                if (existing) {
                    out << "ERROR: This name already exists!" << '\n';
                } else {
                    SimpleList<string>* p = nullptr;
                    if (structureType == "stack") {
                        p = new Stack<string>(listName);
                    } else {
                        p = new Queue<string>(listName);
                    }
                    listSLs.push_front(p);
                }
            }
        
        } else if (command == "push") {  // add an element to a structure
            string valueStr;
            iss >> valueStr;

            if (typeChar == 'i') {
                SimpleList<int>* p = findByName(listSLi, listName);
                if (!p) {
                    out << "ERROR: This name does not exist!" << '\n';
                } else {
                    int value = 0;
                    istringstream conv(valueStr);
                    conv >> value;
                    p->push(value);
                }
            } else if (typeChar == 'd') {
                SimpleList<double>* p = findByName(listSLd, listName);
                if (!p) {
                    out << "ERROR: This name does not exist!" << '\n';
                } else {
                    double value = 0.0;
                    istringstream conv(valueStr);
                    conv >> value;
                    p->push(value);
                }
            } else if (typeChar == 's') {
                SimpleList<string>* p = findByName(listSLs, listName);
                if (!p) {
                    out << "ERROR: This name does not exist!" << '\n';
                } else {
                    p->push(valueStr);
                }
            }
        
        } else if (command == "pop") {  // remove and return an element from a structure
            if (typeChar == 'i') {
                SimpleList<int>* p = findByName(listSLi, listName);
                if (!p) {
                    out << "ERROR: This name does not exist!" << '\n';
                } else if (p->empty()) {
                    out << "ERROR: This list is empty!" << '\n';
                } else {
                    int value = p->pop();
                    out << "Value popped: " << value << '\n';
                }
            } else if (typeChar == 'd') {
                SimpleList<double>* p = findByName(listSLd, listName);
                if (!p) {
                    out << "ERROR: This name does not exist!" << '\n';
                } else if (p->empty()) {
                    out << "ERROR: This list is empty!" << '\n';
                } else {
                    double value = p->pop();
                    out << "Value popped: " << value << '\n';
                }
            } else if (typeChar == 's') {
                SimpleList<string>* p = findByName(listSLs, listName);
                if (!p) {
                    out << "ERROR: This name does not exist!" << '\n';
                } else if (p->empty()) {
                    out << "ERROR: This list is empty!" << '\n';
                } else {
                    string value = p->pop();
                    out << "Value popped: " << value << '\n';
                }
            }
        }
    }

    auto cleanup = [](auto& lst) {  // deallocating all dynamically allocated memory in the heap
        for (auto* p : lst) delete p;
    };
    
    cleanup(listSLi);
    cleanup(listSLd);
    cleanup(listSLs);

    return 0;
}