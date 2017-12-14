#include "SimulationParameters.h"
#include "LogFunctions.h"

SimulationParameters::SimulationParameters()
{}

const std::string& SimulationParameters::at(const std::string& var) const
{
	return params.at(var);
}

std::string& SimulationParameters::at(const std::string& var)
{
	return params.at(var);
}

const std::string& SimulationParameters::operator()(const std::string& var) const
{
	return at(var);
}

std::string& SimulationParameters::operator()(const std::string& var)
{
	return at(var);
}

void SimulationParameters::AddEntry(const std::string& key, const std::string& value)
{
	params[key] = value;
}

void SimulationParameters::RemoveEntry(const std::string& key)
{
	Params::iterator itr = params.find(key);
	if (itr == params.end())
		LogFunctions::Error("SimulationParameters::RemoveEntry", std::string("Parameter \'") + key + "\' not found");
	
	params.erase(itr);
}
