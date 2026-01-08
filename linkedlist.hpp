#include <string>

struct Node {
    std::string key;
    std::string value;
    Node* next;
    Node(std::string key, std::string value) : key(key), value(value), next(nullptr) {}
};

class LinkedList {
    public:
        LinkedList(std::string key, std::string value) : head(new Node(key, value)) {}

        ~LinkedList() {
            clear();
        }

        void append(std::string key, std::string value) {
            Node* current = head;
            while (current->next) {
                if (current->key == key) {
                    current->value = value;
                    return;
                }
                current = current->next;
            }
            // Check the last node before appending
            if (current->key == key) {
                current->value = value;
                return;
            }
            current->next = new Node(key, value);
        }

        void clear() {
            Node* current = head;
            while (current) {
                Node* nextNode = current->next;
                delete current;
                current = nextNode;
            }
            head = nullptr;
        }
        Node* head;
};