#ifndef SENSING_H
#define SENSING_H

class Sensing
{
	std::string ID;
	std::map<std::string, double> measurements;
	
public:
	const std::string& GetID() const;
	const std::map<std::string, double> GetMeasurements() const;
	const 
};

#endif
