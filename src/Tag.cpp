#include "Tag.h"

Tag::Tag(Plane* p) : Entity(p->nodeDeparture(), p->nodeArrival()) {
	// set flags
	_collidable = false;

	_textureStand = QPixmap(loadTexture(":/graphics/tag.png"));

	_idText = QString(p->name().c_str());
	_altitudeText = QString::number(p->altitude() / 1000, 'f', 1);
	_speedText = QString::number(p->speed());

	_nodeDeparture = p->nodeDeparture() - QPointF(0, 50);
	_nodeArrival = p->nodeArrival() - QPointF(0, 50);

	QPainter painter(&_textureStand);
	QFont font("Calibri");

	QString _text = QString(_idText);
	font.setBold(true);
	painter.setFont(font);
	painter.drawText(_textureStand.rect().translated(0, -10), Qt::AlignCenter, _text);

	_text = QString("\n" + _speedText + " " + _altitudeText);
	font.setBold(false);
	painter.setFont(font);
	painter.drawText(_textureStand.rect(), Qt::AlignCenter, _text);

	// set texture
	setPixmap(_textureStand);

	// spawn position	
	setPos(_nodeDeparture);

	_namePlane = QString::fromStdString(p->name());
}

void Tag::updateTexture() {
	_textureStand = QPixmap(loadTexture(":/graphics/tag.png"));
	QPainter painter(&_textureStand);
	QFont font("Calibri");

	QString _text = QString(_idText);
	font.setBold(true);
	painter.setFont(font);
	painter.drawText(_textureStand.rect().translated(0, -10), Qt::AlignCenter, _text);

	_text = QString("\n" + _speedText + " " + _altitudeText);
	font.setBold(false);
	painter.setFont(font);
	painter.setPen(Qt::red);
	painter.drawText(_textureStand.rect(), Qt::AlignCenter, _text);

	setPixmap(_textureStand);
}

void Tag::setAltitudeText(QString altitude) {
	_altitudeText = altitude;
	updateTexture();
}

void Tag::setSpeedText(QString speed) {
	_speedText = speed;
	updateTexture();
}
