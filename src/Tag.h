#pragma once

#include <QPainter>
#include "Entity.h"
#include "Plane.h"
#include "utils.h"

class Tag : public Entity {
protected:
	QString _idText, _altitudeText, _speedText, _namePlane;
public:
	Tag(Plane* p);

	// pure virtual methods that must be implemented
	virtual std::string name() { return "Tag"; }

	void updateTexture();

	// getters
	QString namePlane() { return _namePlane; }

	// setters
	void setSpeedText(QString speed);
	void setAltitudeText(QString altitude);
};
