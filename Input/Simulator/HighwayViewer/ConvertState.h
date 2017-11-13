#ifndef CONVERT_STATE_H
#define CONVERT_STATE_H

#include <string>

class State;

// Get x,y,theta from agent (for visualization)
void ConvertFromState(const State& q, const std::string&, double& x, double& y, double& theta);

#endif
