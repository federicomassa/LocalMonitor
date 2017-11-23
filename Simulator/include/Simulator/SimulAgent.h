#ifndef SIMUL_AGENT_H
#define SIMUL_AGENT_H

#include "Basic/Agent.h"
#include "Basic/Sensing.h"
#include "Automation/DynamicModel.h"
#include "Automation/AutomationModel.h"
#include "Automation/PhysicalLayer.h"
#include "Automation/Controller.h"
#include "Automation/Automaton.h"


#include <string>
#include <map>

class Logger;


class SimulAgent
{
    friend class SimulatorConfiguration;

    Agent agent;
    PhysicalLayer pLayer;
	Controller controller;
	Automaton automaton;
	
	AutomationModel automationModel;
	//Sensing sensing;
	void SetDynamicFunction ( const std::string & );
public:
    SimulAgent();
	SimulAgent(const SimulAgent&);
    const std::string &GetID() const;
    void SetID ( const std::string & );
    const State &GetState() const;
	const DynamicModel& GetDynamicModel() const;
    void SetState ( const State & );
	void SetPossibleManeuvers(const ManeuverList&);
	void SetDynamicModel(const DynamicModel&);
	void SetParameters(const AgentParameters&);
	bool SetManeuver(const ManeuverName&);
    void EvolveState();
    friend Logger &operator<< ( Logger &, const SimulAgent & );
};

typedef std::map<std::string, SimulAgent> SimulAgentVector;

Logger &operator<< ( Logger &, const SimulAgent & );

#endif
