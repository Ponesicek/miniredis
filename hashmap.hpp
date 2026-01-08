#include <string>
#include "linkedlist.hpp"
#define HASHMAP_SIZE 256

class hashmap
{
private:
    LinkedList* list[HASHMAP_SIZE];
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
            if (current->key == key)
            {
                return current->value;
            }
            current = current->next;
        }
        return "";
    }
};

hashmap::hashmap()
{
    for (int i = 0; i < HASHMAP_SIZE; i++)
    {
        list[i] = nullptr;
    }
}

hashmap::~hashmap()
{
}
