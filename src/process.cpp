#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

// Constructor
Process::Process(int pid) : Pid_(pid){
  User_ = LinuxParser::User(pid);
  Command_ = LinuxParser::Command(pid);
  Ram_ = LinuxParser::Ram(pid);
  UpTime_ = LinuxParser::UpTime(pid);
  prevAJiffies_ = LinuxParser::ActiveJiffies(pid);
  
  CpuUtilization_ = ( (float) prevAJiffies_ / sysconf(_SC_CLK_TCK) ) / (float) UpTime_;
};

void Process::Update(){
  // update existing pid member variables
  // Use previous values of Jiffies to produce deltas and get a more current value for CPU usage of process
  int long AJiffies = LinuxParser::ActiveJiffies(Pid_), UpTime = LinuxParser::UpTime(Pid_);
  CpuUtilization_ = ( (float) (AJiffies - prevAJiffies_) / sysconf(_SC_CLK_TCK) ) / (float) (UpTime - UpTime_);
  prevAJiffies_ = AJiffies;
  UpTime_ = UpTime;
  Ram_ = LinuxParser::Ram(Pid_);
  // No need to update User and Command
}

// TODO: Return this process's ID
int Process::Pid() { 
  return Pid_; 
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { return CpuUtilization_; }

// TODO: Return the command that generated this process
string Process::Command() { return Command_; }

// TODO: Return this process's memory utilization (in MB)
string Process::Ram() { return Ram_; }

// TODO: Return the user (name) that generated this process
string Process::User() { return User_; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return UpTime_; }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
	return CpuUtilization_ < a.CpuUtilization_;
}
