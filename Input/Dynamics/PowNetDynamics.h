#ifndef POWNET_DYNAMICS_H
#define POWNET_DYNAMICS_H

#include "Automation/Control.h"
#include "Basic/Agent.h"
#include "Utility/LogFunctions.h"
#include "Utility/MyLogger.h"
#include <math.h>
#include <iostream>

void PowNetDynamics(State& qdot, const Agent &self, const AgentVector& others, const Control & u, const double& simulDeltaT);	


#endif
