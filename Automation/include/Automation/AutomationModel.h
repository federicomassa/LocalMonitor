#ifndef AUTOMATION_MODEL_H
#define AUTOMATION_MODEL_H

#include "Basic/Control.h"

class AutomationModel
{
	std::string name;
    friend class State;
    typedef void ( *ControllerFcn ) ( State&, const State&, const Control& );
    std::vector<std::string> stateVars;
	std::vector<std::string> controlVars;
    std::string dynamicsName;
	
public:
	AutomationModel();
	bool operator<(const DynamicModel&) const;
	bool operator==(const DynamicModel&) const;
	void SetName(const std::string&);
	void SetStateVariables(const std::vector<std::string>&);
	void SetDynamicsFunction(const std::string&);
	void SetControlVariables(const std::vector<std::string>&);
	const std::string& GetName() const;
	const std::vector<std::string>& GetStateVariables() const;
	const std::vector<std::string>& GetControlVariables() const;
	const std::string& GetDynamicsFunctionName() const;
	
};

#endif
