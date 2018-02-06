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
			logger(itr->first).SetPrecision(15);
		}
		else
			LogFunctions::Error("PowerNetworksViewer::PowerNetworksViewer", "Error opening file in \'ascii_output_dir\'");
	}
	
	variables = conf.GetWorldAgentFeatures();
	
	for (auto itr = conf.GetAgents().begin(); itr != conf.GetAgents().end(); itr++)
	{
		
		logger[itr->first] << "Timestamp" << '\t';
		
		for (auto var = variables.begin(); var != variables.end(); var++)
		{
			logger[itr->first] << *var << '\t';
		}
		
		for (auto var = itr->second.GetController()->GetControlModel().GetControlVariables().begin(); 
			 var != itr->second.GetController()->GetControlModel().GetControlVariables().end(); var++)
		{
			logger[itr->first] << *var << '\t';
		}
		
		for (auto man = itr->second.GetAutomaton()->GetPossibleManeuvers().begin();
				man != itr->second.GetAutomaton()->GetPossibleManeuvers().end(); man++)
		{
			logger(itr->first) << man->GetName();
			
			if (man != --itr->second.GetAutomaton()->GetPossibleManeuvers().end())
				logger(itr->first) << '\t';
		}
		
		
		logger[itr->first] << logger[itr->first].EndL();
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

void PowerNetworksViewer::DrawDynamicEnvironment(const SimulAgentVector& agents, const double& currentTime)
{
	for (auto itr = agents.begin(); itr != agents.end(); itr++)
	{
		logger(itr->first) << currentTime << '\t';
		
		for (auto var = variables.begin(); var != variables.end(); var++)
		{
			logger(itr->first) << itr->second.GetWorldState()(*var) << '\t';
		}
		
		// WARNING given the structure, we cannot write control at first instant because control is evaluated after draw. So first raw will be zero, and the 
		// control evaluated at time 0 it's displayed at time 1.
		for (auto var = itr->second.GetController()->GetControlModel().GetControlVariables().begin(); 
			 var != itr->second.GetController()->GetControlModel().GetControlVariables().end(); var++)
		{
			try
			{
				logger(itr->first) << itr->second.GetController()->GetLastControl()(*var) << '\t';
			}
			catch(out_of_range&)
			{
				logger(itr->first) << 0.0 << '\t';
			}
		}
		
		
		for (auto man = itr->second.GetAutomaton()->GetPossibleManeuvers().begin();
				man != itr->second.GetAutomaton()->GetPossibleManeuvers().end(); man++)
				{
					logger(itr->first) << (int)(itr->second.GetManeuver() == *man);
					if (man != --itr->second.GetAutomaton()->GetPossibleManeuvers().end())
						logger(itr->first) << '\t';
				}
		
		logger(itr->first) << logger(itr->first).EndL();
	}
}
