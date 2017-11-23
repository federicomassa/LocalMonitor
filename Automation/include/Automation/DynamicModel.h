#ifndef DYNAMIC_MODEL_H
#define DYNAMIC_MODEL_H

#include <vector>
#include <string>

#include "Input/Dynamics/DynamicModels.h"
#include "Input/Dynamics/StateConversions.h"

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
	void SetDynamicsFunction(const std::string&);
	void SetStateConversionFunction(const std::string&);

	void SetControlVariables(const std::vector<std::string>&);
	const std::string& GetName() const;
	const std::vector<std::string>& GetStateVariables() const;
	const std::vector<std::string>& GetControlVariables() const;
	const std::string& GetDynamicsFunctionName() const;
	
};

#endif
