#include "processor.h"

//functions to set the private variables
void Processor::setJiffies(long jiffy){
    Jiffies = jiffy;
}
void Processor::setActiveJiffies(long aJiffy){
    activeJiffies = aJiffy;
}
void Processor::setIdleJiffies(long iJiffy){
    idleJiffies = iJiffy ;
}

// Returns the aggregate CPU utilization
float Processor::Utilization() { 
    return activeJiffies/ ((float)Jiffies); 
}