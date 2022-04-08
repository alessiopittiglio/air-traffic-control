#pragma once

#include "Entity.h"
#include "utils.h"

class Plane : public Entity {
protected:
	std::string _name;
	float _altitude;
public:
	Plane(QPointF nodeDeparture, QPointF nodeArrival, bool loadPic = true);

	std::string generateName();

	// pure virtual methods that must be implemented
	virtual std::string name() { return _name; }

	// getters
	int altitude() { return _altitude; }

	// setters
	void setName(std::string name);
	void setAltitude(int altitude);
};
