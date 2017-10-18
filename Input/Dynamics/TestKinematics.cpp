#include "TestKinematics.h"
#include "Utility/LogFunctions.h"
#include <iostream>

void TestKinematics(State &qdot, const State &q0, const Maneuver & maneuver)
{
	if (maneuver == "FAST")
		qdot.at("x") = 1;
	else if (maneuver == "SLOW")
		qdot.at("x") = -1;
	else
		LogFunctions::Error("TestKinematics", "Unrecognized maneuver");	
}
