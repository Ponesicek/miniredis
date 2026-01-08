#include <string>
#include "linkedlist.hpp"
#include <iostream>
#define HASHMAP_SIZE 128
#define LOAD_FACTOR 1

class hashmap
{
private:
    LinkedList** list;
    int count = HASHMAP_SIZE;
    int ammountOfObjects = 0;

public:
    class Iterator {
    private:
        LinkedList** list;
        int count;
        int currentBucket;
        Node* currentNode;

        void findNextValid() {
            while (currentBucket < count) {
                if (list[currentBucket] != nullptr) {
                    currentNode = list[currentBucket]->head;
                    return;
                }
                currentBucket++;
            }
            currentNode = nullptr;
        }

    public:
        Iterator(LinkedList** l, int c, int bucket, Node* node) 
            : list(l), count(c), currentBucket(bucket), currentNode(node) {
            if (currentNode == nullptr && currentBucket < count) {
                findNextValid();
            }
        }

        std::pair<std::string, std::string> operator*() const {
            return std::make_pair(currentNode->key, currentNode->value);
        }

        // Helper class for operator->
        struct ArrowProxy {
            std::pair<std::string, std::string> pair;
            std::pair<std::string, std::string>* operator->() {
                return &pair;
            }
        };

        ArrowProxy operator->() const {
            return ArrowProxy{std::make_pair(currentNode->key, currentNode->value)};
        }

        Iterator& operator++() {
            if (currentNode && currentNode->next) {
                currentNode = currentNode->next;
            } else {
                currentBucket++;
                findNextValid();
            }
            return *this;
        }

        bool operator!=(const Iterator& other) const {
            return currentNode != other.currentNode || currentBucket != other.currentBucket;
        }

        bool operator==(const Iterator& other) const {
            return !(*this != other);
        }
    };

    hashmap();
    ~hashmap();
    void insert(std::string key, std::string value)
    {
        int index = std::hash<std::string>{}(key) % count;
        if (list[index] == nullptr)
        {
            list[index] = new LinkedList(key, value);
        }
        else
        {
            list[index]->append(key, value);
        }
        ammountOfObjects++;
        if (ammountOfObjects / count > LOAD_FACTOR)
        {
            doubleSize();
        }
    }
    int size()
    {
        return ammountOfObjects;
    }
    std::string& get(std::string key)
    {
        int index = std::hash<std::string>{}(key) % count;
        if (list[index] == nullptr)
        {
            return *(new std::string(""));
        }
        Node* current = list[index]->head;
        while (current)
        {
            std::cout << "Checking key: " << current->key << std::endl;
            if (current->key == key)
            {
                return current->value;
            }
            current = current->next;
        }
        return *(new std::string(""));
    }
    Iterator begin()
    {
        return Iterator(list, count, 0, nullptr);
    }
    Iterator end()
    {
        return Iterator(list, count, count, nullptr);
    }
    std::string& operator[](std::string index) {
        return get(index);
    }
    bool erase(std::string key)
    {
        int index = std::hash<std::string>{}(key) % count;
        if (list[index] == nullptr)
        {
            return false;
        }
        Node* current = list[index]->head;
        Node* prev = nullptr;
        while (current)
        {
            if (current->key == key)
            {
                if (prev == nullptr)
                {
                    list[index]->head = current->next;
                    delete current;
                    // If the list is now empty, delete the LinkedList itself
                    if (list[index]->head == nullptr)
                    {
                        delete list[index];
                        list[index] = nullptr;
                    }
                }
                else
                {
                    prev->next = current->next;
                    delete current;
                }
                ammountOfObjects--;
                return true;
            }
            prev = current;
            current = current->next;
        }
        return false;
    }
    Iterator find(std::string key)
    {
        int index = std::hash<std::string>{}(key) % count;
        if (list[index] == nullptr)
        {
            return end();
        }
        Node* current = list[index]->head;
        while (current)
        {
            if (current->key == key)
            {
                return Iterator(list, count, index, current);
            }
            current = current->next;
        }
        return end();
    }
    void doubleSize()
    {
        LinkedList** oldList = list;
        int oldSize = count;
        count *= 2;
        list = new LinkedList * [count];
        for (int i = 0; i < count; i++)
        {
            list[i] = nullptr;
        }
        ammountOfObjects = 0;
        for (int i = 0; i < oldSize; i++)
        {
            if (oldList[i] != nullptr)
            {
                Node* current = oldList[i]->head;
                while (current)
                {
                    int index = std::hash<std::string>{}(current->key) % count;
                    if (list[index] == nullptr)
                    {
                        list[index] = new LinkedList(current->key, current->value);
                    }
                    else
                    {
                        list[index]->append(current->key, current->value);
                    }
                    ammountOfObjects++;
                    current = current->next;
                }
                delete oldList[i];
            }
        }
        delete[] oldList;
    }
};

hashmap::hashmap()
{
    list = new LinkedList*[HASHMAP_SIZE];
    for (int i = 0; i < HASHMAP_SIZE; i++)
    {
        list[i] = nullptr;
    }
}

hashmap::~hashmap()
{
    for (int i = 0; i < count; i++)
    {
        delete list[i];
    }
    delete[] list;
}
