#include "PhysicalLayer.h"
#include "Utility/Logger.h"
#include "Utility/LogFunctions.h"

#include <map>
#include <cstdlib>
#include <string>

extern Logger logger;
using namespace LogFunctions;
using namespace std;

PhysicalLayer::PhysicalLayer(const double& simulDeltaT) : simulTimeStep(simulDeltaT)
{
}

PhysicalLayer::PhysicalLayer(const PhysicalLayer& pL) : simulTimeStep(pL.simulTimeStep)
{
	dynamicModel = pL.dynamicModel;
}

PhysicalLayer& PhysicalLayer::operator=(const PhysicalLayer& pL)
{
	dynamicModel = pL.dynamicModel;
}


const double & PhysicalLayer::GetSimulationTimeStep() const
{
	return simulTimeStep;
}

State PhysicalLayer::GetNextState(const Agent & currentAgent, const Control& control) const
{
	State dCurrentState = State::GenerateStateOfType(currentAgent.GetState());
	
	if (dynamicModel.IsSet())
	{
		try
		{
			dynamicModel.Run(dCurrentState, currentAgent, control, simulTimeStep);
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
