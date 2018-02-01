#include "PowerNetworksViewer.h"
#include "Simulator/SimulatorConfiguration.h"
#include "json.hpp"

using namespace std;

PowerNetworksViewer::PowerNetworksViewer(const SimulatorConfiguration& conf) : SimulatorViewer(conf), config(&conf)
{
	string asciiDir;
	
	try
	{
		asciiDir = conf.GetEntry("ascii_output_dir").get<string>();
	}
	catch(out_of_range&)
	{
		LogFunctions::Error("PowerNetworksViewer::PowerNetworksViewer", "\'ascii_output_dir\' is needed in simulator configuration file.");
	}
	
	for (auto itr = conf.GetAgents().begin(); itr != conf.GetAgents().end(); itr++)
	{
		output[itr->first] = new ofstream((asciiDir + "PowerNetworksLog" + itr->first + ".txt").c_str());
		
		if (output(itr->first)->is_open())
		{
			logger[itr->first].SetOutput(output(itr->first));
		}
		else
			LogFunctions::Error("PowerNetworksViewer::PowerNetworksViewer", "Error opening file in \'ascii_output_dir\'");
	}
	
	variables = conf.GetWorldAgentFeatures();
	
	for (auto agent = conf.GetAgents().begin(); agent != conf.GetAgents().end(); agent++)
	{
		for (auto itr = variables.begin(); itr != variables.end(); itr++)
		{
			logger[agent->first] << *itr;
			
			if (itr != --variables.end())
				logger[agent->first] << '\t';
		}
		
		logger[agent->first] << logger[agent->first].EndL();
	}
}

PowerNetworksViewer::~PowerNetworksViewer()
{
	for (auto itr = output.begin(); itr != output.end(); itr++)
	{
		delete itr->second;
	}
}


void PowerNetworksViewer::DrawStaticEnvironment()
{
}

void PowerNetworksViewer::DrawDynamicEnvironment(const SimulAgentVector& agents)
{
	for (auto itr = agents.begin(); itr != agents.end(); itr++)
	{
		logger(itr->first) << "Ciao agente " << itr->first << "!" << logger(itr->first).EndL();
	}
}
