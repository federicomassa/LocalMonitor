#ifndef SIMUL_AGENT_H
#define SIMUL_AGENT_H

#include "Basic/Agent.h"
#include "Automation/PhysicalLayer.h"

#include <string>

class Logger;

class SimulAgent
{
    friend class SimulatorConfiguration;

    Agent agent;
    PhysicalLayer pLayer;
public:
    SimulAgent();
    const std::string &GetID() const;
	void SetID(const std::string&);
    const State &GetState() const;
	void SetState(const State&);
	void EvolveState();
    friend Logger &operator<< ( Logger &, const SimulAgent & );
};

typedef std::vector<SimulAgent> SimulAgentVector;

Logger &operator<< ( Logger &, const SimulAgent & );

#endif
