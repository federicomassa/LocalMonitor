#ifndef CONTROL_MODEL_H
#define CONTROL_MODEL_H

#include <vector>
#include <string>

#include "Controller.h"

class ControlModel
{
	friend class SimulatorConfiguration;
	
	std::string name;
	std::vector<std::string> maneuvers;
	std::vector<std::string> controlVars;
	std::string controllerClassName;
    Controller* controller;
		
public:
	ControlModel();
	~ControlModel();
	ControlModel(const ControlModel&);
	ControlModel& operator=(const ControlModel&);
	bool operator<(const ControlModel&) const;
	bool operator==(const ControlModel&) const;
	void SetName(const std::string&);
	void SetManeuvers(const std::vector<std::string>&);
	void SetController(const std::string& className);
	void SetControlVariables(const std::vector<std::string>&);
	const std::string& GetName() const;
	const std::vector<std::string>& GetControlVariables() const;
	Controller* const & GetController() const;
	
};

#endif
