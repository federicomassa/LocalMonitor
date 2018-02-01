#include "DynamicModel.h"
#include "Utility/LogFunctions.h"

using namespace std;
using namespace LogFunctions;

DynamicModel::DynamicModel()
{
	dynamicsFcn = nullptr;
	conversionFcnToWorld = nullptr;
	conversionFcnToState = nullptr;
	
	isNameSet = isStateVarsSet = isControlVarsSet = isDynamicsFcnSet = isConversionFcnToWorldSet = false, isConversionFcnToStateSet = false;
}

void DynamicModel::Run(State& qdot, const Agent& self, const AgentVector& trueOthersInWorld, const Control& u, const double& simulDeltaT) const
{
	dynamicsFcn(qdot, self, trueOthersInWorld, u, simulDeltaT);
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

DynamicModel::DynamicModel(const DynamicModel& m)
{
	*this = m;
}

DynamicModel& DynamicModel::operator=(const DynamicModel& m)
{
	SetName(m.name);
	SetStateVariables(m.stateVars);
	SetDynamicsFunction(m.dynamicsFcn);
	SetDynamicsFunctionName(m.dynamicsName);
	SetControlVariables(m.controlVars);
	SetStateConversionToWorld(m.conversionFcnToWorld);
	SetStateConversionToWorldName(m.conversionFcnToWorldName);
	SetStateConversionToState(m.conversionFcnToState);
	SetStateConversionToStateName(m.conversionFcnToStateName);
	
	return *this;
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

void DynamicModel::SetDynamicsFunctionName(const string& s)
{
	dynamicsName = s;
}

void DynamicModel::SetStateConversionToWorldName(const std::string& s)
{
	conversionFcnToWorldName = s;
}

void DynamicModel::SetStateConversionToStateName(const std::string& s)
{
	conversionFcnToStateName = s;
}


void DynamicModel::SetDynamicsFunction(DynamicsFcn fcn)
{
	isDynamicsFcnSet = true;
	dynamicsFcn = fcn;
}

void DynamicModel::SetStateConversionToWorld(StateConversionFcn fcn)
{
	isConversionFcnToWorldSet = true;
	conversionFcnToWorld = fcn;
}

void DynamicModel::SetStateConversionToState(StateConversionFcn fcn)
{
	isConversionFcnToStateSet = true;
	conversionFcnToState = fcn;
}


State DynamicModel::GetWorldState(const Agent& a, const State& modelWorldState) const
{
	State convertedState = State::GenerateStateOfType(modelWorldState);
	Agent worldAgent;
	
	worldAgent.SetID(a.GetID());
	worldAgent.SetState(convertedState);
	
	if (IsSet())
		conversionFcnToWorld(worldAgent, a);
	else
		Error("DynamicModel::GetWorldState", "Cannot call GetWorldState without setting the dynamic model first");
	
	return worldAgent.GetState();
}

Agent DynamicModel::GetLocalState(const Agent& a, const State& modelLocalState) const
{
	State convertedState = State::GenerateStateOfType(modelLocalState);
	Agent localAgent;
	
	localAgent.SetID(a.GetID());
	localAgent.SetState(convertedState);
	
	if (IsSet())
		conversionFcnToState(localAgent, a);
	else
		Error("DynamicModel::GetLocalState", "Cannot call GetLocalState without setting the dynamic model first");
	
	return localAgent;
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
	return (isNameSet && isStateVarsSet && isControlVarsSet && isDynamicsFcnSet && isConversionFcnToWorldSet && isConversionFcnToStateSet);
}
