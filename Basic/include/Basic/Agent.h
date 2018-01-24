#ifndef AGENT_H
#define AGENT_H

#include <map>
#include <utility>
#include <string>
#include <fstream>

#include "State.h"
#include "Maneuver.h"
#include "Utility/AgentParameters.h"


class MyLogger;

class Agent
{
	friend class ExternalSensorOutput;
    std::string ID;
    State state;
	AgentParameters parameters;
public:
    Agent();
	Agent(const Agent&);
    const std::string &GetID() const;
    void SetID ( const std::string & );
    const State &GetState() const;
    void SetState ( const State &q );
	void SetParameters(const AgentParameters&);
	const double& GetParameter(const std::string&) const;
	const AgentParameters& GetParameters() const;
	// State variable accessors
	double& operator()(const std::string&);
	const double& operator()(const std::string&) const;
	Agent& operator=(const Agent&);
	
    friend MyLogger &operator<< ( MyLogger &, const Agent & );
};

typedef std::map<std::string, Agent> AgentVector;

MyLogger &operator<< ( MyLogger &, const Agent & );


#endif
