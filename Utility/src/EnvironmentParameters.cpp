#include "EnvironmentParameters.h"

const double& EnvironmentParameters::at(const std::string& var) const
{
	return params.at(var);
}

double& EnvironmentParameters::at(const std::string& var)
{
	return params.at(var);
}

const double& EnvironmentParameters::operator()(const std::string& var) const
{
	return at(var);
}

double& EnvironmentParameters::operator()(const std::string& var)
{
	return at(var);
}

void EnvironmentParameters::AddEntry(const std::string& key, const double& value)
{
	params[key] = value;
}
