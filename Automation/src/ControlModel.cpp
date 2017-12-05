#include "ControlModel.h"
#include "Input/Automation/Controllers/Controllers.h"
#include "Input/Automation/Automatons/Automatons.h"

using namespace std;

void ControlModel::SetName(const string& n)
{
	name = n;
}

void ControlModel::SetManeuvers(const std::vector<std::string>& man)
{
	maneuvers = man;
}

void ControlModel::SetControlVariables(const std::vector<std::string>& c)
{
	controlVars = c;
}

void ControlModel::SetAutomaton(const std::string& aName)
{
	automatonName = aName;
	
	if (automaton)
		delete automaton;
}


void ControlModel::SetController(const string& className)
{
	controllerClassName = className;
	
	if (controller)
		delete controller;
	
	controller = InstantiateController(className);
}


const std::vector<std::string> & ControlModel::GetControlVariables() const
{
	return controlVars;
}

const std::string& ControlModel::GetAutomatonName() const
{
	return automatonClassName;
}

const std::string & ControlModel::GetName() const
{
	return name;
}

ControlModel::ControlModel() : controller(nullptr), automaton(nullptr)
{
}

ControlModel::~ControlModel() 
{
	if (controller)
		delete controller;
	
	if (automaton)
		delete automaton;
}

ControlModel::ControlModel(const ControlModel& m) : controller(nullptr), automaton(nullptr)
{
	name = m.name;
	maneuvers = m.maneuvers;
	controlVars = m.controlVars;
	controllerClassName = m.controllerClassName;
	automatonClassName = m.automatonClassName;
	
	controller = InstantiateController(m.controllerClassName);
	automaton = InstantiateAutomaton(m.automatonClassName);
}


ControlModel & ControlModel::operator=(const ControlModel& m)
{
	name = m.name;
	maneuvers = m.maneuvers;
	controlVars = m.controlVars;
	controllerClassName = m.controllerClassName;
	
	if (controller)
		delete controller;
	
	if (automaton)
		delete automaton;
	
	controller = InstantiateController(m.controllerClassName);
	automaton = InstantiateAutomaton(m.automatonClassName);

}


bool ControlModel::operator<(const ControlModel& model) const
{
	return (name < model.name);
}

bool ControlModel::operator==(const ControlModel& model) const
{
	return (name == model.name);
}

Controller* const & ControlModel::GetController() const
{
	return controller;
}
