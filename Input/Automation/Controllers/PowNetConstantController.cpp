#include "PowNetConstantController.h"
#include "Utility/MyLogger.h"
#include "Utility/LogFunctions.h"
#include "Utility/Math.h"
#include <math.h>
#include <string>
#include <iostream>

using namespace LogFunctions;
using namespace std;

PowNetConstantController::PowNetConstantController(const std::string& className) : Controller(className)
{
}


void PowNetConstantController::ComputeControl(Control& u, const Maneuver& maneuver) const
{	
	u = Control::GenerateStateOfType(GetControlModel().GetControlVariables());
	
	Require(GetSelfTrajectory().size() != 0 && GetOtherAgentsTrajectory().size() != 0, 
			"PowNetConstantController::ComputeControl", "No sensor data available");
	
	
	if (maneuver == "constant")
	{
		u("u1") = 0.0;
		u("u2") = 0.0;
	}
	else
		Error("PowNetConstantController::ComputeControl", string("Unrecognized maneuver: ") + maneuver.GetName());
}
