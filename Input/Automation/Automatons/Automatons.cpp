#include "Automatons.h"
#include <iostream>

#include "TestAutomaton.h"

Automaton* InstantiateAutomaton(const std::string& automatonClassName)
{
	if (automatonClassName == "TestAutomaton")
		return new TestAutomaton(automatonClassName);
	else 
		{
			std::cerr << "Error in configure: " << automatonClassName << ".h not found";
			exit(1);
		} 
}
