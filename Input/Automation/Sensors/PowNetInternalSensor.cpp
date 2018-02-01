#include "PowNetInternalSensor.h"
#include "Utility/LogFunctions.h"
#include "Utility/SimulationParameters.h"

#include <math.h>

using namespace std;
using namespace LogFunctions;

PowNetInternalSensor::PowNetInternalSensor()
{
	AddSelfMeasuredVariable("delta");
	AddSelfMeasuredVariable("omega");
	AddSelfMeasuredVariable("V");
	AddSelfMeasuredVariable("xc");
}



void PowNetInternalSensor::SimulateOutput(InternalSensorOutput& outputInWorld, const Agent& selfInWorld) const
{
	outputInWorld("delta") = selfInWorld("delta");
	outputInWorld("omega") = selfInWorld("omega");
	outputInWorld("V") = selfInWorld("V");
	outputInWorld("xc") = selfInWorld("xc");
}
