#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// Return the system's CPU
Processor& System::Cpu() { 
    cpu_.setJiffies(LinuxParser::Jiffies()); //set private variables of processor object
    cpu_.setActiveJiffies( LinuxParser::ActiveJiffies());
    cpu_.setIdleJiffies(LinuxParser::IdleJiffies());
    return cpu_; }

// Returns a container composed of the system's processes
vector<Process>& System::Processes() { 
    vector<int> pids = LinuxParser::Pids();//the ids of the processes
    processes_ = {};
    for(int pid:pids){  //for loop to add the process object for each id to vector
        Process process(pid);
        processes_.push_back(process);
    }
    std::sort(processes_.begin(), processes_.end(), [](Process a, Process b) { //sort according to highest CPU utilization
        return b < a;   
    });
     return processes_; 
    }

// Returns the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// Returns the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// Returns the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// Returns the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// Returns the number of seconds since the system started running
long System::UpTime() { return LinuxParser::UpTime(); }