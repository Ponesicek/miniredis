#include "hashmap.hpp"
#include <iostream>

int main() {
    hashmap map;
    map.insert("name", "Alice");
    map.insert("age", "30");
    std::cout << map.get("name") << std::endl;
    return 0;
}