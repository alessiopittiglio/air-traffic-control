#pragma once

#include <QGraphicsPixmapItem>

// base class for all game objects
class Object : public QGraphicsPixmapItem {
protected:
	bool _collidable;		// whether the object is collidable
public:
	Object();

	// getters
	bool isCollidable() { return _collidable; }

	// object name
	virtual std::string name() = 0;

	// advance (= compute next location)
	virtual void advance() = 0;
};
