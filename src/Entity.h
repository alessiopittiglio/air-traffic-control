#pragma once

#include "Object.h"
#include "utils.h"

// Entity object are "living", i.e. they can move, collide, die, etc.
class Entity : public Object {
    protected:
		float _directionX, _directionY;

		QPointF _nodeDeparture, _nodeArrival;
		float _speed;
		float _distance;

		// flags
		bool _moving;
		bool _dead;

		QPixmap _textureStand;
    public:
        Entity(QPointF nodeDeparture, QPointF nodeArrival);

		// getters
		virtual int speed() { return _speed; }
		bool isDead() { return _dead; }
		virtual QPointF nodeDeparture() { return _nodeDeparture; }
		virtual QPointF nodeArrival() { return _nodeArrival; }

		// setters
		virtual void setSpeed(float speed);

		// pure virtual methods to be implemented
		virtual std::string name() = 0;

		// abstract methods implemented
        virtual void advance();

		// @override default collider
		virtual QPainterPath shape() const;

		// die method, performs common operations to all entities
		// but it will need be overridden in some cases
		virtual void die();
};
