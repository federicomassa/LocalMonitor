#include "Observers.h"
#include "NaiveObserver.h"

Observer * InstantiateObserver(const std::string& observerClassName)
{
	if (observerClassName == "NaiveObserver")
		return new NaiveObserver(observerClassName);
	else
	{
		std::cerr << "Error in configure: " << observerClassName << ".h not found";
		exit(1);
	}
}
