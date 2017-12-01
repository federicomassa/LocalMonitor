#ifndef ENVIRONMENT_PARAMETERS_H
#define ENVIRONMENT_PARAMETERS_H

#include <map>
#include <string>



class EnvironmentParameters
{
	typedef std::map<std::string, double> Params;
	Params params;
public:
	const double& at(const std::string&) const;
	double& at(const std::string&);
	
	const double& operator()(const std::string&) const;
	double& operator()(const std::string&);
	
	bool IsAvailable(const std::string& key) const;
	
	Params::const_iterator begin() const;
	Params::const_iterator end() const;
	Params::iterator begin();
	Params::iterator end();
	
	void AddEntry(const std::string& key, const double& value);
	
};

#endif
