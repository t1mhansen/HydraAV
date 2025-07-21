#include "scanner.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <cmath>

double calculate_entropy(const std::vector<unsigned char>& buffer) {
    if (buffer.empty()) return 0.0;
    std::vector<size_t> freq(256,0);
    for (unsigned char byte : buffer) {
        freq[byte]++;
    }
    double entropy = 0.0;
    for (size_t count : freq) {
        if (count == 0) continue;
        double p = static_cast<double>(count) / buffer.size();
        entropy -= p * std::log2(p);
    }
    return entropy;
}

int scan_file(const std::string& file_path, ScanResult& result) {
    std::ifstream file(file_path, std::ios::binary);
    if (!file) {
        result.status = ScanStatus::ERROR;
        result.description = "Failed to open file";
        return -1;
    }
    std::vector<unsigned char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    double entropy = calculate_entropy(buffer);
    result.entropy = entropy;
    result.high_entropy_suspicious = (entropy > 7.5);
    std::cout << "Entropy: " << entropy << std::endl;

    // Get signatures from signatures.cpp
    auto signatures = get_signatures();

    for (const auto& signature : signatures) {
        bool found = false;

        if (signature.is_string_pattern) {
            std::string file_content(buffer.begin(), buffer.end());
            found = (file_content.find(signature.string_pattern) != std::string::npos);
        } else {
            auto it = std::search(buffer.begin(), buffer.end(), signature.byte_pattern.begin(), signature.byte_pattern.end());
            found = (it != buffer.end());
        }

        if (found) {
            result.status = ScanStatus::MALWARE;
            result.signature_name = signature.name;
            result.offset = 0; // For string patterns -- don't track offset yet
            result.description = "Signature match detected: " + signature.name;
            return 1;
        }
    }
    if (result.high_entropy_suspicious) {
        result.status = ScanStatus::SUSPICIOUS;
        result.description = "High entropy: possible packed or encrypted file";
        return 2;
    }
    result.status = ScanStatus::CLEAN;
    result.description = "No known signatures found";
    return 0;
} 