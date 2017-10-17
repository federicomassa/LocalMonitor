#include "SimulAgent.h"
#include "SimulatorConfiguration.h"
#include "Utility/LogFunctions.h"
#include "Utility/Logger.h"

// It's important to include SimulatorBuildParams.h first
#include "SimulatorBuildParams.h"
#include KINEMATICS_FUNC_LIST

#include <iostream>

using namespace std;

extern Logger logger;
extern SimulatorConfiguration conf;

SimulAgent::SimulAgent() : pLayer(conf.GetSimulationTimeStep())
{
}

SimulAgent::SimulAgent(const SimulAgent& a) : pLayer(a.pLayer)
{
	agent = a.agent;
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

void SimulAgent::SetKinematics(const string& kinematicsFcnName)
{
    if (kinematicsFcnName == "TestKinematics")
    {
        pLayer.SetKinematics(&TestKinematics);
    }

}

Logger &operator<<(Logger &os, const SimulAgent &a)
{
    os << "Agent " << a.GetID() << os.EndL(Logger::INC);
    os << " {" << os.EndL(Logger::INC);
	os << a.GetState() << os.EndL(Logger::DEC);
	os << "}" << os.EndL(Logger::DEC);
    return os;
}



void SimulAgent::EvolveState()
{
    agent.SetState(pLayer.GetNextState(agent.GetState()));
}

