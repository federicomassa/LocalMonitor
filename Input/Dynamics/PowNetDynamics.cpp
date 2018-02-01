#include "PowNetDynamics.h"
#include <math.h>

void PowNetDynamics(State& qdot, const Agent &self, const AgentVector& trueOthersInWorld, const Control & u, const double& simulDeltaT)
{	
	const State& q0 = self.GetState();
		
	double a = 0.625;
	double c = 52.2556;
	double Z = 51.2579;
	double alpha = 0.113;
	
	qdot("delta") = q0("omega");
	
	double sinSum = 0;
	
	for (auto itr = trueOthersInWorld.begin(); itr != trueOthersInWorld.end(); itr++)
		sinSum += itr->second("V")*Z*sin(q0("delta") - itr->second("delta") + alpha);
	
	qdot("omega") = -a*q0("omega") + c - q0("V")*sinSum;
	
	qdot("V") = u("u1");
	qdot("xc") = u("u2");
}
	
