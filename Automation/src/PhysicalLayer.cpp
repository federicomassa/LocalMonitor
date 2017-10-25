#include "PhysicalLayer.h"
#include "Utility/Logger.h"
#include "Utility/LogFunctions.h"

#include <map>
#include <cstdlib>

extern Logger logger;
using namespace LogFunctions;

PhysicalLayer::PhysicalLayer(const double& simulDeltaT) : simulTimeStep(simulDeltaT)
{
    kinematics = 0;
}

PhysicalLayer::PhysicalLayer(const PhysicalLayer& pL) : simulTimeStep(pL.simulTimeStep)
{
	kinematics = pL.kinematics;
}

const double & PhysicalLayer::GetSimulationTimeStep() const
{
	return simulTimeStep;
}

State PhysicalLayer::GetNextState(const State & currentState, const Maneuver& man) const
{
	State dCurrentState = State::GenerateStateOfType(currentState);
	
	if (kinematics)
	{
		try
		{
			kinematics(dCurrentState, currentState, man);
		}
		catch (std::out_of_range& e)
		{
			Error("PhysicaLLayer::GetNextState", "Out of range exception thrown in kinematics function");
			exit(1);
		}
	}
	
	CheckConsistency("PhysicalLayer::GetNextState", dCurrentState, currentState);
	
	State nextState =  currentState + dCurrentState*simulTimeStep;
	return nextState;
}

void PhysicalLayer::SetKinematics(KinematicsFcn kin)
{
    kinematics = kin;
}
