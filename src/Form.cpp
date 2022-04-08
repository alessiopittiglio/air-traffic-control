#include "Form.h"

#include <QFormLayout>

Form::Form() {
	_routeLabel = new QLabel("Number of air routes in a sector");
	_routeSpinBox = new QSpinBox;
	_routeSpinBox->setMinimum(1);
	_routeSpinBox->setValue(4);

	_airportLabel = new QLabel("Number of airport");
	_airportSpinBox = new QSpinBox;
	_airportSpinBox->setMinimum(2);
	_airportSpinBox->setValue(10);

	_maxDegreeLabel = new QLabel("Max number of incoming route onto an airport");
	_maxDegreeSpinBox = new QSpinBox;
	_maxDegreeSpinBox->setMinimum(1);
	_maxDegreeSpinBox->setValue(4);

	_maxCapacityLabel = new QLabel("Max capacity");
	_maxCapacitySpinBox = new QSpinBox;
	_maxCapacitySpinBox->setMinimum(2);
	_maxCapacitySpinBox->setValue(5);

	_okButton = new QPushButton("&Ok");

	QFormLayout* layout = new QFormLayout;
	layout->addRow(_routeLabel, _routeSpinBox);
	layout->addRow(_airportLabel, _airportSpinBox);
	layout->addRow(_maxDegreeLabel, _maxDegreeSpinBox);
	layout->addRow(_maxCapacityLabel, _maxCapacitySpinBox);
	layout->addRow(_okButton);
	setLayout(layout);

	setMinimumSize(400, 150);
	setWindowIcon(QIcon(":/graphics/icons/pinion_lightgray.png"));
	setWindowTitle(tr("Parameter Form"));

	_mainWindow = new MainWindow;
	connect(_mainWindow, SIGNAL(notifyForm()), this, SLOT(showForm()));
	connect(_okButton, SIGNAL(clicked()), this, SLOT(okButtonClicked()));
}

void Form::okButtonClicked() {
	if (_routeSpinBox->value() < _maxDegreeSpinBox->value()) {
		QMessageBox messageBox;
		messageBox.critical(this, tr("Error occured"), tr("The maximal degree must be inferior of the number of air routes in a sector"));
		return;
	}

	Map::instance()->setNumRoute(_routeSpinBox->value());
	Map::instance()->setNumAirport(_airportSpinBox->value());
	Map::instance()->setMaxDegree(_maxDegreeSpinBox->value());
	Map::instance()->setMaxCapacity(_maxCapacitySpinBox->value());
	Simulation::instance()->reset();

	hide();
	_mainWindow->show();
}

void Form::showForm() {
	this->show();
}
