#include "scanner.h"
#include <vector>

// Global signatures vector
static std::vector<Signature> signatures = {
    // Byte patterns
    {"EICAR-Test-String", {'X','5','O','!','P','%','@','A','P','[','4','\\','P','Z','X','5','4','(','P','^',')','7','C','C',')','$','E','I','C','A','R','-','S','T','A','N','D','A','R','D','-','A','N','T','I','V','I','R','U','S','-','T','E','S','T','-','F','I','L','E','!','$','H','+','H','*'}, "", false},
    
    // String patterns 
    {"Suspicious-Malware", {}, "malware", true},
    {"Suspicious-Virus", {}, "virus", true},
    {"Suspicious-Shellcode", {}, "shellcode", true},
    {"Suspicious-Exploit", {}, "exploit", true},
    {"Suspicious-Payload", {}, "payload", true},
    {"Suspicious-CMD", {}, "cmd.exe", true},
    {"Suspicious-PowerShell", {}, "powershell", true},
    {"Suspicious-Registry", {}, "reg add", true},
    {"Suspicious-Network", {}, "http://", true},
    {"Suspicious-Network2", {}, "https://", true},
};

const std::vector<Signature>& get_signatures() {
    return signatures;
}

