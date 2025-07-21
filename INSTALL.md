# HydraAV Installation Guide

## Prerequisites

### For Static Analysis Only
- **C++ Compiler**: GCC 7+, Clang 6+, or MSVC 2017+
- **CMake**: Version 3.10 or higher
- **Make**: (Unix/Linux) or Visual Studio (Windows)

### For Full Analysis (Static + Sandbox)
- All prerequisites for static analysis
- **Docker**: Version 20.0 or higher

## Installation by Platform

### Linux (Ubuntu/Debian)

```bash
# Install dependencies
sudo apt update
sudo apt install build-essential cmake git

# Optional: Install Docker for sandbox functionality
sudo apt install docker.io
sudo systemctl enable --now docker
sudo usermod -aG docker $USER

# Clone and build
git clone https://github.com/t1mhansen/HydraAV.git
cd HydraAV
chmod +x scripts/build.sh
./scripts/build.sh
```

### macOS

```bash
# Install dependencies using Homebrew
brew install cmake

# Optional: Install Docker for sandbox functionality
brew install --cask docker

# Clone and build
git clone https://github.com/t1mhansen/HydraAV.git
cd HydraAV
chmod +x scripts/build.sh
./scripts/build.sh
```

### Windows

#### Using Visual Studio
1. Install a code editor of choice / IDE with C++ development tools
2. Install CMake from https://cmake.org/download/
3. Clone the repository
4. Run `scripts\build.bat`

#### Using WSL (Windows Subsystem for Linux)
```bash
# Install WSL and Ubuntu
wsl --install

# Follow Linux installation instructions above
```

## Building Options

### Static Analysis Only (Recommended)
```bash
./scripts/build.sh
```

### Full Analysis with Sandbox
```bash
./scripts/build.sh --sandbox
```

## Verification

After installation, test the scanner:

```bash
# Test static analysis
./scanner/build/hydraav-scan --help

# Test with a sample file
echo "print('Hello World')" > test.py
./scanner/build/hydraav-scan test.py
```

## Troubleshooting

### Build Errors
- **CMake not found**: Install CMake from your package manager
- **Compiler not found**: Install build tools for your platform
- **Permission denied**: Use `sudo` on Linux/macOS or run as Administrator on Windows

### Docker Issues
- **Docker not running**: Start Docker Desktop or Docker service
- **Permission denied**: Add your user to the docker group (`sudo usermod -aG docker $USER`)
- **Image not found**: Run `cd sandbox && docker build -t hydraav-sandbox .`

### Runtime Issues
- **File not found**: Ensure the target file exists and is accessible
- **Permission denied**: Check file permissions and ownership

## Uninstallation

```bash
# Remove built files
rm -rf scanner/build

# Remove Docker image (if installed)
docker rmi hydraav-sandbox

# Remove source code
rm -rf HydraAV
``` 