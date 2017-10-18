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

void SimulAgent::SetPossibleManeuvers(const ManeuverList& manList)
{
	agent.SetPossibleManeuvers(manList);
}

bool SimulAgent::SetManeuver(const ManeuverName& manName)
{
	return agent.SetManeuver(manName);
}

void SimulAgent::SetKinematics(const string& kinematicsFcnName)
{
	// TODO Make function identification automatic
    if (kinematicsFcnName == "TestKinematics")
    {
        pLayer.SetKinematics(&TestKinematics);
    }

}

Logger &operator<<(Logger &os, const SimulAgent &a)
{
    os << a.agent;
    return os;
}



void SimulAgent::EvolveState()
{
    agent.SetState(pLayer.GetNextState(agent.GetState(), agent.GetManeuver()));
}

