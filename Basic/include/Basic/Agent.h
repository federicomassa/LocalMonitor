#ifndef AGENT_H
#define AGENT_H

#include <map>
#include <utility>
#include <string>
#include <fstream>

#include "State.h"
#include "Maneuver.h"
#include "Utility/AgentParameters.h"


class Logger;

class Agent
{
	friend class ExternalSensorOutput;
    std::string ID;
    State state;
	AgentParameters parameters;
public:
    Agent();
    const std::string &GetID() const;
    void SetID ( const std::string & );
    const State &GetState() const;
    void SetState ( const State &q );
	void SetParameters(const AgentParameters&);
	const double& GetParameter(const std::string&) const;
	// State variable accessors
	double& operator()(const std::string&);
	const double& operator()(const std::string&) const;
	Agent& operator=(const Agent&);
	
    friend Logger &operator<< ( Logger &, const Agent & );
};

typedef std::map<std::string, Agent> AgentVector;

Logger &operator<< ( Logger &, const Agent & );


#endif
