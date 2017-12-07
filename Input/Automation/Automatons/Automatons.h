#ifndef AUTOMATIONS_H
#define AUTOMATIONS_H

#include <string>

class Automaton;

Automaton* InstantiateAutomaton(const std::string& automatonClassName);


#endif
