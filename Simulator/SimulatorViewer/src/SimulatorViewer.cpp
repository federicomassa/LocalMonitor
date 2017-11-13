// this has to go first
#include "SimulatorBuildParams.h" 
#include "SimulatorConfiguration.h"

#include <iostream>

#include "SimulatorViewer.h"
#include CHOSEN_VIEWER_INCLUDE

#include "Utility/LogFunctions.h"

#include <iostream>
using namespace std;
using namespace LogFunctions;

SimulatorViewer::SimulatorViewer(const SimulatorConfiguration& c, SimulatorViewer* _parent) : conf(c)
{
	parent = _parent;
	
	if (parent == nullptr)
	{
		object = nullptr;
		properties = nullptr;
	}
	else
	{
		object = parent->object;
		properties = parent->properties;
	}
}

SimulatorViewer::~SimulatorViewer()
{	
	// Parent object has ownership
	if (parent == nullptr)
	{
		if (object)
			delete object;
	
		if (properties)
			delete properties;
	}
}
	
void SimulatorViewer::Initialize(int& argc, char** argv)
{	
	if (parent)
		Error("SimulatorViewer::Initialize", "Cannot call from child object");
	
	properties = new Properties;
	
	if(object)
	{
		delete object;
		object = nullptr;
	}
	
#ifdef CHOSEN_VIEWER
	object = new CHOSEN_VIEWER(conf, argc, argv, this);
#else
	object = nullptr;
	#error "simulator_viewer was not set in config file"
#endif
}

const SimulatorConfiguration& SimulatorViewer::GetSimulatorConfiguration() const
{
	return conf;
}

void SimulatorViewer::DrawStaticEnvironment()
{
	if (object)
		object->DrawStaticEnvironment();
}

void SimulatorViewer::DrawDynamicEnvironment(const SimulAgentVector& v)
{
	if (object)
		object->DrawDynamicEnvironment(v);
}

void SimulatorViewer::Encode()
{
	if (object)
		object->Encode();
}

void SimulatorViewer::SetProperty(const std::string& propertyName, const std::string& propertyValue)
{
	if (properties)
	{	
		(*properties)[propertyName] = propertyValue;
	}
	else
		Error("SimulatorViewer::SetProperty", "Call SimulatorViewer::Initialize first");
}

std::string SimulatorViewer::GetProperty(const std::string& propertyName) const
{
	std::string returnValue;
	
	if (properties)
	{
		try
		{
			returnValue = properties->at(propertyName);
		}
		catch(std::out_of_range& e)
		{
			Error("SimulatorViewer::GetProperty", "Property " + propertyName + " was not set");
		}
	}
	else
		Error("SimulatorViewer::GetProperty", "Call SimulatorViewer::Initialize first");
	
	return returnValue;	
}
