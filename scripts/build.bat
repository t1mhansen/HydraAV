@echo off
REM HydraAV Build Script for Windows
REM Builds the static scanner and optionally the sandbox

echo === HydraAV Build Script ===

REM Check if we're in the right directory
if not exist "scanner\CMakeLists.txt" (
    echo [ERROR] Please run this script from the HydraAV root directory
    exit /b 1
)

REM Build static scanner
echo [INFO] Building static scanner...
cd scanner

REM Create build directory
if not exist "build" mkdir build
cd build

REM Configure with CMake
echo [INFO] Configuring with CMake...
cmake ..

REM Build
echo [INFO] Building scanner...
cmake --build . --config Release

REM Check if build was successful
if exist "Release\hydraav-scan.exe" (
    echo [INFO] Static scanner built successfully!
) else (
    echo [ERROR] Failed to build static scanner
    exit /b 1
)

cd ..\..

REM Check if Docker is available for sandbox build
docker --version >nul 2>&1
if %errorlevel% equ 0 (
    echo [INFO] Docker found. Building sandbox...
    
    REM Check if user wants to build sandbox
    if "%1"=="--sandbox" (
        cd sandbox
        echo [INFO] Building Docker sandbox image...
        docker build -t hydraav-sandbox .
        
        if %errorlevel% equ 0 (
            echo [INFO] Sandbox built successfully!
        ) else (
            echo [ERROR] Failed to build sandbox
            exit /b 1
        )
        
        cd ..
    ) else (
        echo [WARNING] Docker available but sandbox not requested. Use --sandbox to build it.
    )
) else (
    echo [WARNING] Docker not found. Sandbox functionality will not be available.
    echo [WARNING] Install Docker to enable dynamic analysis features.
)

echo [INFO] Build completed successfully!
echo [INFO] You can now use: scanner\build\Release\hydraav-scan.exe ^<target^> 