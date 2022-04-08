#include "Plane.h"
#include "Simulation.h"
#include "config.h"

#include <QtMath>

Plane::Plane(QPointF nodeDeparture, QPointF nodeArrival, bool loadPic) : _altitude(DEFAULT_ALTITUDE), Entity(nodeDeparture, nodeArrival) {
	
	if (loadPic)
	{
		_textureStand = QPixmap(loadTexture(":/graphics/plane.png"));

		// set texture
		setPixmap(_textureStand);
	}

	_name = generateName();
	
	// spawn position	
	setTransformOriginPoint(boundingRect().center());
	setRotation(atan2(_nodeArrival.y() - _nodeDeparture.y(), _nodeArrival.x() - _nodeDeparture.x()) * 180 / M_PI);
	setPos(_nodeDeparture);
}

std::string Plane::generateName() {
	auto randchar = []() -> char {
		const char charset[] =
			"0123456789"
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ";
		const size_t max_index = (sizeof(charset) - 1);
		return charset[rand() % max_index];
	};

	std::string str(6, 0);
	std::generate_n(str.begin(), 6, randchar);

	return str;
}

void Plane::setAltitude(int altitude) {
	_altitude = altitude;
}

void Plane::setName(std::string name) {
	_name = name;
}
