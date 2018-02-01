#include "AccOmegaControl.h"
#include "Utility/MyLogger.h"
#include "Utility/LogFunctions.h"
#include <math.h>
#include <string>
#include <iostream>

using namespace LogFunctions;
using namespace std;


AccOmegaControl::AccOmegaControl(const std::string& className) : Controller(className)
{
}


void AccOmegaControl::ComputeControl(Control& u, const Maneuver& maneuver) const
{
	u = Control::GenerateStateOfType(GetControlModel().GetControlVariables());
	
	const double K = 1;
	
	const State& q0 = GetSelfTrajectory().begin().value().GetState();
	
	const string myID = GetSelfTrajectory().begin().value().GetID();
	
	const EnvironmentParameters& env = GetEnvironmentTrajectory().begin().value();
	
	const double y = (q0("yb") + q0("yf"))/2.0;
	const double theta = atan2(q0("yf") - q0("yb"), q0("xf") - q0("xb"));
		
	double laneWidth;
	if (env.IsAvailable("lane_width"))
		laneWidth = env("lane_width");
	else
		Error("AccOmegaControl::ComputeControl", "Environment lane width is needed");
	
	
	double beginOfLaneY = floor(y/laneWidth)*laneWidth;
	double centerOfLaneY = beginOfLaneY + laneWidth/2.0;
		
	if (maneuver == "FAST")
	{
		u("a") = 10000000;
		if (fabs(theta) > 1E-9)
			u("omega") = -(y - centerOfLaneY)*q0("v")*sin(theta)/theta - K*q0("v")*theta;
		else
			u("omega") = -(y - centerOfLaneY)*q0("v");
		
	}
	else if (maneuver == "SLOW")
	{
		u("a") = -1000000;
		if (fabs(theta) > 1E-9)
			u("omega") = -(y - centerOfLaneY)*q0("v")*sin(theta)/theta - K*q0("v")*theta;
		else
			u("omega") = -(y - centerOfLaneY)*q0("v");		
	}
	else if (maneuver == "LEFT")
	{
		u("a") = 0;
		
		// Max omega, saturation is given by the system in dynamics fcn
		u("omega") = 10000;
	}
	else if (maneuver == "RIGHT")
	{
		u("a") = 0;
		
		// Max omega, saturation is given by the system in dynamics fcn
		u("omega") = -10000;
	}
	else
		Error("AccOmegaControl::ComputeControl", string("Unrecognized maneuver: ") + maneuver.GetName());	
}
