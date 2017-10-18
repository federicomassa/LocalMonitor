#include "Automation/PhysicalLayer.h"
#include "Basic/State.h"
#include "Utility/Logger.h"
#include <iostream>
#include "Dynamics/TestKinematics.h"

Logger logger(std::cout);

//Link to Boost
 #define BOOST_TEST_DYN_LINK

//Define our Module name (prints at testing)
 #define BOOST_TEST_MODULE "Test PhysicalLayer"

//VERY IMPORTANT - include this last
#include <boost/test/unit_test.hpp>

// ------------- Tests Follow --------------
BOOST_AUTO_TEST_CASE( StateEvolution )
{
  double simulTime = 0.01;
	
  PhysicalLayer p(simulTime);
  p.SetKinematics(&TestKinematics);
  
  StateMap m;
  m["x"] = 0.3;
  m["y"] = 6.1;
  m["theta"] = 2.2;
  m["v"] = 4;
  
  State initState(m);
  
  State finalState = p.GetNextState(initState, Maneuver("man"));
//Check default constructor
//Use BOOST_CHECK for small equal checks - true or false
  BOOST_CHECK_EQUAL(finalState.at("x"), initState.at("x") + simulTime);
  BOOST_CHECK_EQUAL(finalState.at("y"), initState.at("y"));
}
