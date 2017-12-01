#include "AccOmegaControl.h"
#include <math.h>

void AccOmegaControl::ComputeControl(Control& u, const Maneuver& maneuver) const
{
	const double K = 10;
	
	const State& q0 = GetSelfTrajectory().last().value().GetState();
	const EnvironmentParameters& env = GetEnvironmentTrajectory().last().value();
	
	const double& y = (q0("yb") + q0("yf"))/2.0;
	const double& theta = atan2(q0("yf") - q0("yb"), q0("xf") - q0("xb"));
	
	double laneWidth;
	if (env.IsAvailable("lane_width"))
		laneWidth = env("lane_width");
	else
		Error("AccOmegaControl::ComputeControl", "Environment lane width is needed");
	
	
	double beginOfLaneY = floor(q0("y")/laneWidth);
	
	if (maneuver == "FAST")
	{
		u("a") = 1;
		u("omega") = -(y - (beginOfLaneY + laneWidth/2.0))*q0("v")*sin(theta)/theta - K*q0("v")*theta;
	}
	else
		Error("AccOmegaControl::ComputeControl", string("Unrecognized maneuver: ") + maneuver.GetManeuverName());
	
}
