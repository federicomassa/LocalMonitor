#include "SimulationParameters.h"

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
