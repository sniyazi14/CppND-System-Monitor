#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// read operating system information from file
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// read kernel from file
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >>version>> kernel;
  }
  return kernel;
}

// read the process ids and return in vector 
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string line, key,value;
  vector<int> memInfo = {}; //memTotal, memFree, memAvailable, buffers;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    for(int i=0; i<4; i++){
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> key >> value;
    memInfo.push_back(stoi(value));
    }
    return (memInfo[0]- memInfo[1])/ ((float)memInfo[0]);
  }
  return 0.0; }

// Read and return the system uptime
long LinuxParser::UpTime() {  
  string uptime, other;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime>> other;
  }
  long num = atol(uptime.c_str());
  return num; } 

// Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  vector<string> time = LinuxParser::CpuUtilization();
  long sum = 0;
  for(unsigned int i =0; i < time.size(); i++){
    sum += stoi(time[i]);
  }
  return sum; }

// Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) { 
  string line, value;
  long sum = 0;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    for(int i=0; i<13; i++){
      linestream >> value;
    }
    for(int i=0; i<4; i++){
      linestream >> value;
      sum += stoi(value);
    }
    return sum; }
    return sum;
}

// Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  vector<string> time = LinuxParser::CpuUtilization();
  return stoi(time[CPUStates::kUser_]) + stoi(time[CPUStates::kNice_]) +
  stoi(time[CPUStates::kSteal_]) + stoi(time[CPUStates::kSoftIRQ_]) +
  stoi(time[CPUStates::kIRQ_]) + stoi(time[CPUStates::kSystem_]); 
}

//Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  vector<string> time = LinuxParser::CpuUtilization();
  return stoi(time[CPUStates::kIOwait_]) + stoi(time[CPUStates::kIdle_]);
}

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  string line, word;
  vector<string> cpu = {};
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> word;
    for(int i=0; i<10; i++){
      linestream >> word;
      cpu.push_back(word);
    }
  } 
  return cpu; } 

// Read and return the total number of processes  
int LinuxParser::TotalProcesses(){ 
  string line, key,value;
  int processes;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "processes") {
        processes = stoi(value);
        return processes;
      }
    }
  }
  return processes;
  }  

// Read and return the number of running processes
int LinuxParser::RunningProcesses() { string line, key,value;
  int processes;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
        if (key == "procs_running") {
          processes = stoi(value);
          return processes;
        }
    }
  }
  return processes; } 

// Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  string line, command, num;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >>num >> command;
    return command;
  }
  return command;
 }

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) { 
  string line, key, value;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
        if (key == "VmSize:") {
          return to_string(stoi(value)/1000);
        }
    }
  }
  return string();
 }

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  string line, key, value;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "Uid:") {
        return value;
      }
    }
  }
  return string(); }

// Read and return the user associated with a process
string LinuxParser::User(int pid) { 
  string line, user, uid, x;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> user >> x >> uid;
        if (uid == Uid(pid)) {
          return user;
        }
    }
  }
  return string();
   }

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) { 
  string line, value;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    for(int i=0; i<22; i++){
      linestream >> value;
    }
    long uptime = LinuxParser::UpTime();
    return uptime - stoi(value)/sysconf(_SC_CLK_TCK);
  }
  return 0; 
}