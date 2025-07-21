#!/usr/bin/env python3
"""
HydraAV Container Analyzer
Runs inside Docker container to analyze files safely
"""

import os
import sys
import time
import json
import subprocess
import psutil
import threading
from pathlib import Path
from datetime import datetime
import logging

class ContainerAnalyzer:
    def __init__(self):
        self.results = {
            'container_id': os.environ.get('HOSTNAME', 'unknown'),
            'timestamp': datetime.now().isoformat(),
            'file_analyzed': None,
            'file_system_changes': [],
            'processes_created': [],
            'network_connections': [],
            'suspicious_activities': [],
            'overall_risk_score': 0,
            'conclusion': 'CLEAN'
        }
        
        # Setup logging
        logging.basicConfig(level=logging.INFO)
        self.logger = logging.getLogger('HydraAV_Container')
        
        # Track initial state
        self.initial_files = set()
        self.initial_processes = set()
        
    def capture_initial_state(self):
        """Capture initial state of the container"""
        self.logger.info("Capturing initial container state...")
        
        # Capture initial files
        for root, dirs, files in os.walk('/sandbox'):
            for file in files:
                self.initial_files.add(os.path.join(root, file))
        
        # Capture initial processes
        for proc in psutil.process_iter(['pid', 'name']):
            try:
                self.initial_processes.add((proc.info['pid'], proc.info['name']))
            except (psutil.NoSuchProcess, psutil.AccessDenied):
                pass
                
        self.logger.info(f"Initial state: {len(self.initial_files)} files, {len(self.initial_processes)} processes")
    
    def find_target_file(self):
        """Find the file to analyze in the input directory"""
        input_dir = Path('/sandbox/input')
        if input_dir.exists():
            for file_path in input_dir.iterdir():
                if file_path.is_file():
                    return str(file_path)
        return None
    
    def execute_file(self, file_path):
        """Execute the target file safely"""
        try:
            file_ext = Path(file_path).suffix.lower()
            self.results['file_analyzed'] = file_path
            
            self.logger.info(f"Executing file: {file_path} (type: {file_ext})")
            
            # Make file executable if needed
            os.chmod(file_path, 0o755)
            
            if file_ext in ['.py']:
                # Execute Python files
                result = subprocess.run([sys.executable, file_path], 
                                      timeout=30, capture_output=True, text=True)
            elif file_ext in ['.sh']:
                # Execute shell scripts
                result = subprocess.run(['bash', file_path], 
                                      timeout=30, capture_output=True, text=True)
            elif file_ext in ['.txt', '.log']:
                # Read text files
                with open(file_path, 'r') as f:
                    content = f.read()
                self.logger.info(f"Text file content length: {len(content)} characters")
                return True
            else:
                # Try to execute as binary
                result = subprocess.run([file_path], 
                                      timeout=30, capture_output=True, text=True)
            
            self.logger.info(f"Execution completed with return code: {result.returncode}")
            return True
            
        except subprocess.TimeoutExpired:
            self.logger.warning("Execution timed out")
            return True
        except Exception as e:
            self.logger.error(f"Execution error: {e}")
            return False
    
    def monitor_changes(self):
        """Monitor for changes after execution"""
        self.logger.info("Monitoring for changes...")
        
        # Check for new files
        current_files = set()
        for root, dirs, files in os.walk('/sandbox'):
            for file in files:
                current_files.add(os.path.join(root, file))
        
        new_files = current_files - self.initial_files
        for file_path in new_files:
            self.results['file_system_changes'].append({
                'type': 'file_created',
                'path': file_path,
                'timestamp': datetime.now().isoformat()
            })
        
        # Check for new processes
        current_processes = set()
        for proc in psutil.process_iter(['pid', 'name', 'cmdline']):
            try:
                proc_info = proc.info
                current_processes.add((proc_info['pid'], proc_info['name']))
                
                if (proc_info['pid'], proc_info['name']) not in self.initial_processes:
                    self.results['processes_created'].append({
                        'pid': proc_info['pid'],
                        'name': proc_info['name'],
                        'cmdline': proc_info['cmdline'],
                        'timestamp': datetime.now().isoformat()
                    })
            except (psutil.NoSuchProcess, psutil.AccessDenied):
                pass
    
    def check_suspicious_activities(self):
        """Check for suspicious activities"""
        suspicious_keywords = [
            'nc', 'netcat', 'curl', 'wget', 'ssh', 'telnet',
            'rm', 'del', 'format', 'dd', 'shred'
        ]
        
        for proc_info in self.results['processes_created']:
            process_name = proc_info['name'].lower()
            cmdline = ' '.join(proc_info['cmdline']).lower() if proc_info['cmdline'] else ''
            
            for keyword in suspicious_keywords:
                if keyword in process_name or keyword in cmdline:
                    self.results['suspicious_activities'].append({
                        'type': 'suspicious_process',
                        'process': proc_info['name'],
                        'keyword': keyword,
                        'timestamp': proc_info['timestamp']
                    })
    
    def calculate_risk_score(self):
        """Calculate overall risk score"""
        score = 0
        score += len(self.results['file_system_changes']) * 5
        score += len(self.results['processes_created']) * 10
        score += len(self.results['suspicious_activities']) * 20
        
        self.results['overall_risk_score'] = score
        
        if score >= 50:
            self.results['conclusion'] = 'MALWARE'
        elif score >= 20:
            self.results['conclusion'] = 'SUSPICIOUS'
        else:
            self.results['conclusion'] = 'CLEAN'
    
    def run_analysis(self):
        """Run complete analysis"""
        self.logger.info("Starting container analysis...")
        
        try:
            # Capture initial state
            self.capture_initial_state()
            
            # Find and execute target file
            target_file = self.find_target_file()
            if target_file:
                self.execute_file(target_file)
            else:
                self.logger.warning("No target file found in input directory")
            
            # Monitor for changes
            time.sleep(2)  # Wait for any delayed effects
            self.monitor_changes()
            self.check_suspicious_activities()
            
            # Calculate risk score
            self.calculate_risk_score()
            
            # Save results
            output_file = '/sandbox/output/analysis_results.json'
            with open(output_file, 'w') as f:
                json.dump(self.results, f, indent=2)
            
            self.logger.info(f"Analysis complete. Risk score: {self.results['overall_risk_score']}")
            self.logger.info(f"Conclusion: {self.results['conclusion']}")
            
            return self.results
            
        except Exception as e:
            self.logger.error(f"Analysis error: {e}")
            self.results['conclusion'] = 'ERROR'
            return self.results

def main():
    analyzer = ContainerAnalyzer()
    results = analyzer.run_analysis()
    
    # Print results to stdout for Docker logs
    print(json.dumps(results, indent=2))
    
    # Exit with appropriate code
    if results['conclusion'] == 'MALWARE':
        sys.exit(1)
    elif results['conclusion'] == 'SUSPICIOUS':
        sys.exit(2)
    else:
        sys.exit(0)

if __name__ == "__main__":
    main() 