#ifndef TEST_KINEMATICS_H
#define TEST_KINEMATICS_H

#include "Basic/Control.h"
#include "Basic/Agent.h"
#include "Utility/LogFunctions.h"
#include <math.h>
#include <iostream>

void TestKinematics(State& qdot, const Agent &self, const AgentVector& neighbors, const Control & u)
{
		const State& q0 = self.GetState();
		
		qdot("x") = q0("v")*cos(q0("theta"));	
		qdot("y") = q0("v")*sin(q0("theta"));
		
		qdot("theta") = u("omega");
		qdot("v") = u("a");
}

#endif
