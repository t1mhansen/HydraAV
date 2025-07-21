#pragma once
#include <string>

// Function to verify a file exists and is valid
bool verify_file(const std::string& file_path);

// Function to check if a file is a media file (should be skipped)
bool is_media_file(const std::string& file_path); 