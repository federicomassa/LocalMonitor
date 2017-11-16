#include "TestKinematics.h"
#include "Utility/LogFunctions.h"
#include <iostream>

void TestKinematics(State &qdot, const State &q0, const Maneuver & maneuver)
{
	if (maneuver == "FAST")
	{
		qdot.at("x") = q0.at("v");
	}
	else if (maneuver == "SLOW")
		qdot.at("x") = q0.at("v");
	else
		LogFunctions::Error("TestKinematics", "Unrecognized maneuver");	
}
