#include "SelfSensor.h"
#include "Utility/LogFunctions.h"
#include "Utility/SimulationParameters.h"

#include <math.h>

using namespace std;
using namespace LogFunctions;

SelfSensor::SelfSensor()
{
	AddSelfMeasuredVariable("xb");
	AddSelfMeasuredVariable("yb");
	AddSelfMeasuredVariable("xf");
	AddSelfMeasuredVariable("yf");
	AddSelfMeasuredVariable("v");
	AddSelfMeasuredVariable("desiredV");
	AddSelfMeasuredVariable("length");
}



void SelfSensor::SimulateOutput(InternalSensorOutput& outputInWorld, const Agent& selfInWorld) const
{
	outputInWorld("xb") = selfInWorld("xb");
	outputInWorld("yb") = selfInWorld("yb");
	outputInWorld("xf") = selfInWorld("xf");
	outputInWorld("yf") = selfInWorld("yf");
	outputInWorld("v") = selfInWorld("v");
	outputInWorld("desiredV") = selfInWorld("desiredV");
	outputInWorld("length") = selfInWorld("length");
	
}
