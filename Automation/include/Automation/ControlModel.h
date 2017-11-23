#ifndef CONTROL_MODEL_H
#define CONTROL_MODEL_H

#include <vector>
#include <string>

class State;
class Control;

class ControlModel
{
	std::string name;
    friend class Control;
    typedef void ( *ControlFcn ) ( State&, const State&, const Control& );
    std::vector<std::string> stateVars;
	std::vector<std::string> controlVars;
    std::string dynamicsName;
	
public:
	DynamicModel();
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
