#!/usr/bin/env python3
"""
Clean test file - should pass analysis
"""
import os
import time

print("Hello from clean test!")
print(f"Current directory: {os.getcwd()}")
print(f"Files in current directory: {os.listdir('.')}")

# Create a harmless file
with open('test_output.txt', 'w') as f:
    f.write("This is a test output file\n")

print("Test completed successfully!") 