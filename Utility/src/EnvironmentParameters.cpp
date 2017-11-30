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

EnvironmentParameters::Params::const_iterator EnvironmentParameters::begin() const
{
	return params.begin();
}

EnvironmentParameters::Params::const_iterator EnvironmentParameters::end() const
{
	return params.end();
}

EnvironmentParameters::Params::iterator EnvironmentParameters::begin()
{
	return params.begin();
}

EnvironmentParameters::Params::iterator EnvironmentParameters::end()
{
	return params.end();
}
