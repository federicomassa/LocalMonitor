#include "PhysicalLayer.h"
#include "Utility/MyLogger.h"
#include "Utility/LogFunctions.h"

#include <map>
#include <cstdlib>
#include <string>

using namespace LogFunctions;
using namespace std;

PhysicalLayer::PhysicalLayer()
{
	logger = nullptr;
	simulTimeStep = -1;
}


PhysicalLayer::PhysicalLayer(const double& simulDeltaT, MyLogger* log) : simulTimeStep(simulDeltaT)
{
	logger = log;
}

PhysicalLayer::PhysicalLayer(const PhysicalLayer& pL) : simulTimeStep(pL.simulTimeStep)
{
	dynamicModel = pL.dynamicModel;
	logger = pL.logger;
}

PhysicalLayer& PhysicalLayer::operator=(const PhysicalLayer& pL)
{
	dynamicModel = pL.dynamicModel;
	simulTimeStep = pL.simulTimeStep;
	logger = pL.logger;
	
	return *this;
}


const double & PhysicalLayer::GetSimulationTimeStep() const
{
	return simulTimeStep;
}

void PhysicalLayer::SetSimulationTimeStep(const double& deltaT)
{
	simulTimeStep = deltaT;
}

State PhysicalLayer::GetNextState(const Agent & currentAgent, const AgentVector& trueOthersInWorld, const Control& control) const
{
	if (simulTimeStep < 0)
		Error("PhysicalLayer::GetNextState", "Simulation time step should be > 0");
	
	State dCurrentState = State::GenerateStateOfType(currentAgent.GetState());
	
	if (dynamicModel.IsSet())
	{
		try
		{
			dynamicModel.Run(dCurrentState, currentAgent, trueOthersInWorld, control, simulTimeStep);
		}
		catch (std::out_of_range& e)
		{
			Error("PhysicaLLayer::GetNextState", string("Out of range exception thrown in kinematics function ") + dynamicModel.GetDynamicsFunctionName());
			exit(1);
		}
	}
	
	CheckConsistency("PhysicalLayer::GetNextState", dCurrentState, currentAgent.GetState());
	
	State nextState =  currentAgent.GetState() + dCurrentState*simulTimeStep;
	return nextState;
}

void PhysicalLayer::SetDynamicModel(const DynamicModel& m)
{
    dynamicModel = m;	
}

const DynamicModel& PhysicalLayer::GetDynamicModel() const
{
	return dynamicModel;
}
