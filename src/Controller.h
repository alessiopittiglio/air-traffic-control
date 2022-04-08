#pragma once

#include <QThread>
#include <QGraphicsScene>
#include "Plane.h"

class Controller : public QThread {
	Q_OBJECT
	enum controller_state { BUSY, FREE };
private:
	Controller();

	controller_state _state;
	std::vector<Plane*> _planes;
	std::vector<int> _speed, _altitudes;
	std::vector<QString> _buffer;

	// parameters
	bool _collision;
	int _sectorsCongested;
	float _delay; // in hours
	float _globalDelay; // in hours
	float _alpha, _beta;
	int _index;

	float utility();

	std::vector<Plane*> _bpkPlanes;
	void restorePlanes();		// restore from backup planes

	void run();
public:
	static Controller* instance();
	bool isFree() { if (_state == FREE) return true; else return false; };
signals:
	// AI found the best move
	void notifyAIMove(QString id, int speed, int altitude);
	void notifyGlobalDelay(float global_delay);
};
