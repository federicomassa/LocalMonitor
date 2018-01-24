#include "Agent.h"
#include "Utility/LogFunctions.h"
#include "Utility/MyLogger.h"

#include <iostream>

using namespace std;


Agent::Agent()
{
}

Agent::Agent(const Agent& a)
{
	*this = a;
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

const AgentParameters & Agent::GetParameters() const
{
	return parameters;
}


MyLogger &operator<<(MyLogger &os, const Agent &a)
{
    os << "Agent " << a.ID << os.EndL(MyLogger::INC);
    os << "{" << os.EndL(MyLogger::INC);
    os << a.state << os.EndL();
	os << a.parameters << os.EndL(MyLogger::DEC);
	os << "}" << os.EndL(MyLogger::DEC);

    return os;
}


