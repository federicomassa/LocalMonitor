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

void generateDynamicFunctionsFile(const nlohmann::json& j, ostream& outputModels);
void generateStateConversionFile(const nlohmann::json& j, ostream& os);

int main(int argc, char** argv)
{
	if (argc != 3)
		Error("Usage", "./configure <path/to/Simulator.json> <path/to/project root dir> without slashes at the end of dir");
	
	const char* configFilePath = argv[1];
	const char* rootPath = argv[2];
	
	ifstream configFile(configFilePath);
	if (!configFile.is_open())
		Error("configure::main", string("Cannot open ") + configFilePath);
	
	
	
	ofstream outputCMakeConfig((string(rootPath) + "/Simulator/src/SET_SIMULATOR_VARIABLES.in").c_str());
	
	
	ofstream outputModels((string(rootPath) + "/Input/Dynamics/DynamicModels.h")); 
	outputModels.clear();
	
	json j;
	configFile >> j;
	
	try 
	{
		string chosenSimulatorViewer = j.at("simulator_viewer").get<string>();
		outputCMakeConfig << "set (CHOSEN_SIMULATOR_VIEWER " << chosenSimulatorViewer << ")\n";
		
		string videoOutputName;
		try
		{
			videoOutputName = j.at("video_output").get<string>();
			outputCMakeConfig << "set (VIDEO_OUTPUT " << videoOutputName << ")\n";
		}
		catch(out_of_range& e)
		{
			//Default value 
			outputCMakeConfig << "set (VIDEO_OUTPUT simulation.avi)\n";
		}
		
	
		
		
	}
	catch (out_of_range& e)
	{
		Error("configure::main", (string("simulator_viewer was not set in ") + configFilePath).c_str());
	}
	
	ofstream outputConversion((string(rootPath) + "/Input/Dynamics/StateConversions.h")); 
	
	
	generateDynamicFunctionsFile(j, outputModels);
	generateStateConversionFile(j, outputConversion);
	
	return 0;	
}



void generateDynamicFunctionsFile(const nlohmann::json& j, ostream& outputModels)
{
		outputModels.clear();
	
		json dynModelsJson = j.at("dynamic_models");
		string FcnType = "typedef void (*DynamicsFcn) (State&, const Agent&, const AgentVector&, const Control&);";
		
		string includeGuard = "#ifndef DYNAMIC_MODELS_H\n#define DYNAMIC_MODELS_H\n";
		string closeIncludeGuard = "#endif";
		
		string includeString = "#include <string>";
		string includeIostream = "#include <iostream>";
		string includeAgent = "#include \"Basic/Agent.h\"";
		string includeControl = "#include \"Basic/Control.h\"";
		
		
		vector<string> dynFcnName;
		for (int dynIndex = 0; dynIndex < dynModelsJson.size(); dynIndex++)
		{
			json modelJson = dynModelsJson.at(dynIndex);
			dynFcnName.push_back(modelJson.at("dynamics").get<string>());
		}
		
		outputModels << includeGuard << endl;
		outputModels << includeString << endl;
		outputModels << includeIostream << endl;
		outputModels << includeAgent << endl;
		outputModels << includeControl << endl;
		
		// This file will include the header of each dynamics function declared in dynamic_models entry
		for (int i = 0; i < dynFcnName.size(); i++)
			outputModels << "#include \"" << dynFcnName[i] << ".h\"" << endl;
		
		outputModels << endl;
		outputModels << FcnType << endl << endl;
		
		// Open function
		outputModels <<
		"DynamicsFcn GetDynamicsFunction(const std::string& fcnName) \n{\n\t";
		
		bool first = true;
		for (int i = 0; i < dynFcnName.size(); i++)
		{
			if (first)
			{
				outputModels << "if (fcnName == " << "\"" << dynFcnName[i] << "\")\n\t\t";
				outputModels << "return &" << dynFcnName[i] << ";\n\t";
				first = false;
			}
			else
			{
				outputModels << "else if (fcnName == " << "\"" << dynFcnName[i] << "\")\n\t\t";
				outputModels << "return &" << dynFcnName[i] << ";\n\t";
			}
		}
		
		outputModels << "else \n\t\t";
		outputModels << "{\n\t\t\tstd::cerr << \"Error in configure: \" << fcnName << \".h not found\";\n\t\t\texit(1);\n\t\t} " << endl;
		
		
		// Close function
		outputModels << "}" << endl << endl;
		outputModels << closeIncludeGuard;
		
}


void generateStateConversionFile(const nlohmann::json& j, ostream& os)
{
		os.clear();
	
		json conversionModelsJson = j.at("dynamic_models");
		string FcnType = "typedef void (*StateConversionFcn) (State&, const Agent&);";
		
		string includeGuard = "#ifndef STATE_CONVERSIONS_H\n#define STATE_CONVERSIONS_H\n";
		string closeIncludeGuard = "#endif";
		
		string includeString = "#include <string>";
		string includeIostream = "#include <iostream>";
		string includeAgent = "#include \"Basic/Agent.h\"";
		
		
		vector<string> conversionFcnName;
		for (int conversionIndex = 0; conversionIndex < conversionModelsJson.size(); conversionIndex++)
		{
			json modelJson = conversionModelsJson.at(conversionIndex);
			conversionFcnName.push_back(modelJson.at("world_conversion_function").get<string>());
		}
		
		os << includeGuard << endl;
		os << includeString << endl;
		os << includeIostream << endl;
		os << includeAgent << endl;
		
		// This file will include the header of each conversionamics function declared in conversionamic_models entry
		for (int i = 0; i < conversionFcnName.size(); i++)
			os << "#include \"" << conversionFcnName[i] << ".h\"" << endl;
		
		os << endl;
		os << FcnType << endl << endl;
		
		// Open function
		os <<
		"StateConversionFcn GetStateConversionFunction(const std::string& fcnName) \n{\n\t";
		
		bool first = true;
		for (int i = 0; i < conversionFcnName.size(); i++)
		{
			if (first)
			{
				os << "if (fcnName == " << "\"" << conversionFcnName[i] << "\")\n\t\t";
				os << "return &" << conversionFcnName[i] << ";\n\t";
				first = false;
			}
			else
			{
				os << "else if (fcnName == " << "\"" << conversionFcnName[i] << "\")\n\t\t";
				os << "return &" << conversionFcnName[i] << ";\n\t";
			}
		}
		
		os << "else \n\t\t";
		os << "{\n\t\t\tstd::cerr << \"Error in configure: \" << fcnName << \".h not found\";\n\t\t\texit(1);\n\t\t} " << endl;
		
		
		// Close function
		os << "}" << endl << endl;
		os << closeIncludeGuard;
		
}
