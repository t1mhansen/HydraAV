#pragma once
#include <string>

// Function to scan all files in a directory
int scan_directory(const std::string& dir_path, bool recursive = false, bool include_media = false); 