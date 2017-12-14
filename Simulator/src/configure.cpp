/*
 * This program generates code from the Simulator config file (Simulator.json)
 * 
 * */

#include "Utility/LogFunctions.h"
#include "Utility/MyLogger.h"
#include "SimulatorBuildParams.h"
#include "json.hpp"
#include <iostream>
#include <fstream>
#include <utility>
#include <string>

using namespace nlohmann;
using namespace LogFunctions;
using namespace std;
MyLogger logger(std::cout);

void generateDynamicFunctionsFile(const nlohmann::json& j, ostream& outputModels);
void generateStateConversionFile(const nlohmann::json& j, ostream& os);
void generateSensorList(const nlohmann::json& j, ostream& os);
void generateControllerList(const nlohmann::json& j, ostream& os);


int main(int argc, char** argv)
{
	if (argc != 3)
		Error("Usage", "./configure <path/to/Simulator.json> <path/to/project root dir> without slashes at the end of dir");
	
	const char* configFilePath = argv[1];
	const char* rootPath = argv[2];
	
	ifstream configFile(configFilePath);
	if (!configFile.is_open())
		Error("configure::main", string("Cannot open ") + configFilePath);
	
	Info("configure::main", string("Opened file ") + argv[1]);
	
	ofstream outputCMakeConfig((string(rootPath) + "/Simulator/src/SET_SIMULATOR_VARIABLES.in").c_str());
	
	
	ofstream outputModels((string(rootPath) + "/Input/Dynamics/DynamicModels.h")); 
	outputModels.clear();
	
	json j;
	
	try
	{
		configFile >> j;
	}
	catch(exception& e)
	{
		Error("configure - json parsing", e.what());
	}
	
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
	
	
	Info("configure::main", "CMake output configured.");
	
	ofstream outputConversion((string(rootPath) + "/Input/Dynamics/StateConversions.h")); 
	
	
	generateDynamicFunctionsFile(j, outputModels);
	
	Info("configure::main", "Dynamic functions configured.");
	
	generateStateConversionFile(j, outputConversion);
	
	Info("configure::main", "Agent to world state conversion functions configured.");
	
	ofstream outputSensors((string(rootPath) + "/Input/Automation/Sensors/Sensors.h")); 
	generateSensorList(j, outputSensors);
	
	Info("configure::main", "Sensors list configured.");
	// TODO Generate skeleton header and source for Sensor children
	
	
	
	ofstream outputControllers((string(rootPath) + "/Input/Automation/Controllers/Controllers.h")); 
	generateControllerList(j, outputControllers);
	
	Info("configure::main", "Controller list configured.");
	
	cout << endl << "...configuration complete!" << endl;
	return 0;	
}



void generateDynamicFunctionsFile(const nlohmann::json& j, ostream& outputModels)
{
		outputModels.clear();
	
		json dynModelsJson = j.at("dynamic_models");
		string FcnType = "typedef void (*DynamicsFcn) (State&, const Agent&, const Control&);";
		
		string includeGuard = "#ifndef DYNAMIC_MODELS_H\n#define DYNAMIC_MODELS_H\n";
		string closeIncludeGuard = "#endif";
		
		string includeString = "#include <string>";
		string includeCstdlib = "#include <cstdlib>";
		string includeIostream = "#include <iostream>";
		string includeAgent = "#include \"Basic/Agent.h\"";
		string includeControl = "#include \"Automation/Control.h\"";
		
		
		vector<string> dynFcnName;
		for (int dynIndex = 0; dynIndex < dynModelsJson.size(); dynIndex++)
		{
			json modelJson = dynModelsJson.at(dynIndex);
			dynFcnName.push_back(modelJson.at("dynamics").get<string>());
		}
		
		outputModels << includeGuard << endl;
		outputModels << includeString << endl;
		outputModels << includeCstdlib << endl;
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

// TODO Automatically generate skeleton for sensor child class (header and source)
void generateSensorList(const nlohmann::json& j, ostream& os)
{
		os.clear();
	
		enum Type {Internal, External};
		
		json thisJson = j.at("sensors");
		
		string includeGuard = "#ifndef SENSORS_H\n#define SENSORS_H\n";
		string closeIncludeGuard = "#endif";
		
		string includeString = "#include <string>";
		string includeIostream = "#include <iostream>";		
		string includeIntSensors = "#include \"Automation/InternalSensor.h\"";
		string includeExtSensors = "#include \"Automation/ExternalSensor.h\"";
		
		vector<pair<string, Type> > sensorClasses;
		for (int index = 0; index < thisJson.size(); index++)
		{
			json currJson = thisJson.at(index);
			string name = currJson.at("name").get<string>();
			string typeStr = currJson.at("type").get<string>();
			Type type;
			
			if (typeStr == "internal" ||
				typeStr == "Internal" ||
				typeStr == "INTERNAL")
				type = Internal;
			else if (typeStr == "external" ||
				typeStr == "External" ||
				typeStr == "EXTERNAL")
				type = External;
			else
				Error("Configure::generateSensorList", string("Unknown type \'" ) + typeStr + "\'");
			
			sensorClasses.push_back(make_pair(name, type));
		}
		
		os << includeGuard << endl;
		os << includeString << endl;
		os << includeIostream << endl;
		
		os << endl << includeIntSensors << endl;
		os << includeExtSensors << endl;
		
		// This file will include the header of each conversionamics function declared in conversionamic_models entry
		for (int i = 0; i < sensorClasses.size(); i++)
			os << "#include \"" << sensorClasses[i].first << ".h\"" << endl;
		
		os << endl;
		
		// ============= Internal sensor function ============
		// Open function
		os <<
		"InternalSensor* InstantiateInternalSensor(const std::string& sensorClassName) \n{\n\t";
		
		bool first = true;
		for (int i = 0; i < sensorClasses.size(); i++)
		{
			if (sensorClasses[i].second != Internal)
				continue;
			
			if (first)
			{
				os << "if (sensorClassName == " << "\"" << sensorClasses[i].first << "\")\n\t\t";
				os << "return new " << sensorClasses[i].first << ";\n\t";
				first = false;
			}
			else
			{
				os << "else if (sensorClassName == " << "\"" << sensorClasses[i].first << "\")\n\t\t";
				os << "return new " << sensorClasses[i].first << ";\n\t";
			}
		}
		
		// There was at least one sensor
		if (!first)
		{
			os << "else \n\t\t";
			os << "{\n\t\t\tstd::cerr << \"Error in configure: \" << sensorClassName << \".h not found\";\n\t\t\texit(1);\n\t\t} " << endl;
		}
		else
		{
			os << "std::cerr << \"Error in configure: \" << sensorClassName << \".h not found\";\n\texit(1);\n";
		}
		
		// Close function
		os << "}" << endl << endl;
				
		// ========== External sensor function ===========
		// Open function
		os <<
		"ExternalSensor* InstantiateExternalSensor(const std::string& sensorClassName) \n{\n\t";
		
		first = true;
		for (int i = 0; i < sensorClasses.size(); i++)
		{
			if (sensorClasses[i].second != External)
				continue;
			
			if (first)
			{
				os << "if (sensorClassName == " << "\"" << sensorClasses[i].first << "\")\n\t\t";
				os << "return new " << sensorClasses[i].first << ";\n\t";
				first = false;
			}
			else
			{
				os << "else if (sensorClassName == " << "\"" << sensorClasses[i].first << "\")\n\t\t";
				os << "return new " << sensorClasses[i].first << ";\n\t";
			}
		}
		
		if (!first)
		{
			os << "else \n\t\t";
			os << "{\n\t\t\tstd::cerr << \"Error in configure: \" << sensorClassName << \".h not found\";\n\t\t\texit(1);\n\t\t} " << endl;
		}
		else
		{
			os << "std::cerr << \"Error in configure: \" << sensorClassName << \".h not found\";\n\texit(1);\n";
		}
		
		// Close function
		os << "}" << endl << endl;
		
		os << closeIncludeGuard;
		
}



void generateControllerList(const nlohmann::json& j, ostream& os)
{
		os.clear();
		
		json thisJson = j.at("control_models");
		
		string includeGuard = "#ifndef CONTROLLERS_H\n#define CONTROLLERS_H\n";
		string closeIncludeGuard = "#endif";
		
		string includeString = "#include <string>";
		string includeIostream = "#include <iostream>";		
		
		vector<string> controllerClasses;
		for (int index = 0; index < thisJson.size(); index++)
			controllerClasses.push_back(thisJson.at(index).at("controller").get<string>());
		
		
		os << includeGuard << endl;
		os << includeString << endl;
		os << includeIostream << endl;
		
		// This file will include the header of each conversionamics function declared in conversionamic_models entry
		for (int i = 0; i < controllerClasses.size(); i++)
			os << "#include \"" << controllerClasses[i] << ".h\"" << endl;
		
		os << endl;
		
		// ============= Internal sensor function ============
		// Open function
		os <<
		"Controller* InstantiateController(const std::string& controllerClassName) \n{\n\t";
		
		bool first = true;
		for (int i = 0; i < controllerClasses.size(); i++)
		{
			
			if (first)
			{
				os << "if (controllerClassName == " << "\"" << controllerClasses[i] << "\")\n\t\t";
				os << "return new " << controllerClasses[i] << ";\n\t";
				first = false;
			}
			else
			{
				os << "else if (controllerClassName == " << "\"" << controllerClasses[i] << "\")\n\t\t";
				os << "return new " << controllerClasses[i] << "(" << controllerClasses[i] << ")" << ";\n\t";
			}
		}
		
		// There was at least one sensor
		if (!first)
		{
			os << "else \n\t\t";
			os << "{\n\t\t\tstd::cerr << \"Error in configure: \" << controllerClassName << \".h not found\";\n\t\t\texit(1);\n\t\t} " << endl;
		}
		else
		{
			os << "std::cerr << \"Error in configure: \" << controllerClassName << \".h not found\";\n\texit(1);\n";
		}
		
		// Close function
		os << "}" << endl << endl;
				
		os << closeIncludeGuard;
		
}


