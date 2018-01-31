#include "ControlModel.h"

using namespace std;

void ControlModel::SetName(const string& n)
{
	name = n;
}

void ControlModel::SetManeuvers(const std::vector<Maneuver>& man)
{
	maneuvers = man;
}

void ControlModel::SetControlVariables(const std::vector<std::string>& c)
{
	controlVars = c;
}

void ControlModel::SetAutomaton(const std::string& aName)
{
	automatonClassName = aName;
}


void ControlModel::SetController(const string& className)
{
	controllerClassName = className;
}


const std::vector<std::string> & ControlModel::GetControlVariables() const
{
	return controlVars;
}

const std::string& ControlModel::GetAutomatonName() const
{
	return automatonClassName;
}

const std::string& ControlModel::GetControllerName() const
{
	return controllerClassName;
}


const std::string & ControlModel::GetName() const
{
	return name;
}

const std::vector<Maneuver> & ControlModel::GetManeuvers() const
{
	return maneuvers;
}

ControlModel::ControlModel()
{
}

ControlModel::~ControlModel() 
{

}

ControlModel::ControlModel(const ControlModel& m) 
{
	*this = m;
}


ControlModel & ControlModel::operator=(const ControlModel& m)
{
	name = m.name;
	maneuvers = m.maneuvers;
	controlVars = m.controlVars;
	controllerClassName = m.controllerClassName;
	automatonClassName = m.automatonClassName;
	
	return *this;
}


bool ControlModel::operator<(const ControlModel& model) const
{
	return (name < model.name);
}

bool ControlModel::operator==(const ControlModel& model) const
{
	return (name == model.name);
}

const std::vector<Maneuver> & ControlModel::GetPossibleManeuvers() const
{
	return maneuvers;
}

