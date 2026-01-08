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
    map.insert("city", "New York");
    map.insert("country", "USA");
    map.insert("job", "Engineer");
    map.insert("hobby", "Reading");
    map.insert("email", "alice@example.com");
    map.insert("phone", "555-1234");
    map.insert("education", "Masters");
    map.insert("experience", "5 years");
    map.insert("department", "IT");
    map.insert("status", "Active");
    std::cout << map.get("sex") << std::endl;
    return 0;
}