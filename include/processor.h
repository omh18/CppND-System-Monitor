#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  // Use previous values of Jiffies to produce deltas and get a more current value for CPU usage
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
 long prevAJiffies = 0;
 long prevJiffies = 0;
};

#endif
