#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  void setJiffies(long jiffy);
  void setActiveJiffies(long aJiffy);
  void setIdleJiffies(long iJiffy);
  float Utilization(); 

 private:
 long int activeJiffies;
 long int Jiffies;
 long int idleJiffies;
 
};

#endif