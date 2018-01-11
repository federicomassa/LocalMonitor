#ifndef TEST_KINEMATICS_H
#define TEST_KINEMATICS_H

#include "Automation/Control.h"
#include "Basic/Agent.h"
#include "Utility/LogFunctions.h"
#include "Utility/MyLogger.h"
#include <math.h>
#include <iostream>

void TestKinematics(State& qdot, const Agent &self, const Control & u, const double& simulDeltaT)
{	
		// Saturation
		const double maxA = 3;
		const double maxOmega = 2;
		const double maxV = 33;
		const double maxTheta = 0.4;
			
		MyLogger logger(std::cout);
		
		std::cout << "!!!" << std::endl;
		logger << qdot << logger.EndL();
		
		std::cout << "0" << std::endl;
		
		const State& q0 = self.GetState();
		//logger << q0.Size() << logger.EndL();
		
		qdot("x") = q0("v")*cos(q0("theta"));	
		qdot("y") = q0("v")*sin(q0("theta"));
		
		std::cout << "0" << std::endl;
		
		if (fabs(u("omega")) > maxOmega)
			qdot("theta") = maxOmega*fabs(u("omega"))/u("omega");
		else
			qdot("theta") = maxOmega;
		
		std::cout << "0" << std::endl;
		double nextTheta = q0("theta") + qdot("theta")*simulDeltaT;
		
		std::cout << "0" << std::endl;
		if (nextTheta > maxTheta)
			qdot("theta") = (maxTheta - q0("theta"))/simulDeltaT;
		else if (nextTheta < -maxTheta)
			qdot("theta") = (-maxTheta - q0("theta"))/simulDeltaT;
		
		std::cout << "0" << std::endl;
		if (fabs(u("a")) > maxA)
			qdot("v") = maxA*fabs(u("a"))/u("a");
		else
			qdot("v") = u("a");
		
		std::cout << "0" << std::endl;
		
		double nextV = q0("v") + qdot("v")*simulDeltaT;
		if (nextV > maxV)
			qdot("v") = (maxV - q0("v"))/simulDeltaT;
		else if (nextV < 0)
			qdot("v") = (0 - q0("v"))/simulDeltaT;
}
		


#endif
