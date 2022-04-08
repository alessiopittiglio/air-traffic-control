#pragma once
#include "Controller.h"
#include "Map.h"
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTimer>

class Simulation : public QGraphicsView {
	Q_OBJECT

	enum simulation_state { READY, RUNNING, PAUSE };

private:
	static Simulation* uniqueInstance;		// singleton design pattern
	Simulation(QGraphicsView* parent = 0);		// singleton design pattern

	QGraphicsScene* _scene;
	QTimer _engine; // simulation engine
	simulation_state _state;

	void init();
	void setRandomTime();

	double _waitingTime, _timer;
	bool spawned;

	int _sectorsCongested;

	Controller* _controller;
public:
	// singleton
	static Simulation* instance();

	// getters
	QGraphicsScene* scene() { return _scene; }
	std::vector<Plane*> planes();

	void spawn();
public slots:
	// advance simulation
	void advance();

	// start simulation
	void start();

	// pause simulation
	void pause();

	// reset simulation
	void reset();

	// AI has moved plane at the given speed and altitude
	void AIMove(QString id, int speed, int altitude);
};
