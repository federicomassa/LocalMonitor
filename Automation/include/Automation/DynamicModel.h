#ifndef DYNAMIC_MODEL_H
#define DYNAMIC_MODEL_H

#include <vector>
#include <string>

#include "Basic/Agent.h"
#include "Control.h"

typedef void (*DynamicsFcn) (State&, const Agent&, const AgentVector&, const Control&, const double&);
typedef void (*StateConversionFcn) (Agent&, const Agent&);

class DynamicModel
{
	std::string name;
    friend class State;
    std::vector<std::string> stateVars;
	std::vector<std::string> controlVars;
    std::string dynamicsName;
	std::string conversionFcnToWorldName;
	std::string conversionFcnToStateName;
	DynamicsFcn dynamicsFcn;
	StateConversionFcn conversionFcnToWorld;
	StateConversionFcn conversionFcnToState;
	
	bool isNameSet;
	bool isStateVarsSet;
	bool isControlVarsSet;
	bool isDynamicsFcnSet;
	bool isConversionFcnToWorldSet;
	bool isConversionFcnToStateSet;
	
public:
	DynamicModel();
	DynamicModel(const DynamicModel&);

	bool operator<(const DynamicModel&) const;
	bool operator==(const DynamicModel&) const;
	DynamicModel& operator=(const DynamicModel&);
	bool IsSet() const;
	void SetName(const std::string&);
	void SetStateVariables(const std::vector<std::string>&);
	void SetDynamicsFunctionName(const std::string&);
	void SetStateConversionToWorldName(const std::string&);
	void SetStateConversionToStateName(const std::string&);
	void SetDynamicsFunction(DynamicsFcn);
	void SetStateConversionToWorld(StateConversionFcn);
	void SetStateConversionToState(StateConversionFcn);
	void Run(State&, const Agent&, const AgentVector&, const Control&, const double&) const;
	
	// Conversion functions
	State GetWorldState(const Agent&, const State& modelWorldState) const;
	Agent GetLocalState(const Agent&, const State& modelLocalState) const;

	void SetControlVariables(const std::vector<std::string>&);
	const std::string& GetName() const;
	const std::vector<std::string>& GetStateVariables() const;
	const std::vector<std::string>& GetControlVariables() const;
	const std::string& GetDynamicsFunctionName() const;
};

#endif
