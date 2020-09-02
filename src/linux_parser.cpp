#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::stol;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
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

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string line;
  string key;
  string value;
  double MemFree, MemTotal;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal") {
          MemTotal = stol(value);
        }
        
        if (key == "MemFree") {
          MemFree = stol(value);
          return (MemTotal - MemFree)/MemTotal; 
        }
      }
    }
  }
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  string line;
  string uptime;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
  }
  return stol(uptime);
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  long jiffies = 0;
  int counter = 0;
  //Add all jiffies to get jiffies for system
  for(auto& jiffie : CpuUtilization()) {
    //omit guesttime and guestnicetime as already accounted for in usertime and nicetime
    if(counter != kGuest_ && counter != kGuestNice_){
      jiffies += stol(jiffie);
    }
    
    counter++;
  }
  
  return jiffies;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
  string PID = '/' + to_string(pid);
  string line;
  string value;
  int counter = 1;
  long jiffies = 0;
  std::ifstream filestream(kProcDirectory + PID + kStatFilename);
  if (filestream.is_open()) {
    // File is made up of 1 line, only need 1 call to getline
    if (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> value) {
        // add active jiffies for process (14, 15) and its children (16, 17)
        if (counter == 14 || counter == 15 || counter == 16 || counter == 17) {
          jiffies += stol(value);
          
          if(counter == 17){
            break;
          }
        }
        
        counter++;
      }
    }
  }
  return jiffies;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  auto CpuUtil = CpuUtilization();
  // cancel idletime and iowaittime to get active jiffies
  return long {Jiffies() - stol(CpuUtil[kIdle_]) - stol(CpuUtil[kIOwait_])};
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  auto CpuUtil = CpuUtilization();
  // add idletime and iowaittime to get idle jiffies
  return long {stol(CpuUtil[kIdle_]) + stol(CpuUtil[kIOwait_])};
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  string line;
  string cpu;
  std::vector<string> CpuUtil(10);
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    if (std::getline(filestream, line)){
      std::istringstream linestream(line);
      if(linestream >> cpu) {
        for(auto& jiffies : CpuUtil) {
          linestream >> jiffies;
        }
      }
    }
  }
  return CpuUtil;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          return stoi(value);
        }
      }
    }
  }
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          return stoi(value);
        }
      }
    }
  }
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string PID = '/' + to_string(pid);
  string line;
  std::ifstream filestream(kProcDirectory + PID + kCmdlineFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      return line;
    }
  }
}

// TODO: Read and return the memory used by a process (in MB)
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  string PID = '/' + to_string(pid);
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + PID + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize") {
          // divide by 1000 before returning as string to get MB
          return to_string(stol(value) / 1000);
        }
      }
    }
  }
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string PID = '/' + to_string(pid);
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + PID + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid") {
          return value;
        }
      }
    }
  }
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  string PID = '/' + to_string(pid);
  string line;
  string x;
  string value, user;
  string uid = Uid(pid);
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> user >> x >> value) {
        if (value == uid) {
          return user;
        }
      }
    }
  }
}

// TODO: Read and return the uptime of a process (in seconds)
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  string PID = '/' + to_string(pid);
  string line;
  string value;
  int counter = 1;
  long starttime = 0;
  std::ifstream filestream(kProcDirectory + PID + kStatFilename);
  if (filestream.is_open()) {
    // File is made up of 1 line, only need 1 call to getline
    if (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> value) {
        // save startime for procee
        if (counter == 22) {
          starttime = stol(value);
          break;
        }
        
        counter++;
      }
    }
  }
  // minus uptime of system 
  // by starttime of process (converted to seconds by dividing by number of ticks/jiffies per second)
  // to get uptime of process
  return UpTime() - (starttime / sysconf(_SC_CLK_TCK));
}
