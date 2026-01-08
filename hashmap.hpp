#include <string>
#include "linkedlist.hpp"
#include <iostream>
#define HASHMAP_SIZE 2

class hashmap
{
private:
    LinkedList** list;
    int size = HASHMAP_SIZE;
    int ammountOfObjects = 0;

public:
    hashmap();
    ~hashmap();
    void insert(std::string key, std::string value)
    {
        int index = std::hash<std::string>{}(key) % size;
        if (list[index] == nullptr)
        {
            list[index] = new LinkedList(key, value);
        }
        else
        {
            list[index]->append(key, value);
        }
        ammountOfObjects++;
        if (ammountOfObjects / size > 2)
        {
            doubleSize();
        }
    }
    std::string get(std::string key)
    {
        int index = std::hash<std::string>{}(key) % size;
        if (list[index] == nullptr)
        {
            return "";
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
        return "";
    }
    void doubleSize()
    {
        LinkedList** oldList = list;
        int oldSize = size;
        size *= 2;
        list = new LinkedList * [size];
        for (int i = 0; i < size; i++)
        {
            list[i] = nullptr;
        }
        for (int i = 0; i < oldSize; i++)
        {
            if (oldList[i] != nullptr)
            {
                Node* current = oldList[i]->head;
                while (current)
                {
                    insert(current->key, current->value);
                    current = current->next;
                }
                delete oldList[i];
            }
        }
        delete[] oldList;
        std::cout << "Hashmap size doubled to " << size << std::endl;
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
    for (int i = 0; i < size; i++)
    {
        delete list[i];
    }
    delete[] list;
}
