#ifndef DYNAMIC_MODEL_H
#define DYNAMIC_MODEL_H

#include <vector>
#include <string>

#include "Basic/Agent.h"
#include "Control.h"

typedef void (*DynamicsFcn) (State&, const Agent&, const Control&, const double&);
typedef void (*StateConversionFcn) (State&, const Agent&);

class DynamicModel
{
	std::string name;
    friend class State;
    std::vector<std::string> stateVars;
	std::vector<std::string> controlVars;
    std::string dynamicsName;
	std::string conversionFcnName;
	DynamicsFcn dynamicsFcn;
	StateConversionFcn conversionFcn;
	
	bool isNameSet;
	bool isStateVarsSet;
	bool isControlVarsSet;
	bool isDynamicsFcnSet;
	bool isConversionFcnSet;
	
public:
	DynamicModel();
	bool operator<(const DynamicModel&) const;
	bool operator==(const DynamicModel&) const;
	DynamicModel& operator=(const DynamicModel&);
	bool IsSet() const;
	void SetName(const std::string&);
	void SetStateVariables(const std::vector<std::string>&);
	void SetDynamicsFunctionName(const std::string&);
	void SetStateConversionFunctionName(const std::string&);
	void SetDynamicsFunction(DynamicsFcn);
	void SetStateConversionFunction(StateConversionFcn);
	void Run(State&, const Agent&, const Control&, const double&) const;
	State GetWorldState(const Agent&, const State& modelWorldState) const;
	void SetControlVariables(const std::vector<std::string>&);
	const std::string& GetName() const;
	const std::vector<std::string>& GetStateVariables() const;
	const std::vector<std::string>& GetControlVariables() const;
	const std::string& GetDynamicsFunctionName() const;
	
};

#endif
