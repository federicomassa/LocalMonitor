#include "EnvironmentParameters.h"
#include "Utility/Logger.h"

using namespace std;

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

bool EnvironmentParameters::IsAvailable(const std::string& key) const
{
	bool isAvailable = true;
	
	try
	{
		at(key);
	}
	catch (out_of_range&)
	{
		isAvailable = false;
	}
	
	return isAvailable;
}

Logger& operator<< (Logger& os, const EnvironmentParameters& params)
{
	os << "Parameters: " << os.EndL();
    os << "{" << os.EndL(Logger::INC);
    
	for (auto itr = params.begin(); itr != params.end(); itr++)
		os << itr->first << "\t" << itr->second << os.EndL();
	
	os << "}" << os.EndL(Logger::DEC);

    return os;
}

