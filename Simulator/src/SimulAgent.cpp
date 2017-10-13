#include "SimulAgent.h"
#include "Utility/LogFunctions.h"
#include "Utility/Logger.h"

#include <iostream>

using namespace std;

extern Logger logger;

SimulAgent::SimulAgent()
{
}

const string &SimulAgent::GetID() const
{
    return agent.GetID();
}

void SimulAgent::SetID(const string& str)
{
	agent.SetID(str);
}
const State &SimulAgent::GetState() const
{
    return agent.GetState();
}

void SimulAgent::SetState(const State& q)
{
	agent.SetState(q);
}

Logger &operator<<(Logger &os, const SimulAgent &a)
{
    os << "Agent " << a.GetID() << os.EndL(Logger::INC);
    os << "{" << os.EndL(Logger::INC);
    os << a.GetState() << os.EndL(Logger::DEC);
    os << "}" << os.EndL(Logger::DEC);

    return os;
}

void SimulAgent::EvolveState()
{
	
}

