#pragma once
#include <string>
#include <vector>
#include "signatures.h"

// Scan status enum
enum class ScanStatus {
    CLEAN,
    SUSPICIOUS,
    MALWARE,
    ERROR
};

// Scan result struct
struct ScanResult {
    ScanStatus status;
    std::string signature_name;
    size_t offset;
    std::string description;
    double entropy = 0.0;
    bool high_entropy_suspicious = false;
};

// Function to scan a file
int scan_file(const std::string& file_path, ScanResult& result);

// Function to verify a file exists and is valid
bool verify_file(const std::string& file_path);

// Function to check if a file is a media file (should be skipped)
bool is_media_file(const std::string& file_path);

// Function to scan all files in a directory
int scan_directory(const std::string& dir_path, bool recursive = false, bool include_media = false);

// Function to calculate entropy of a file
double calculate_entropy(const std::vector<unsigned char>& buffer);

