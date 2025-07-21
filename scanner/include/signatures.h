#pragma once
#include <vector>
#include <string>

// Signature struct for malware patterns
struct Signature {
    std::string name;
    std::vector<unsigned char> byte_pattern;
    std::string string_pattern;
    bool is_string_pattern = false;
};

// Function to get signatures
const std::vector<Signature>& get_signatures(); 