/*
 * This class contains environment parameters that are supposed to be taken
 * from the config file.
 * 
 * TODO: now parameters can only be of type double, thinking about generalizing using
 * 		boost::variant
 * 
 * */

#ifndef ENVIRONMENT_PARAMETERS_H
#define ENVIRONMENT_PARAMETERS_H

#include <map>
#include <string>

typedef std::map<std::string, double> EnvironmentParameters;

#endif
