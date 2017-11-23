#ifndef AGENT_H
#define AGENT_H

#include <map>
#include <utility>
#include <string>
#include <fstream>

#include "State.h"
#include "Maneuver.h"
#include "AgentParameters.h"


class Logger;


class Agent
{
    std::string ID;
    State state;
	Maneuver maneuver;
	AgentParameters parameters;
	ManeuverList possibleManeuvers;
public:
    Agent();
    const std::string &GetID() const;
    void SetID ( const std::string & );
    const State &GetState() const;
	const Maneuver &GetManeuver() const;
    void SetState ( const State &q );
	void SetPossibleManeuvers(const ManeuverList&);
		/**
	 * @brief Returns false if maneuver was not among the possible maneuvers (manList)
	 */
	bool SetManeuver(const ManeuverName&);
	void SetParameters(const AgentParameters&);
	const std::string& GetParameter(const std::string&) const;
	// State variable accessors
	double& operator()(const std::string&);
	const double& operator()(const std::string&) const;
	
    friend Logger &operator<< ( Logger &, const Agent & );
};

typedef std::map<std::string, Agent> AgentVector;

Logger &operator<< ( Logger &, const Agent & );


#endif
