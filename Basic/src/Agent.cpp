#include "Agent.h"
#include "Utility/LogFunctions.h"
#include "Utility/Logger.h"

#include <iostream>

using namespace std;


Agent::Agent()
{
}

const string &Agent::GetID() const
{
    return ID;
}

void Agent::SetID(const string &str)
{
    ID = str;
}

const State &Agent::GetState() const
{
    return state;
}

double& Agent::operator()(const string& var)
{
	return state(var);
}

const double& Agent::operator()(const string& var) const
{
	return state(var);
}

Agent& Agent::operator=(const Agent& a)
{
	ID = a.ID;
	state = a.state;
	parameters = a.parameters;
	
	return *this;
}


void Agent::SetState(const State &q)
{
    state = q;
}



void Agent::SetParameters(const AgentParameters& pars)
{
	parameters = pars;
}

const double& Agent::GetParameter(const string& parName) const
{
	try
	{
		return parameters.at(parName);
	}
	catch (out_of_range&)
	{
		LogFunctions::Error("Agent::GetParameter", string(parName) + " in agent " + ID + " was not set");
	}
	
	// Added to avoid warning, but shouldn't get here
	return parameters.at("");
}

Logger &operator<<(Logger &os, const Agent &a)
{
    os << "Agent " << a.ID << os.EndL(Logger::INC);
    os << "{" << os.EndL(Logger::INC);
    os << a.state << os.EndL();
	os << "Possible maneuvers: " << os.EndL(Logger::INC);
	os << a.possibleManeuvers << os.EndL(Logger::DEC);
	os << a.maneuver << os.EndL(Logger::DEC);
    os << "}" << os.EndL(Logger::DEC);

    return os;
}


