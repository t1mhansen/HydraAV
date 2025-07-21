# HydraAV Demo Files

These files are provided for testing and demonstrating HydraAV's capabilities.

## ⚠️ **Important Note**

These are **harmless test files** designed to trigger different detection levels. They do not contain actual malware and are safe to run.

## Demo Files

### `clean_test.py`
- **Purpose**: Demonstrates normal, safe behavior
- **Expected Result**: Should pass all scans
- **Use Case**: Testing that HydraAV doesn't generate false positives

### `suspicious_test.py`
- **Purpose**: Demonstrates suspicious behavior patterns
- **Expected Result**: Should trigger suspicious warnings
- **Behaviors**: 
  - System command execution attempts
  - Multiple file creation
  - Network access attempts

### `malware_test.py`
- **Purpose**: Demonstrates malware-like behavior patterns
- **Expected Result**: Should trigger high-risk alerts
- **Behaviors**:
  - Rapid file creation
  - Multiple suspicious command attempts
  - Process spawning
  - Sensitive directory access attempts

## Usage Examples

```bash
# Test static analysis
./scanner/build/hydraav-scan tests/demo_files/clean_test.py
./scanner/build/hydraav-scan tests/demo_files/suspicious_test.py
./scanner/build/hydraav-scan tests/demo_files/malware_test.py

# Test with sandbox (requires Docker)
./scanner/build/hydraav-scan --sandbox tests/demo_files/malware_test.py

# Test directory scanning
./scanner/build/hydraav-scan -r tests/demo_files/
```

## Expected Results

| File | Static Scan | Sandbox Analysis |
|------|-------------|------------------|
| `clean_test.py` | CLEAN | Low risk score |
| `suspicious_test.py` | SUSPICIOUS | Medium risk score |
| `malware_test.py` | SUSPICIOUS | High risk score |
