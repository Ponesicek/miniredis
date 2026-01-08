#include "hashmap.hpp"
#include <iostream>
#include <unordered_map>

int main() {
    hashmap map;
    //std::unordered_map<std::string, std::string> map;
    map.insert("name", "Alice");
    map.insert("age", "30");
    map.insert("Mother", "Eva");
    map.insert("Father", "John");
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
    std::cout << "[find.first]: " << map.find("status")->first << ", Should be: status"<< std::endl;
    std::cout << "[find.second]: " << map.find("status")->second << ", Should be: Active"<< std::endl;
    map.erase("age");
    std::cout << "[erase (age)]: " << map.get("age") << ", Should be: " << std::endl;
    return 1;
}