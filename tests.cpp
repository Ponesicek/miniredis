#include "hashmap.hpp"
#include <iostream>

int main() {
    hashmap map;
    map.insert("name", "Alice");
    map.insert("age", "30");
    map.insert("sex", "female");
    map.insert("husband", "Peter");
    map.insert("weight", "70kg");
    map.insert("pay", "100k");
    map.insert("height", "160cm");
    std::cout << map.get("sex") << std::endl;
    return 0;
}