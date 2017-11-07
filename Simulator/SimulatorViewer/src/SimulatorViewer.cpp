// this has to go first
#include "SimulatorBuildParams.h" 
#include "SimulatorConfiguration.h"

#include <iostream>

#include "SimulatorViewer.h"
#include CHOSEN_VIEWER_INCLUDE

#include <iostream>
using namespace std;

SimulatorViewer::SimulatorViewer(const SimulatorConfiguration& c) : conf(c)
{
	object = nullptr;
}

SimulatorViewer::~SimulatorViewer()
{	
	if (object)
		delete object;
}
	
void SimulatorViewer::Initialize(int& argc, char** argv)
{	
	if(object)
	{
		delete object;
		object = nullptr;
	}
	
#ifdef CHOSEN_VIEWER
	object = new CHOSEN_VIEWER(conf, argc, argv);
#else
	object = nullptr;
	#error "simulator_viewer was not set in config file"
#endif
}

void SimulatorViewer::SetOutputFileName(const std::string& name)
{
	outputFileName = name;
}

const SimulatorConfiguration& SimulatorViewer::GetSimulatorConfiguration() const
{
	return conf;
}

void SimulatorViewer::DrawEnvironment()
{
	if (object)
		object->DrawEnvironment();
}

