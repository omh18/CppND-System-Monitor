#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include "linux_parser.h"
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  //constructor
  Process(int Pid_);
  // update existing pid member variables
  // Use previous values of Jiffies to produce deltas and get a more current value for CPU usage of process
  void Update();
  int Pid();                               // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp

  // TODO: Declare any necessary private members
  private:
  std::string User_, Command_, Ram_;
  float CpuUtilization_;
  int Pid_;
  int long UpTime_, prevAJiffies_;
};

#endif
