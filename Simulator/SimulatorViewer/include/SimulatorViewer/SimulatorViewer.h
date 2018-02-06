/*
 * Base class for simulator viewers
 * */

#ifndef SIMULATORVIEWER_H
#define SIMULATORVIEWER_H

#include <string>
#include <map>
#include "Simulator/SimulAgent.h"
#include <QMainWindow>

class SimulatorConfiguration;



/**
 * @brief Base class for simulator viewers. NB the particular design chosen 
 * requires all objects in this class (except for parent) to be initialized in Initialize method for base object (parent == nullptr) and to parent objects if 
 * parent != nullptr. The pointer named 'object' points to the child object, i.e. the
 * specific simulator viewer. Only the base object is allowed to delete allocated memory. See constructor, destructor and Initialize method to see examples.
 * Also, each virtual method added should explicitly call its object's version.
 * 
 */
class SimulatorViewer : public QMainWindow
{
	Q_OBJECT

	const SimulatorConfiguration& conf;
	typedef std::map<std::string, std::string> Properties;
	
	//Properties
	Properties properties;
	
protected:
	
	const SimulatorConfiguration& GetSimulatorConfiguration() const;
public:
	SimulatorViewer(const SimulatorConfiguration&);
	virtual ~SimulatorViewer();
	virtual void DrawStaticEnvironment(); 
	virtual void DrawDynamicEnvironment(const SimulAgentVector&, const double& currentTime = 0); 
	
	/**
	 * @brief Set a viewer property, that must have been registered in the constructor of the child viewer
	 * 
	 */
	void SetProperty(const std::string& propertyName, const std::string& propertyValue);
	std::string GetProperty(const std::string& propertyName) const;
	virtual void Encode();
};

#endif
