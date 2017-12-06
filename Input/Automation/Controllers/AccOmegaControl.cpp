#include "AccOmegaControl.h"
#include "Utility/LogFunctions.h"
#include "Utility/Logger.h"
#include <math.h>
#include <string>
#include <iostream>

using namespace LogFunctions;
using namespace std;

extern Logger logger;

void AccOmegaControl::ComputeControl(Control& u, const Maneuver& maneuver) const
{
	const double K = 0.5;
	
	const State& q0 = GetSelfTrajectory().latest().value().GetState();
	
	const string& myID = GetSelfTrajectory().latest().value().GetID();
	
	string otherID;
	if (myID == "0")
		otherID = "1";
	else
		otherID = "0";
	
	const AgentVector& others = GetOtherAgentsTrajectory().latest().value();
	for (auto itr = others.begin(); itr != others.end(); itr++)
	{
		logger <<  myID << " sees: " << itr->first << logger.EndL();
	}
	
	Agent otherAgent = others.at(otherID);
	
	logger << "Other agent: " << otherAgent << logger.EndL();
	
	
	const EnvironmentParameters& env = GetEnvironmentTrajectory().latest().value();
	
	const double& y = (q0("yb") + q0("yf"))/2.0;
	const double& theta = atan2(q0("yf") - q0("yb"), q0("xf") - q0("xb"));
	
	cout << "Theta? " << theta << endl;
	
	double laneWidth;
	if (env.IsAvailable("lane_width"))
		laneWidth = env("lane_width");
	else
		Error("AccOmegaControl::ComputeControl", "Environment lane width is needed");
	
	
	double beginOfLaneY = floor(y/laneWidth);
	
	
	logger << "lw: " << laneWidth << " \t beginOfLaneY: " << beginOfLaneY << logger.EndL();
	
	if (maneuver == "FAST")
	{
		u("a") = 1;
		if (fabs(theta) > 1E-9)
			u("omega") = -(y - (beginOfLaneY + laneWidth/2.0))*q0("v")*sin(theta)/theta - K*q0("v")*theta;
		else
			u("omega") = -(y - (beginOfLaneY + laneWidth/2.0))*q0("v");
	}
	else
		Error("AccOmegaControl::ComputeControl", string("Unrecognized maneuver: ") + maneuver.GetManeuverName());
	
	
	logger << "Control: " << u << logger.EndL();
	
}
