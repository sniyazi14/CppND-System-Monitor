#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process (int id){
    pid = id;
    cpu = CpuUtilization();
}

// TODO: Return this process's ID
int Process::Pid() { return pid; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 
    const long sysUptime = LinuxParser::UpTime();
    const long totalTime = LinuxParser::ActiveJiffies(pid);
    const long startTime = LinuxParser::UpTime(pid);
    const long seconds = sysUptime - (startTime/sysconf(_SC_CLK_TCK));
    
    return (totalTime / ((float)sysconf(_SC_CLK_TCK))) / seconds; }

// TODO: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid); }

// TODO: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid); }

// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid); }//check it is in seconds

// Overloads the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const { 
    if(cpu < a.cpu) return true;
    return false;
    }