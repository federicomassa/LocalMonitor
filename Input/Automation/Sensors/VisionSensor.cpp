#include "VisionSensor.h"
#include "Utility/LogFunctions.h"
#include "Utility/SimulationParameters.h"

#include <math.h>

using namespace std;
using namespace LogFunctions;

VisionSensor::VisionSensor()
{
	AddAgentMeasuredVariable("xb");
	AddAgentMeasuredVariable("xf");
	AddAgentMeasuredVariable("yb");
	AddAgentMeasuredVariable("yf");
	AddAgentMeasuredVariable("length");
	
	AddEnvironmentMeasuredVariable("lanes");
	AddEnvironmentMeasuredVariable("lane_width");
}

void VisionSensor::SimulateVisibility(StateRegion& visible, std::set<std::string>& visibleIDs, const Agent& selfInWorld, const AgentVector& othersInWorld) const
{
	visible.Purge();
	visibleIDs.insert("0");
}

void VisionSensor::SimulateOutput(ExternalSensorOutput& outputInWorld, const Agent& selfInWorld, const AgentVector& othersInWorld, const EnvironmentParameters& envParam) const
{
	for (auto itr = othersInWorld.begin(); itr != othersInWorld.end(); itr++)
	{
		const State& neigh = itr->second.GetState();
		outputInWorld.AgentID(itr->first)("xb") = neigh("xb");
		outputInWorld.AgentID(itr->first)("xf") = neigh("xf");
		outputInWorld.AgentID(itr->first)("yb") = neigh("yb");
		outputInWorld.AgentID(itr->first)("yf") = neigh("yf");
		
		outputInWorld.AgentID(itr->first)("length") = itr->second.GetParameter("length");
	}
	
	outputInWorld.Environment("lanes") = envParam("lanes");
		
}
