#include <iostream>
#include <string>
#include "Utility/Logger.h"

#include "SimulatorViewer/SimulatorViewer.h"

#include "SimulatorBuildParams.h"
#include "SimulatorConfiguration.h"


//Link to Boost
#define BOOST_TEST_DYN_LINK

//Define our Module name (prints at testing)
#define BOOST_TEST_MODULE "Test Simulator Viewers"

//VERY IMPORTANT - include this last
#include <boost/test/unit_test.hpp>

Logger logger(std::cout);
using namespace std;

int currentTimeStep = 0;

SimulatorConfiguration conf(string(INPUT_DIR) + "/" + string(SIMULATOR_CONFIG_FILE));


// ------------- Tests Follow --------------
BOOST_AUTO_TEST_CASE(ViewerChoice)
{
	
	SimulatorViewer simV(conf);
	simV.Initialize();
	
	simV.SetOutputFileName(VIDEO_OUTPUT);

#ifdef USE_GRAPHICS
	std::cout << "Using graphics!" << std::endl;
#else
		cout << "NOT using graphics!" << endl;
#endif
	
	simV.Print();
	
}
