/*
 * This file defines two objects:
 * --- WorldEnvironmentFeatures: contains everything that an agent's sensor can potentially measure of the environment (e.g. lane width in a highway)
 * 
 * --- WorldAgentFeatures: contains everything that an agent's sensor can potentially measure of another agent (e.g. position of back and front of a vehicle, speed)
 * 
 * */

#ifndef WORLD_FEATURES_H
#define WORLD_FEATURES_H

#include <map>
#include <set>
#include <string>

typedef std::set<std::string> WorldEnvironmentFeatures;
typedef std::set<std::string> WorldAgentFeatures;

/*
typedef Agent WorldAgent;
typedef std::map<std::string, WorldAgent> WorldAgentVector;
*/

#endif
