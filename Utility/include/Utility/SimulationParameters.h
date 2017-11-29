#ifndef SIMULATION_PARAMETERS_H
#define SIMULATION_PARAMETERS_H

#include <map>
#include <string>



class SimulationParameters
{
	typedef std::map<std::string, std::string> Params;
	Params params;
public:
	SimulationParameters();
	const std::string& at(const std::string&) const;
	std::string& at(const std::string&);
	
	const std::string& operator()(const std::string&) const;
	std::string& operator()(const std::string&);
	
	void AddEntry(const std::string& key, const std::string& value);
	
};

#endif
