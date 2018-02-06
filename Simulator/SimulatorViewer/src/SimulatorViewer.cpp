// this has to go first
#include "SimulatorBuildParams.h" 
#include "SimulatorConfiguration.h"

#include <iostream>

#include "SimulatorViewer.h"

#include "Utility/LogFunctions.h"

#include <iostream>
using namespace std;
using namespace LogFunctions;

SimulatorViewer::SimulatorViewer(const SimulatorConfiguration& c) : QMainWindow(nullptr), conf(c)
{
}

const SimulatorConfiguration& SimulatorViewer::GetSimulatorConfiguration() const
{
	return conf;
}

SimulatorViewer::~SimulatorViewer()
{
}

void SimulatorViewer::SetProperty(const std::string& propertyName, const std::string& propertyValue)
{
	properties[propertyName] = propertyValue;
}

std::string SimulatorViewer::GetProperty(const std::string& propertyName) const
{
	std::string returnValue;

	try
		{
			returnValue = properties.at(propertyName);
		}
		catch(std::out_of_range& e)
		{
			Error("SimulatorViewer::GetProperty", "Property " + propertyName + " was not set");
		}
		
	return returnValue;	
}

void SimulatorViewer::DrawStaticEnvironment()
{
}

void SimulatorViewer::DrawDynamicEnvironment(const SimulAgentVector&, const double&)
{
}

void SimulatorViewer::Encode()
{
}


