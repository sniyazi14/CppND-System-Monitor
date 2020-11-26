#include <string>

#include "format.h"

using std::string;
using std::to_string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    string hrs, min, sec;
    if(seconds / 3600 < 10 ) { 
        hrs = "0"+ to_string(seconds / 3600);
        }
    else {
        hrs = to_string(seconds / 3600);
    }

    if((seconds % 3600)/60 < 10 ) { 
        min = "0"+ to_string((seconds % 3600)/60);
        }
    else {
        min = to_string((seconds % 3600)/60);
    }


    if(seconds % 60 < 10 ) { 
        sec = "0"+ to_string(seconds % 60);
        }
    else {
        sec = to_string(seconds %60);
    }
    return hrs + ":" + min + ":" + sec; }