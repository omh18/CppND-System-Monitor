#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
  //Use previous values of Jiffies to produce deltas and get a more current value for CPU usage
  long AJiffies = LinuxParser::ActiveJiffies();
  long Jiffies = LinuxParser::Jiffies();
  return ((float) (AJiffies - prevAJiffies) / (Jiffies - prevJiffies));
  prevAJiffies = LinuxParser::ActiveJiffies();
  prevJiffies = LinuxParser::Jiffies();
}
