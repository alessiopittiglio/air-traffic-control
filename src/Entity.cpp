#include "Entity.h"
#include "Simulation.h"

Entity::Entity(QPointF nodeDeparture, QPointF nodeArrival) : Object() {
	_nodeDeparture = nodeDeparture;
	_nodeArrival = nodeArrival;

	// physics parameters
	_distance = distance(_nodeDeparture, _nodeArrival);
	_directionX = (_nodeArrival.x() - _nodeDeparture.x()) / _distance;
	_directionY = (_nodeArrival.y() - _nodeDeparture.y()) / _distance;
	_speed = DEFAULT_SPEED;

	// flags
	_moving = true;
	_dead = false;
}

void Entity::advance() {
	if (_moving) {
		setPos(pos().x() + _directionX * _speed / SPEEDCONVERT / FPS, pos().y() + _directionY * _speed / SPEEDCONVERT / FPS);

		if (distance(nodeDeparture(), pos()) >= _distance)
			die();
	}
}

QPainterPath Entity::shape() const {
	// collider is 4 pixels smaller than the bounding rectangle (default collider)
	QPainterPath path;
	path.addRect(4, 4, boundingRect().width() - 8, boundingRect().height() - 8);
	return path;
}

void Entity::setSpeed(float speed) {
	_speed = speed;
}

void Entity::die() {
	// only a full living entity can die
	if (!_dead) {
		// cannot move
		_moving = false;

		// dead
		_dead = true;
	}
}
