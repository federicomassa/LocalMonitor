#include "SimulatorConfiguration.h"
#include "SimulatorBuildParams.h"
#include "Utility/LogFunctions.h"
#include "Utility/Logger.h"

#include "Environment.h"

#include <string>
#include <iostream>

using namespace std;

Logger logger(cout);
int currentTimeStep = 0;
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
int main(int argc, char **argv)
{
    if (argc > 1) {
        LogFunctions::Error("Simulator::main", "Simulator should be called with no arguments");
    }

    // Parse config file
    conf.Parse();

    Environment env;
    env.Configure(conf);

    for (; currentTimeStep < conf.GetSimulationSteps(); currentTimeStep++) {
        env.Run();
    }


    return 0;
}

