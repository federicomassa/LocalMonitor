#include "Automatons.h"
#include <iostream>

#include "TestAutomaton.h"
#include "PowNetDistrAutomaton.h"
#include "PowNetConstAutomaton.h"

Automaton* InstantiateAutomaton(const std::string& automatonClassName)
{
	if (automatonClassName == "TestAutomaton")
		return new TestAutomaton(automatonClassName);
	else if (automatonClassName == "PowNetDistrAutomaton")
		return new PowNetDistrAutomaton(automatonClassName);
	else if (automatonClassName == "PowNetConstAutomaton")
		return new PowNetConstAutomaton(automatonClassName);

	else 
		{
			std::cerr << "Error in configure: " << automatonClassName << ".h not found";
			exit(1);
		} 
}
