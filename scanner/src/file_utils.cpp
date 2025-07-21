#include "scanner.h"
#include <iostream>
#include <filesystem>
#include <algorithm>

namespace fs = std::filesystem;

bool verify_file(const std::string& file_path) {
    fs::path path(file_path);
    
    if (!fs::exists(path)) {
        std::cerr << "File does not exist: " << file_path << std::endl;
        return false;
    }
    
    if (!fs::is_regular_file(path)) {
        std::cerr << "Path is not a regular file: " << file_path << std::endl;
        return false;
    }
    
    std::cout << "File verified: " << file_path << std::endl;
    return true;
}

bool is_media_file(const std::string& file_path) {
    fs::path path(file_path);
    std::string extension = path.extension().string();
    
    // Convert to lowercase for case-insensitive comparison
    std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
    
    // Common media file extensions
    std::vector<std::string> media_extensions = {
        // Images
        ".jpg", ".jpeg", ".png", ".gif", ".bmp", ".tiff", ".tif", ".webp", ".svg", ".ico",
        // Videos
        ".mp4", ".avi", ".mov", ".wmv", ".flv", ".webm", ".mkv", ".m4v", ".3gp", ".mpg", ".mpeg",
        // Audio
        ".mp3", ".wav", ".flac", ".aac", ".ogg", ".wma", ".m4a",
        // Archives 
        ".zip", ".rar", ".7z", ".tar", ".gz", ".bz2"
    };
    
    return std::find(media_extensions.begin(), media_extensions.end(), extension) != media_extensions.end();
} 