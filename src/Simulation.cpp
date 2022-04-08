#include "Simulation.h"
#include "Plane.h"
#include "config.h"

// singleton design pattern
Simulation* Simulation::uniqueInstance = 0;
Simulation* Simulation::instance() {
	if (uniqueInstance == 0)
		uniqueInstance = new Simulation();
	return uniqueInstance;
}

Simulation::Simulation(QGraphicsView* parent) : QGraphicsView(parent) {
	_sectorsCongested = 0;

	// setup scene/view
	_scene = new QGraphicsScene();
	setScene(_scene);
	setInteractive(false); // disables events
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	// game engine
	QObject::connect(&_engine, SIGNAL(timeout()), this, SLOT(advance()));
	_engine.setInterval(1000 / FPS);
	_engine.setTimerType(Qt::PreciseTimer);

	// parameters
	_waitingTime = 0.0;
	_timer = 0.0;
	spawned = false;

	// connect simulation to AI
	connect(Controller::instance(), SIGNAL(notifyAIMove(QString, int, int)), this, SLOT(AIMove(QString, int, int)));
}

void Simulation::reset() {
	_state = READY;
	_engine.stop();
	_scene->clear();

	init();
}

void Simulation::init() {
	if (_state == READY) {
		// load map
		_scene->setSceneRect(0, 0, size().width(), size().height());
		Map::instance()->clean();
		Map::instance()->load(_scene);

		_timer = MAX_TIME;
	}
}

void Simulation::AIMove(QString name, int speed, int altitude) {
	for (auto item : _scene->items()) {
		Plane* plane = dynamic_cast<Plane*>(item);
		if (plane && QString::fromStdString(plane->name()) == name) {
			plane->setSpeed(speed);
			plane->setAltitude(altitude);
		}

		Tag* tag = dynamic_cast<Tag*>(item);
		if (tag && tag->namePlane() == name) {
			tag->setSpeed(speed);
			tag->setSpeedText(QString::number(speed));
			tag->setAltitudeText(QString::number(altitude / 1000, 'f', 1));
		}
	}
}

std::vector<Plane*> Simulation::planes() {
	std::vector<Plane*> planes;
	for (auto item : _scene->items()) {
		Plane* plane = dynamic_cast<Plane*>(item);
		if (plane)
			planes.push_back(plane);
	}

	return planes;
}

void Simulation::start() {
	_state = RUNNING;
	_engine.start();
}

void Simulation::pause() {
	if (_state == RUNNING) {
		_engine.stop();
		_state = PAUSE;
	}
}

void Simulation::setRandomTime() {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(MIN_TIME, MAX_TIME);

	_waitingTime = dis(gen);
}

void Simulation::spawn() {
	_timer += 1.0 / FPS;

	if (_timer > _waitingTime) {
		Map::instance()->spawnPlanes(_scene);
		_timer = 0.0;
		setRandomTime();
	}
}

void Simulation::advance() {
	if (_state != RUNNING)
		return;

	spawn();

	// tell all objects to advance in the scene
	for (auto item : _scene->items()) {
		Object* obj = dynamic_cast<Object*>(item);
		if (obj) {
			obj->advance();

			// destroy died Entity objects
			Entity* entity_obj = dynamic_cast<Entity*>(obj);
			if (entity_obj->isDead()) {
				_scene->removeItem(entity_obj);
				delete entity_obj;
			}
		}
	}

	if (Controller::instance()->isFree() && planes().size() != 0)
		Controller::instance()->start();

	// check for collisions between entities
	for (auto item1 : _scene->items()) {
		Plane* pln1 = dynamic_cast<Plane*>(item1);
		if (pln1) {
			for (auto item2 : _scene->items()) {
				Plane* pln2 = dynamic_cast<Plane*>(item2);
				if (
					pln2
					&& pln1 != pln2
					&& pln1->nodeDeparture().toPoint() != pln2->nodeDeparture().toPoint()
					&& pln1->nodeArrival().toPoint() != pln2->nodeArrival().toPoint()
					) {

					QPoint pln2_point = QPoint(pln2->pos().x(), pln2->pos().y());

					if (
						abs(pln1->altitude() - pln2->altitude()) < DMINVERTCOL
						&& QRect(pln2_point - QPoint(20, 20), pln2_point + QPoint(20, 20)).contains(QPoint(pln1->pos().x(), pln1->pos().y()))
						) {
						
						if (true) { // for debug
							Simulation::instance()->pause();
							if (
								QMessageBox::warning(0, "Warning", "Collision between " + QString::fromStdString(pln1->name())
									+ " and " + QString::fromStdString(pln2->name()), QMessageBox::Ok)
								)
								return;
						}
					}

				}
			}
		}
	}
		
	// check for congestions
	for (auto r : Map::instance()->regions()) {
		int capacity = 0;
		for (auto plane : planes()) {
			// preliminary check
			if (!(plane->pos().x() < r->left() || plane->pos().x() > r->right() 
				|| plane->pos().y() < r->top() || plane->pos().y() > r->bottom()))
				if (pnpoly(r->shape().size(), r->vertx(), r->verty(), plane->pos().x(), plane->pos().y()))
					capacity++;
		}

		if (capacity > Map::instance()->maxCapacity()) {
			Map::instance()->backgroundVisible(r, _scene);
			_sectorsCongested++;
		}
		else if (capacity == Map::instance()->maxCapacity())
			Map::instance()->backgroundNotVisible(r, _scene);
	}
}
