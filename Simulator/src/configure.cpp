/*
 * This program generates code from the Simulator config file (Simulator.json)
 * 
 * */

#include "Utility/LogFunctions.h"
#include "Utility/Logger.h"
#include "SimulatorBuildParams.h"
#include "json.hpp"
#include <iostream>
#include <fstream>
#include <string>

using namespace nlohmann;
using namespace LogFunctions;
using namespace std;
Logger logger(std::cout);


int main(int argc, char** argv)
{
	if (argc != 3)
		Error("Usage", "./configure <path/to/Simulator.json> <path/to/project root dir> without slashes at the end of dir");
	
	const char* configFilePath = argv[1];
	const char* rootPath = argv[2];
	
	ifstream configFile(configFilePath);
	if (!configFile.is_open())
		Error("configure::main", string("Cannot open ") + configFilePath);
	
	ofstream outputConfig((string(rootPath) + "/Simulator/SET_SIMULATOR_VARIABLES.in").c_str());
	
	json j;
	configFile >> j;
	
	try 
	{
		string chosenSimulatorViewer = j.at("simulator_viewer").get<string>();
		outputConfig << "set (CHOSEN_SIMULATOR_VIEWER " << chosenSimulatorViewer << ")\n";
		
		string videoOutputName;
		try
		{
			videoOutputName = j.at("video_output").get<string>();
			outputConfig << "set (VIDEO_OUTPUT " << videoOutputName << ")\n";
		}
		catch(out_of_range& e)
		{
			//Default value 
			outputConfig << "set (VIDEO_OUTPUT simulation.avi)\n";
		}
		
		
		
	}
	catch (out_of_range& e)
	{
		Error("configure::main", (string("simulator_viewer was not set in ") + configFilePath).c_str());
	}
	
	return 0;	
}
