#include "Object.h"
#include "config.h"

Object::Object() : QGraphicsPixmapItem(0) {
	_collidable = true;

	setShapeMode(QGraphicsPixmapItem::MaskShape);
}
