#include "PowNetExternalSensor.h"
#include "Utility/LogFunctions.h"
#include "Utility/SimulationParameters.h"

#include <math.h>

using namespace std;
using namespace LogFunctions;

PowNetExternalSensor::PowNetExternalSensor()
{
	AddAgentMeasuredVariable("delta");
	AddAgentMeasuredVariable("omega");
	AddAgentMeasuredVariable("V");
	AddAgentMeasuredVariable("xc");
}

void PowNetExternalSensor::SimulateVisibility(StateRegion& visible, std::set<std::string>& visibleIDs, const Agent& selfInWorld, const AgentVector& othersInWorld) const
{
	visible.Purge();
	for (auto itr = othersInWorld.begin(); itr != othersInWorld.end(); itr++)
		visibleIDs.insert(itr->first);
}

void PowNetExternalSensor::SimulateOutput(ExternalSensorOutput& outputInWorld, const Agent& selfInWorld, const AgentVector& othersInWorld, const EnvironmentParameters& envParam) const
{
	for (auto itr = othersInWorld.begin(); itr != othersInWorld.end(); itr++)
	{
		const State& neigh = itr->second.GetState();
		outputInWorld.AgentID(itr->first)("delta") = neigh("delta");
		outputInWorld.AgentID(itr->first)("omega") = neigh("omega");
		outputInWorld.AgentID(itr->first)("V") = neigh("V");
		outputInWorld.AgentID(itr->first)("xc") = neigh("xc");
	}
}
