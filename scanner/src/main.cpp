#include "scanner.h"
#include <iostream>
#include <filesystem>
#include <cstdlib>

namespace fs = std::filesystem;

int call_sandbox(const std::string& file_path) {
    std::cout << "Calling Docker sandbox for dynamic analysis..." << std::endl;
    
    // Check if Docker is available
    if (system("docker --version > /dev/null 2>&1") != 0) {
        std::cout << "Warning: Docker not found. Skipping sandbox analysis." << std::endl;
        return 0;
    }
    
    // Check if sandbox image exists
    if (system("docker image inspect hydraav-sandbox > /dev/null 2>&1") != 0) {
        std::cout << "Warning: hydraav-sandbox Docker image not found." << std::endl;
        std::cout << "Run 'cd sandbox && docker build -t hydraav-sandbox .' to build it." << std::endl;
        return 0;
    }
    
    // Create a temporary directory for the file
    std::string temp_dir = "/tmp/hydraav_" + std::to_string(time(nullptr));
    std::string mkdir_cmd = "mkdir -p " + temp_dir;
    system(mkdir_cmd.c_str());
    
    // Copy file to temp directory
    std::string cp_cmd = "cp \"" + file_path + "\" " + temp_dir + "/";
    system(cp_cmd.c_str());
    
    // Run Docker sandbox
    std::string docker_cmd = "docker run --rm -v " + temp_dir + ":/sandbox/input -v " + 
                            temp_dir + "_output:/sandbox/output hydraav-sandbox";
    
    int result = system(docker_cmd.c_str());
    
    // Clean up
    std::string cleanup_cmd = "rm -rf " + temp_dir + " " + temp_dir + "_output";
    system(cleanup_cmd.c_str());
    
    std::cout << "Sandbox analysis completed with exit code: " << result << std::endl;
    
    return result;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " [OPTIONS] <file_path_or_directory>" << std::endl;
        std::cerr << "Options:" << std::endl;
        std::cerr << "  -r, --recursive          Scan directories recursively" << std::endl;
        std::cerr << "  --include-media          Include media files in scan" << std::endl;
        std::cerr << "  --sandbox               Enable sandbox analysis (requires Docker)" << std::endl;
        std::cerr << "  --no-sandbox            Disable sandbox analysis (default)" << std::endl;
        std::cerr << "  -v, --verbose           Verbose output" << std::endl;
        std::cerr << "  -h, --help              Show this help message" << std::endl;
        return 1;
    }

    bool recursive = false;
    bool include_media = false;
    bool enable_sandbox = false;
    bool verbose = false;
    std::string target_path;
    
    // Parse command line arguments
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        
        if (arg == "-h" || arg == "--help") {
            std::cerr << "Usage: " << argv[0] << " [OPTIONS] <file_path_or_directory>" << std::endl;
            std::cerr << "Options:" << std::endl;
            std::cerr << "  -r, --recursive         Scan directories recursively" << std::endl;
            std::cerr << "  --include-media         Include media files in scan" << std::endl;
            std::cerr << "  --sandbox               Enable sandbox analysis (requires Docker)" << std::endl;
            std::cerr << "  --no-sandbox            Disable sandbox analysis (default)" << std::endl;
            std::cerr << "  -v, --verbose           Verbose output" << std::endl;
            std::cerr << "  -h, --help              Show this help message" << std::endl;
            return 0;
        } else if (arg == "-r" || arg == "--recursive") {
            recursive = true;
        } else if (arg == "--include-media") {
            include_media = true;
        } else if (arg == "--sandbox") {
            enable_sandbox = true;
        } else if (arg == "--no-sandbox") {
            enable_sandbox = false;
        } else if (arg == "-v" || arg == "--verbose") {
            verbose = true;
        } else if (arg[0] == '-') {
            std::cerr << "Unknown option: " << arg << std::endl;
            std::cerr << "Use -h or --help for usage information." << std::endl;
            return 1;
        } else {
            // Target path
            if (target_path.empty()) {
                target_path = arg;
            } else {
                std::cerr << "Multiple target paths specified. Only one allowed." << std::endl;
                return 1;
            }
        }
    }
    
    if (target_path.empty()) {
        std::cerr << "No target path specified." << std::endl;
        std::cerr << "Use -h or --help for usage information." << std::endl;
        return 1;
    }
    
    fs::path path(target_path);
    
    if (!fs::exists(path)) {
        std::cerr << "Path does not exist: " << target_path << std::endl;
        return 1;
    }
    
    if (fs::is_directory(path)) {
        // Scan directory
        int result = scan_directory(target_path, recursive, include_media);
        return result;
    } else if (fs::is_regular_file(path)) {
        // For single files, check if it's a media file
        if (!include_media && is_media_file(target_path)) {
            std::cout << "Skipping media file: " << target_path << std::endl;
            std::cout << "Use --include-media to scan media files." << std::endl;
            return 0;
        }
        
        // Scan single file
        if (!verify_file(target_path)) {
            return 1;
        }

        ScanResult result;
        int scan_result = scan_file(target_path, result);

        if (scan_result == -1) {
            std::cerr << "Failed to open file: " << target_path << std::endl;
            return 1;
        } else if (scan_result == 0) {
            std::cout << "Scan completed: " << result.description << std::endl;
            
            if (enable_sandbox) {
                std::cout << "Proceeding with sandbox execution..." << std::endl;
                // Call the Docker sandbox for dynamic analysis
                int sandbox_result = call_sandbox(target_path);
                return sandbox_result;
            } else {
                std::cout << "Sandbox analysis disabled. Use --sandbox to enable." << std::endl;
                return 0;
            }
        } else if (scan_result == 1) {
            std::cout << "Malware detected: " << result.signature_name
                      << " at offset " << result.offset << std::endl;
            std::cout << result.description << std::endl;
            return 1;
        } else {
            std::cerr << "Scan failed! Unknown error occurred." << std::endl;
            return 1;
        }
    } else {
        std::cerr << "Path is neither a file nor a directory: " << target_path << std::endl;
        return 1;
    }
}