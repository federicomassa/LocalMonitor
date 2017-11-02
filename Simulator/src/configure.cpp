/*
 * This program generates code from the Simulator config file (Simulator.json)
 * 
 * */

#include "Utility/LogFunctions.h"
#include "Utility/Logger.h"
#include <iostream>

using namespace LogFunctions;
Logger logger(std::cout);

int main(int argc, char** argv)
{
	if (argc != 3)
		Error("Usage", "./configure <path/to/Simulator.json> <path/to/root_dir>");
	
	const char* configFilePath = argv[1];
	const char* rootPath = argv[2];
	
	
	
	
	return 0;	
}
