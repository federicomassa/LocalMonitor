#include "SimulatorConfiguration.h"
#include "SimulatorBuildParams.h"
#include "Utility/LogFunctions.h"
#include "Utility/Logger.h"

#ifdef USE_GRAPHICS
#include "SimulatorViewer/SimulatorViewer.h"
#include CHOSEN_VIEWER_INCLUDE
#include <QApplication>
#endif

#include "Environment.h"

#include <string>
#include <iostream>

using namespace std;

Logger logger(cout);
SimulatorConfiguration conf(string(INPUT_DIR) + "/" + string(SIMULATOR_CONFIG_FILE));


/**
 * @brief This is the simulator of the LocalMonitor
 * software. The environment configuration
 * is obtained by parsing Input/Simulator.conf
 *
 * @param argc p_argc: Number of arguments provided after command
 * @param argv p_argv: Arguments provided
 * @return int: error code
 */

// TODO add option for graphics use. USE_GRAPHICS is intended for
// users who do not want to build the project with Qt.
// The option would be intended for those who potentially want the viewer
// but do not want it in every run
int main(int argc, char **argv)
{
   if (argc > 1) {
        LogFunctions::Error("Simulator::main", "Simulator should be called with no arguments");
    }
	
	int currentTimeStep = 0;
	
    // Parse config file
    conf.Parse();

    Environment env;
    env.Configure(conf);
	
	
	#ifdef USE_GRAPHICS
	QApplication app(argc, argv);
	SimulatorViewer* simViewer = new CHOSEN_VIEWER(conf);
	
	simViewer->DrawStaticEnvironment();
	#endif
	
    for (; currentTimeStep < conf.GetSimulationSteps(); currentTimeStep++) 
	{
		double currentTime = currentTimeStep*conf.GetSimulationTimeStep();
		
		env.ConvertAgentsToWorld();
		
		#ifdef USE_GRAPHICS
		simViewer->DrawDynamicEnvironment(env.GetAgents());
		simViewer->Encode();
		#endif
		
        env.Run(currentTime);
    }
    
#ifdef USE_GRAPHICS
    delete simViewer;
#endif
	
	return 0;
}

