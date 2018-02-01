#ifndef POWNET_TO_STATE_H
#define POWNET_TO_STATE_H

#include "Basic/Agent.h"
#include <math.h>
#include <iostream>


void PowNetToState(Agent& localState, const Agent& agent)
{
	localState.SetID(agent.GetID());
	
	localState("delta") = agent("delta");
	localState("omega") = agent("omega");
	localState("V") = agent("V");
	localState("xc") = agent("xc");
}

#endif
