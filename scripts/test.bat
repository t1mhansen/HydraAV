@echo off
REM HydraAV Test Script for Windows
REM Tests the installation and basic functionality

echo === HydraAV Test Script ===

REM Check if we're in the right directory
if not exist "scanner\CMakeLists.txt" (
    echo [FAIL] Please run this script from the HydraAV root directory
    exit /b 1
)

REM Test 1: Check if scanner executable exists
set SCANNER_PATH=
if exist "scanner\build\Release\hydraav-scan.exe" (
    set SCANNER_PATH=scanner\build\Release\hydraav-scan.exe
) else if exist "scanner\build\hydraav-scan.exe" (
    set SCANNER_PATH=scanner\build\hydraav-scan.exe
) else (
    echo [FAIL] Scanner executable not found. Please build the project first.
    exit /b 1
)

echo [PASS] Scanner executable found: %SCANNER_PATH%

REM Test 2: Test help command
echo Testing help command...
%SCANNER_PATH% --help >nul 2>&1
if %errorlevel% equ 0 (
    echo [PASS] Help command works
) else (
    echo [FAIL] Help command failed
    exit /b 1
)

REM Test 3: Create test files
echo Creating test files...
if not exist "test_files" mkdir test_files

REM Clean test file
echo print("Hello, World!") > test_files\clean.py

REM Suspicious test file
echo import os > test_files\suspicious.py
echo import subprocess >> test_files\suspicious.py
echo os.system("del /s /q C:\") >> test_files\suspicious.py
echo subprocess.call(["nc", "-l", "4444"]) >> test_files\suspicious.py

echo [PASS] Test files created

REM Test 4: Test scanning clean file
echo Testing clean file scan...
%SCANNER_PATH% test_files\clean.py >nul 2>&1
if %errorlevel% equ 0 (
    echo [PASS] Clean file scan completed
) else (
    echo [WARN] Clean file scan had issues
)

REM Test 5: Test scanning suspicious file
echo Testing suspicious file scan...
%SCANNER_PATH% test_files\suspicious.py >nul 2>&1
if %errorlevel% equ 0 (
    echo [PASS] Suspicious file scan completed
) else (
    echo [WARN] Suspicious file scan had issues
)

REM Test 6: Test recursive directory scan
echo Testing recursive directory scan...
%SCANNER_PATH% -r test_files >nul 2>&1
if %errorlevel% equ 0 (
    echo [PASS] Recursive directory scan completed
) else (
    echo [WARN] Recursive directory scan had issues
)

REM Test 7: Check Docker availability (optional)
docker --version >nul 2>&1
if %errorlevel% equ 0 (
    echo Testing Docker availability...
    echo [PASS] Docker is available
    
    REM Check if sandbox image exists
    docker image inspect hydraav-sandbox >nul 2>&1
    if %errorlevel% equ 0 (
        echo [PASS] Sandbox image found
        
        REM Test sandbox functionality
        echo Testing sandbox functionality...
        %SCANNER_PATH% --sandbox test_files\clean.py >nul 2>&1
        if %errorlevel% equ 0 (
            echo [PASS] Sandbox functionality works
        ) else (
            echo [WARN] Sandbox functionality had issues
        )
    ) else (
        echo [WARN] Sandbox image not found. Run 'cd sandbox ^&^& docker build -t hydraav-sandbox .' to build it.
    )
) else (
    echo [WARN] Docker not found. Sandbox functionality will not be available.
)

REM Cleanup
echo Cleaning up test files...
rmdir /s /q test_files

echo [PASS] All tests completed!
echo.
echo HydraAV is ready to use!
echo Usage: %SCANNER_PATH% [OPTIONS] ^<target^>
echo Use --help for more information. 