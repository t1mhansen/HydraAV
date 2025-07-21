# HydraAV - Lightweight Antivirus Toolkit

A fast, lightweight antivirus toolkit with static scanning and optional sandbox execution capabilities.

## Features

- **Static Analysis**: Fast file scanning with signature detection
- **Dynamic Analysis**: Optional sandbox execution for suspicious files
- **Cross-platform**: Works on Windows, Linux, and macOS
- **Lightweight**: Minimal dependencies, fast execution
- **Extensible**: Easy to add new signatures and detection methods

## Quick Start

### Static Analysis Only (Recommended for most users)

```bash
# Build the scanner
cd scanner
mkdir build && cd build
cmake ..
make

# Scan a file
./hydraav-scan suspicious_file.exe

# Scan a directory recursively
./hydraav-scan -r /path/to/directory
```

### Full Analysis (Static + Sandbox)

Requires Docker to be installed.

```bash
# Build everything
./scripts/build.sh

# Run full analysis
./hydraav-scan --sandbox suspicious_file.exe
```

## Installation

### Prerequisites

- **Static Analysis**: C++ compiler (GCC, Clang, MSVC, or MinGW-w64)
- **Full Analysis**: Docker (optional)

### Build Instructions

1. **Clone the repository**
   ```bash
   git clone https://github.com/t1mhansen/HydraAV.git
   cd HydraAV
   ```

2. **Build the scanner**
   ```bash
   cd scanner
   mkdir build && cd build
   cmake ..
   make
   ```

3. **Optional: Build sandbox (requires Docker)**
   ```bash
   cd ../../sandbox
   docker build -t hydraav-sandbox .
   ```

## Usage

### Command Line Options

```bash
hydraav-scan [OPTIONS] <target>

Options:
  -r, --recursive          Scan directories recursively
  --include-media          Include media files in scan
  --sandbox               Enable sandbox analysis (requires Docker)
  --no-sandbox            Disable sandbox analysis (default)
  -v, --verbose           Verbose output
  -h, --help              Show this help message
```

### Examples

```bash
# Scan a single file
./hydraav-scan malware.exe

# Scan directory recursively
./hydraav-scan -r /home/user/downloads

# Full analysis with sandbox
./hydraav-scan --sandbox suspicious_file.py

# Static analysis only (faster)
./hydraav-scan --no-sandbox suspicious_file.py

# Test with demo files
./hydraav-scan tests/demo_files/clean_test.py
./hydraav-scan tests/demo_files/suspicious_test.py
./hydraav-scan tests/demo_files/malware_test.py
```

## Architecture

```
HydraAV/
├── scanner/             # Static analysis engine (C++)
│   ├── src/             # Source code
│   ├── include/         # Headers
│   └── build/           # Build output
├── sandbox/             # Dynamic analysis (Python + Docker)
│   ├── container_analyzer.py
│   └── Dockerfile
├── scripts/             # Build and utility scripts
└── tests/               # Test files
```

## Security Features

### Static Analysis
- File signature detection
- Heuristic analysis
- File type identification
- Suspicious pattern matching

### Dynamic Analysis (Optional)
- Isolated execution environment
- Process monitoring
- File system change detection
- Network activity monitoring
- Risk scoring

## Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Add tests
5. Submit a pull request

## License

MIT License - see LICENSE file for details

## Disclaimer

This tool is for educational and research purposes. Use at your own risk. The author is not responsible for any damage caused by the use of this software. 