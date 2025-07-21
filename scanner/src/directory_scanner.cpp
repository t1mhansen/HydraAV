#include "scanner.h"
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

int scan_directory(const std::string& dir_path, bool recursive, bool include_media) {
    fs::path dir(dir_path);
    
    if (!fs::exists(dir)) {
        std::cerr << "Directory does not exist: " << dir_path << std::endl;
        return -1;
    }
    
    if (!fs::is_directory(dir)) {
        std::cerr << "Path is not a directory: " << dir_path << std::endl;
        return -1;
    }
    
    std::cout << "Scanning directory: " << dir_path;
    if (recursive) {
        std::cout << " (recursive)";
    }
    if (include_media) {
        std::cout << " (including media files)";
    }
    std::cout << std::endl;
    
    int total_files = 0;
    int clean_files = 0;
    int malware_files = 0;
    int suspicious_files = 0;
    int error_files = 0;
    int skipped_files = 0;
    
    // Choose iterator type based on recursive flag
    if (recursive) {
        // Recursive scan - includes all subdirectories
        for (const auto& entry : fs::recursive_directory_iterator(dir)) {
            if (entry.is_regular_file()) {
                std::string file_path = entry.path().string();
                std::string relative_path = fs::relative(entry.path(), dir).string();
                
                // Skip media files unless include_media is true
                if (!include_media && is_media_file(file_path)) {
                    std::cout << "Skipping media file: " << relative_path << std::endl;
                    skipped_files++;
                    continue;
                }
                
                total_files++;
                std::cout << "[" << total_files << "] Scanning: " << relative_path << " - ";
                
                ScanResult result;
                int scan_result = scan_file(file_path, result);
                
                if (scan_result == 0) {
                    std::cout << "CLEAN";
                    clean_files++;
                } else if (scan_result == 1) {
                    std::cout << "MALWARE DETECTED (" << result.signature_name << ")";
                    malware_files++;
                } else if (scan_result == 2) {
                    std::cout << "SUSPICIOUS (High entropy)";
                    suspicious_files++;
                } else {
                    std::cout << "ERROR";
                    error_files++;
                }
                std::cout << std::endl;
            }
        }
    } else {
        // Non-recursive scan - only files in the specified directory
        for (const auto& entry : fs::directory_iterator(dir)) {
            if (entry.is_regular_file()) {
                std::string file_path = entry.path().string();
                
                // Skip media files unless include_media is true
                if (!include_media && is_media_file(file_path)) {
                    std::cout << "Skipping media file: " << entry.path().filename().string() << std::endl;
                    skipped_files++;
                    continue;
                }
                
                total_files++;
                std::cout << "[" << total_files << "] Scanning: " << entry.path().filename().string() << " - ";
                
                ScanResult result;
                int scan_result = scan_file(file_path, result);
                
                if (scan_result == 0) {
                    std::cout << "CLEAN";
                    clean_files++;
                } else if (scan_result == 1) {
                    std::cout << "MALWARE DETECTED (" << result.signature_name << ")";
                    malware_files++;
                } else if (scan_result == 2) {
                    std::cout << "SUSPICIOUS (High entropy)";
                    suspicious_files++;
                } else {
                    std::cout << "ERROR";
                    error_files++;
                }
                std::cout << std::endl;
            }
        }
    }
    
    // Print summary
    std::cout << "\nScan Summary:" << std::endl;
    std::cout << "- Total files scanned: " << total_files << std::endl;
    std::cout << "- Files skipped: " << skipped_files << std::endl;
    std::cout << "- Clean: " << clean_files << std::endl;
    std::cout << "- Malware detected: " << malware_files << std::endl;
    std::cout << "- Suspicious: " << suspicious_files << std::endl;
    std::cout << "- Errors: " << error_files << std::endl;
    
    return (malware_files > 0 || suspicious_files > 0) ? 1 : 0; // Return 1 if any malware or suspicious found, 0 if clean
} 