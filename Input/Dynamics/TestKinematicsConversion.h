#ifndef TEST_KINEMATIC_CONVERSION_H
#define TEST_KINEMATIC_CONVERSION_H

#include "Basic/Agent.h"
#include <math.h>

void TestKinematicsConversion(State& worldState, const Agent& agent)
{
	double L = agent.GetParameter("length");
	double costheta = cos(agent("theta"));
	double sintheta = sin(agent("theta"));
	
	worldState("xb") = agent("x") - L/2*costheta;
	worldState("yb") = agent("y") - L/2*sintheta;
	worldState("xf") = agent("x") + L/2*costheta;
	worldState("yf") = agent("y") + L/2*sintheta;
	worldState("v") = agent("v");
	
	worldState("length") = L;
}

#endif
