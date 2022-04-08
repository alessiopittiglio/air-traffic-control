#include "Controller.h"
#include "Plane.h"
#include "Simulation.h"
#include "Timer.h"
#include "config.h"
#include "utils.h"

Controller* Controller::instance() {
	static Controller unique_instance;
	return &unique_instance;
}

Controller::Controller() {
	_state = FREE;
	_collision = false;
	_sectorsCongested = 0;
	_delay = 0.0;
	_globalDelay = 0.0;
	_alpha = 10.0;
	_beta = 1.0;
	_index = 0;

	// populate moves
	int _sizeSpeed = (MAX_SPEED - MIN_SPEED) / DELTA_SPEED + 1;
	int _sizeAltitude = (MAX_ALTITUDE - MIN_ALTITUDE) / DELTA_ALTITUDE + 1;

	for (int i = 0; i < _sizeSpeed; i++)
		_speed.push_back(MIN_SPEED + DELTA_SPEED * i);
	for (int i = 0; i < _sizeAltitude; i++)
		_altitudes.push_back(MIN_ALTITUDE + DELTA_ALTITUDE * i);
}

void Controller::restorePlanes() {
	for (auto _plane : _bpkPlanes)
		delete _plane;
	_bpkPlanes.clear();

	for (auto plane : _planes) {
		Plane* plane_copy = new Plane(plane->nodeDeparture(), plane->nodeArrival(), false);
		plane_copy->setName(plane->name());
		plane_copy->setPos(plane->pos());
		plane_copy->setSpeed(plane->speed());
		plane_copy->setAltitude(plane->altitude());
		_bpkPlanes.push_back(plane_copy);
	}
}

float Controller::utility()
{
	if (_collision)
		return 100;
	else
		return _alpha * (_sectorsCongested / float(NTICKS)) + _beta * _delay;
}

int findIndex(std::vector<Plane*> vec, std::string str) {
	int z = 0, count = 0;
	for (auto plane : vec) {
		if (str == plane->name())
			z = count;
		count++;
	}
	return z;
}

void Controller::run() {
	// nothing to do if controller is busy
	if (_state != FREE)
		return;
	_state = BUSY;

	for (auto plane : Simulation::instance()->planes()) {
		Plane* plane_copy = new Plane(plane->nodeDeparture(), plane->nodeArrival(), false);
		plane_copy->setName(plane->name());
		plane_copy->setPos(plane->pos());
		plane_copy->setSpeed(plane->speed());
		plane_copy->setAltitude(plane->altitude());
		_planes.push_back(plane_copy);

		// load the buffer
		if (std::find(_buffer.begin(), _buffer.end(), QString::fromStdString(plane->name())) == _buffer.end())
			_buffer.push_back(QString::fromStdString(plane->name()));
	}

	// play random simulations to estimate average expected scores for each move
	int best_speed = 0, best_altitude = 0;
	float best_move_delay = 0.0;
	float best_move_score = std::numeric_limits<float>::infinity();
	Timer timer;
	Plane* p;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, int(_planes.size() - 1));

	// remove obsolete plane
	auto it = _buffer.begin();
	while (it != _buffer.end()) {
		bool trovato = false;
		for (auto p2 : _planes) {
			if (*it == QString::fromStdString(p2->name()))
				trovato = true;
		}
		if (!trovato)
			it = _buffer.erase(it);
		else {
			++it;
		}
	}
	if (_index >= int(_buffer.size()))
		_index = _index % int(_buffer.size());

	// init circular buffer
	p = _planes.at(findIndex(_planes, _buffer.at(_index).toStdString()));

	QPoint nodeDeparture = p->nodeDeparture().toPoint();
	QPoint nodeArrival = p->nodeArrival().toPoint();
	float time_expected = distance(nodeDeparture, nodeArrival) / float(DEFAULT_SPEED);

	for (auto& s : _speed)
		for (auto& a : _altitudes) {
			// check delay
			float time_real = distance(nodeDeparture, nodeArrival) / float(s);
			_delay = time_real - time_expected;
			if (_delay < 0.0) // a negative delay is not a delay
				_delay = 0.0;

			// do current move
			p->setSpeed(s);
			p->setAltitude(a);

			float score_avg = 0;
			for (int i = 0; i < 1; i++) { 
				// restore planes state
				restorePlanes();

				// find the index of the selected plane
				int index = findIndex(_bpkPlanes, p->name());

				for (unsigned int i = NTICKS; i > 0; --i) {
					for (auto plane : _bpkPlanes)
						plane->advance();

					// check for collisions between entities
					for (auto other : _bpkPlanes) {
						if (
							other != _bpkPlanes.at(index)
							&& nodeDeparture != other->nodeDeparture().toPoint()
							&& nodeArrival != other->nodeArrival().toPoint()
							) {

							QPoint other_point = QPoint(other->pos().x(), other->pos().y());

							if (
								abs(_bpkPlanes.at(index)->altitude() - other->altitude()) < DMINVERTCOL
								&& QRect(other_point - QPoint(20, 20), other_point + QPoint(20, 20)).contains(QPoint(_bpkPlanes.at(index)->pos().x(), _bpkPlanes.at(index)->pos().y()))
								)
								_collision = true;
						}
					}
					if (_collision == true)
						break;

					// check for congestions
					for (auto r : Map::instance()->regions()) {
						int capacity = 0;
						for (auto plane : _bpkPlanes) {
							// preliminary check
							if (
								!(plane->pos().x() < r->left() || plane->pos().x() > r->right() 
								|| plane->pos().y() < r->top() || plane->pos().y() > r->bottom())
								)
								if (pnpoly(r->shape().size(), r->vertx(), r->verty(), plane->pos().x(), plane->pos().y()))
									capacity++;
						}

						if (capacity > Map::instance()->maxCapacity())
							_sectorsCongested++;
					}
				}
				score_avg += utility();
			}

			// update best move
			score_avg /= 1;
			if (score_avg < best_move_score) {
				best_move_score = score_avg;
				best_speed = s;
				best_altitude = a;
				best_move_delay = _delay;
			}

			// undo current move
			_collision = false;
			_sectorsCongested = 0;
		}

	_globalDelay += best_move_delay;

	// printf("AI moved in %.1f seconds, best average expected score = %.5f\n", timer.elapsed<float>(), best_move_score);
	printf("Time: %.1f secons, Name plane: %s, best speed: %u, best altitude: %u\n", timer.elapsed<float>(), p->name().c_str(), best_speed, best_altitude);

	// notify move
	emit notifyAIMove(QString::fromStdString(p->name()), best_speed, best_altitude);
	emit notifyGlobalDelay(_globalDelay);

	// deallocate containers 
	for (auto pln : _planes)
		delete pln;
	_planes.clear();

	_index++;
	if (_index == _buffer.size())
		_index = 0;
	
	_state = FREE;
}
