#!/usr/bin/env python3
"""
Suspicious test file - should trigger warnings
"""
import os
import subprocess
import time

print("Starting suspicious operations...")

# Try to execute system commands (should be detected)
try:
    result = subprocess.run(['ls', '-la'], capture_output=True, text=True)
    print(f"Directory listing: {result.stdout}")
except:
    print("Command execution blocked")

# Try to create multiple files (should increase risk score)
for i in range(5):
    with open(f'suspicious_file_{i}.txt', 'w') as f:
        f.write(f"Suspicious content {i}\n")

# Try to access network (should be detected as suspicious)
try:
    import urllib.request
    print("Network access attempted")
except:
    print("Network access blocked")

print("Suspicious test completed!") 