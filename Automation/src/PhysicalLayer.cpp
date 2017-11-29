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
	simulTimeStep = pL.simulTimeStep;
	dynamicModel = pL.dynamicModel;
}


const double & PhysicalLayer::GetSimulationTimeStep() const
{
	return simulTimeStep;
}

State PhysicalLayer::GetNextState(const State & currentState, const Maneuver& man) const
{
	State dCurrentState = State::GenerateStateOfType(currentState);
	
	if (dynamicModel.isSet())
	{
		try
		{
			dynamicModel.Run(dCurrentState, currentState, man);
		}
		catch (std::out_of_range& e)
		{
			Error("PhysicaLLayer::GetNextState", string("Out of range exception thrown in kinematics function ") + dynamicModel.GetDynamicFunctionName());
			exit(1);
		}
	}
	
	CheckConsistency("PhysicalLayer::GetNextState", dCurrentState, currentState);
	
	State nextState =  currentState + dCurrentState*simulTimeStep;
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
