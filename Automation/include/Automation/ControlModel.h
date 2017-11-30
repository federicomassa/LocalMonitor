#ifndef CONTROL_MODEL_H
#define CONTROL_MODEL_H

#include <vector>
#include <string>

#include "Controller.h"

class ControlModel
{
	std::string name;
	std::vector<std::string> controlVars;
    Controller* controller;
	
public:
	ControlModel();
	bool operator<(const ControlModel&) const;
	bool operator==(const ControlModel&) const;
	void SetName(const std::string&);
	void SetControlVariables(const std::vector<std::string>&);
	void SetDynamicsFunction(const std::string&);
	void SetControlVariables(const std::vector<std::string>&);
	const std::string& GetName() const;
	const std::vector<std::string>& GetStateVariables() const;
	const std::vector<std::string>& GetControlVariables() const;
	const std::string& GetDynamicsFunctionName() const;
	
};

#endif
