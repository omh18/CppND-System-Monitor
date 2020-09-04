#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <algorithm>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() {
  vector<Process> processes{};
  processes.reserve(LinuxParser::TotalProcesses());
	vector<int> pids = LinuxParser::Pids();
  	for(auto& pid : pids){
      // Uncomment below if Process::Update() working
      /*bool found = false;
      for(auto process : processes_){
        if(pid == process.Pid()){
          // It appears that some processes expire in between call to LinuxParser::Pids(), and this code
          // Hence, exceptions (where [pid] file no longer exists) are caught, using a std::invalid_argument exception added in code
          try{
            process.Update();
            processes.push_back(process);
            found = true;
            break;
          }

          catch(std::invalid_argument&){
            found = true;
            break;
          }
        }
      }
      
      if(!found){*/

      // It appears that some processes expire in between call to LinuxParser::Pids(), and this code
      // Hence, exceptions (where [pid] file no longer exists) are caught, using a std::invalid_argument exception added in code
      // These processes are then skipped, i.e. not added to System::processes_
      // Lead to a std::bad_alloc, SIGABRT or SIGSEV error
      try{
      	Process process(pid);
        processes.push_back(process);
      }

      catch(const std::invalid_argument&){}
      //} <-- Uncomment if Process::Update() working
    }

    // Sort by processes by CPU Utilization
  	std::sort(processes.rbegin(), processes.rend());
  	processes_ = processes;
  	return processes_;
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// TODO: Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }
