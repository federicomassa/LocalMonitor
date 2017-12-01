#include "DynamicModel.h"
#include "Utility/LogFunctions.h"

using namespace std;
using namespace LogFunctions;

DynamicModel::DynamicModel()
{
	dynamicsFcn = nullptr;
	conversionFcn = nullptr;
	
	isNameSet = isStateVarsSet = isControlVarsSet = isDynamicsFcnSet = isConversionFcnSet = false;
}

void DynamicModel::Run(State& qdot, const Agent& self, const Control& u) const
{
	dynamicsFcn(qdot, self, u);
}



// Used to order elements in a std::set
bool DynamicModel::operator<(const DynamicModel& m) const
{
	return (name < m.name);
}

bool DynamicModel::operator==(const DynamicModel& m) const
{
	return (name == m.name);
}

DynamicModel& DynamicModel::operator=(const DynamicModel& m)
{
	SetName(m.name);
	SetStateVariables(m.stateVars);
	SetDynamicsFunction(m.dynamicsName);
	SetControlVariables(m.controlVars);
	SetStateConversionFunction(m.conversionFcnName);
}

void DynamicModel::SetName(const string& s)
{
	isNameSet = true;
	name = s;
}

void DynamicModel::SetStateVariables(const vector<string>& s)
{
	isStateVarsSet = true;
	stateVars = s;
}

void DynamicModel::SetDynamicsFunction(const string& s)
{
	isDynamicsFcnSet = true;
	dynamicsName = s;
	dynamicsFcn = GetDynamicsFunction(s);
}

void DynamicModel::SetStateConversionFunction(const std::string& s)
{
	isConversionFcnSet = true;
	conversionFcnName = s;
	conversionFcn = GetStateConversionFunction(s);
}


State DynamicModel::GetWorldState(const Agent& a, const State& modelWorldState) const
{
	State convertedState = State::GenerateStateOfType(modelWorldState);
	
	if (IsSet())
		conversionFcn(convertedState, a);
	else
		Error("DynamicModel::GetWorldState", "Cannot call GetWorldState without setting the dynamic model first");
	
	return convertedState;
}

void DynamicModel::SetControlVariables(const vector<string>& s)
{
	isControlVarsSet = true;
	controlVars = s;
}

const string& DynamicModel::GetName() const
{
	return name;
}

const string& DynamicModel::GetDynamicsFunctionName() const
{
	return dynamicsName;
}

const vector<string>& DynamicModel::GetStateVariables() const
{
	return stateVars;
}

const vector<string>& DynamicModel::GetControlVariables() const
{
	return controlVars;
}

bool DynamicModel::IsSet() const
{
	return (isNameSet && isStateVarsSet && isControlVarsSet && isDynamicsFcnSet && isConversionFcnSet);
}
