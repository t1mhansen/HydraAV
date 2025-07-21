cmake_minimum_required(VERSION 3.10)
project(HydraAV_Scanner)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source files
set(SOURCES
    src/main.cpp
    src/scanner_core.cpp
    src/directory_scanner.cpp
    src/file_utils.cpp
    src/signatures.cpp
)

# Header files
set(HEADERS
    include/scanner.h
    include/directory_scanner.h
    include/file_utils.h
    include/signatures.h
)

# Create executable
add_executable(hydraav-scan ${SOURCES} ${HEADERS})

# Include directories
target_include_directories(hydraav-scan PRIVATE include)

# Platform-specific settings
if(WIN32)
    # Windows-specific settings
    target_compile_definitions(hydraav-scan PRIVATE _WIN32_WINNT=0x0601)
else()
    # Unix/Linux settings
    target_compile_options(hydraav-scan PRIVATE -Wall -Wextra)
endif()

# Set output directory
set_target_properties(hydraav-scan PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)