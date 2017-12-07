#ifndef TEST_KINEMATICS_H
#define TEST_KINEMATICS_H

#include "Automation/Control.h"
#include "Basic/Agent.h"
#include "Utility/LogFunctions.h"
#include "Utility/Logger.h"
#include <math.h>
#include <iostream>

extern Logger logger;

void TestKinematics(State& qdot, const Agent &self, const Control & u, const double& simulDeltaT)
{	
		const double maxTheta = 0.4;
			
		const State& q0 = self.GetState();
		
		qdot("x") = q0("v")*cos(q0("theta"));	
		qdot("y") = q0("v")*sin(q0("theta"));
		
		qdot("theta") = u("omega");
		
		double nextTheta = q0("theta") + qdot("theta")*simulDeltaT;
		
		if (nextTheta > maxTheta)
			qdot("theta") = (maxTheta - q0("theta"))/simulDeltaT;
		else if (nextTheta < -maxTheta)
			qdot("theta") = (-maxTheta - q0("theta"))/simulDeltaT;
		
		
		qdot("v") = u("a");
}

#endif
