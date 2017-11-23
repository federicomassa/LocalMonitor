#ifndef SENSING_H
#define SENSING_H

#include "Sensors.h"

// TODO define what sensors can the agent have and how they filter the real state
class Sensing
{
public:
	SensedSelf self;
	std::map<std::string, SensedAgent> agents;
	SensedEnvironment environment;	
};

#endif
