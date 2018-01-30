#ifndef CONTROL_MODEL_H
#define CONTROL_MODEL_H

#include <vector>
#include <string>

#include "Basic/Maneuver.h"

class ControlModel
{
	friend class SimulatorConfiguration;
	
	std::string name;
	std::vector<Maneuver> maneuvers;
	std::vector<std::string> controlVars;
	std::string controllerClassName;
	std::string automatonClassName;
		
public:
	ControlModel();
	~ControlModel();
	ControlModel(const ControlModel&);
	ControlModel& operator=(const ControlModel&);
	bool operator<(const ControlModel&) const;
	bool operator==(const ControlModel&) const;
	void SetName(const std::string&);
	void SetAutomaton(const std::string&);
	const std::vector<Maneuver>& GetManeuvers() const;
	void SetManeuvers(const std::vector<Maneuver>&);
	void SetController(const std::string& className);
	void SetControlVariables(const std::vector<std::string>&);
	const std::string& GetName() const;
	const std::vector<std::string>& GetControlVariables() const;
	const std::string& GetAutomatonName() const;
	const std::string& GetControllerName() const;
	const std::vector<Maneuver>& GetPossibleManeuvers() const;
};

#endif
