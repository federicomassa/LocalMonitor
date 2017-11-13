// NB Must be linked to Utility and Basic library
#include <string>

#include "ConvertState.h"
#include "Utility/LogFunctions.h"
#include "Basic/State.h"

using namespace std;
using namespace LogFunctions;

void ConvertFromState(const State& q, const std::string& ID, double& x, double& y, double& theta)
{
  try
    {
      x = q.at("x");
      y = q.at("y");
      theta = q.at("theta");
    }
  catch (out_of_range& e)
    {
      Error("ConvertFromState", string("x, y or theta not available in agent") + ID);
    }
}
