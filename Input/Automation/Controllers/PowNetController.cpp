#include "PowNetController.h"
#include "Utility/MyLogger.h"
#include "Utility/LogFunctions.h"
#include "Utility/Math.h"
#include <math.h>
#include <string>
#include <iostream>

using namespace LogFunctions;
using namespace std;

PowNetController::PowNetController(const std::string& className) : Controller(className)
{
}


void PowNetController::ComputeControl(Control& u, const Maneuver& maneuver) const
{	
	u = Control::GenerateStateOfType(GetControlModel().GetControlVariables());
	
	Require(GetSelfTrajectory().size() != 0 && GetOtherAgentsTrajectory().size() != 0, 
			"PowNetController::ComputeControl", "No sensor data available");
	
	const Agent& self = GetSelfTrajectory().begin().value();
	const AgentVector& others = GetOtherAgentsTrajectory().begin().value();
	
	double l1 = 1;
	double desiredV = 1.4941;
	
	if (maneuver == "sigma_1")
	{
		u("u1") = -l1*self("xc");
		
		double interaction = 0;
		for (auto itr = others.begin(); itr != others.end(); itr++)
		{
			double deltaV = pow(self("xc"), 2) - pow(itr->second("xc"), 2);
			double deltaS = self("xc")*itr->second("xc");
			
			interaction -= (self("xc") - itr->second("xc"))*Utility::Sign(deltaV*deltaS);
		}
		
		u("u2") = l1*(self("V") - desiredV) + interaction;
	}
	else if (maneuver == "sigma_2")
	{
		u("u1") = +l1*self("xc");
		
		double interaction = 0;
		for (auto itr = others.begin(); itr != others.end(); itr++)
		{
			double deltaV = pow(self("xc"), 2) - pow(itr->second("xc"), 2);
			double deltaS = self("xc")*itr->second("xc");
			
			interaction -= (self("xc") - itr->second("xc"))*Utility::Sign(deltaV*deltaS);
		}
		
		u("u2") = -l1*(self("V") - desiredV) + interaction;
	}
	else if (maneuver == "reset")
	{
		u("u1") = 0;
		u("u2") = -100*self("xc");
	}
	else
		Error("PowNetController::ComputeControl", string("Unrecognized maneuver: ") + maneuver.GetName());
}
