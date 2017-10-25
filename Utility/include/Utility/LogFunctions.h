#ifndef LOGFUNCTIONS_H
#define LOGFUNCTIONS_H

#include <string>

namespace LogFunctions
{
/**
 * @brief Print class name
 */
std::string ClassInfo ( const std::string & );

/**
 * @brief Check that a condition is true, if not print error info
 */
void Require ( const bool &, const std::string &className, const std::string &msg );


/**
 * @brief Print error
 */
void Error ( const std::string className, const std::string msg = "");
void Info ( const std::string className, const std::string msg = "");
void Warning ( const std::string className, const std::string msg = "");


/**
 * @brief Convert double to string with given precision
 */
std::string ToStringWithPrecision ( const double &, const int &precision = 0 );
}

#endif
