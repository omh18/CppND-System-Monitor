#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
	char buffer [9];
  	long minutes = seconds / 60;
    int hours = minutes / 60;
	seconds -= minutes * 60;
  	minutes -= hours * 60;
  	snprintf (buffer, 9, "%.2d:%.2d:%.2d", hours, (int) minutes, (int) seconds);
  	return buffer; 
}
