#ifndef POWNET_TO_WORLD_H
#define POWNET_TO_WORLD_H

#include "Basic/Agent.h"
#include <math.h>
#include <iostream>


void PowNetToWorld(Agent& worldState, const Agent& agent)
{
	worldState.SetID(agent.GetID());
	
	worldState("delta") = agent("delta");
	worldState("omega") = agent("omega");
	worldState("V") = agent("V");
	worldState("xc") = agent("xc");
}

#endif
