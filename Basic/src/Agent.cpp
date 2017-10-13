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

void Agent::SetID(const string& str)
{
	ID = str;
}

const State &Agent::GetState() const
{
    return state;
}

void Agent::SetState(const State& q)
{
	state = q;
}

Logger &operator<<(Logger &os, const Agent &a)
{
    os << "Agent " << a.ID << os.EndL(Logger::INC);
    os << "{" << os.EndL(Logger::INC);
    os << a.state << os.EndL(Logger::DEC);
    os << "}" << os.EndL(Logger::DEC);

    return os;
}


