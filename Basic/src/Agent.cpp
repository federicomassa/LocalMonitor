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

const Maneuver& Agent::GetManeuver() const
{
	return maneuver;
}

void Agent::SetState(const State &q)
{
    state = q;
}

void Agent::SetPossibleManeuvers(const ManeuverList& manList)
{
	possibleManeuvers = manList;
}

bool Agent::SetManeuver(const ManeuverName& manName)
{
	if (possibleManeuvers.find(manName) != possibleManeuvers.end())
	{
		maneuver = manName;
		return true;
	}
	
	return false;
		
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


