#include <QVBoxLayout>
#include "MainWindow.h"

MainWindow::MainWindow() : QWidget(0) {
	_menuBar = new QMenuBar();

	_fileMenu = new QMenu("File");
	_newAction = new QAction("New");
	_fileMenu->addAction(_newAction);

	_simulationMenu = new QMenu("Simulation");
	_runAction = new QAction(QIcon(":/graphics/icons/play_green.png"), "Run");
	_runAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_R));
	_simulationMenu->addAction(_runAction);
	_pauseAction = new QAction(QIcon(":/graphics/icons/pause.png"), "Pause");
	_pauseAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_P));
	_simulationMenu->addAction(_pauseAction);
	_stopAction = new QAction(QIcon(":/graphics/icons/stop.png"), "Stop");
	_stopAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));
	_simulationMenu->addAction(_stopAction);

	_settingsMenu = new QMenu("Settings");
	_mergeToggle = new QAction("Show Merge");
	_mergeToggle->setCheckable(true);
	_mergeToggle->setChecked(false);
	_settingsMenu->addAction(_mergeToggle);

	_helpMenu = new QMenu("Help");
	_aboutAction = new QAction(QIcon(":/graphics/icons/info.png"), "About");
	_helpMenu->addAction(_aboutAction);

	_menuBar->addMenu(_fileMenu);
	_menuBar->addMenu(_simulationMenu);
	_menuBar->addMenu(_settingsMenu);
	_menuBar->addMenu(_helpMenu);

	_fpsEdit = new QLineEdit(QString::number(FPS) + " fps");
	_fpsEdit->setMaximumWidth(80);
	_fpsEdit->setAlignment(Qt::AlignCenter);
	_fpsEdit->setDisabled(true);

	_placeholder = new QWidget();
	_placeholder->setMinimumWidth(10);

	_probabilityLabel = new QLabel("Probability: ");

	_probabilitySlider = new QSlider(Qt::Horizontal);
	_probabilitySlider->setMaximumWidth(200);
	_probabilitySlider->setPageStep(20);

	_toolBar = new QToolBar;
	_toolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	_toolBar->setIconSize(QSize(24, 24));
	_toolBar->addAction(_runAction);
	_toolBar->addAction(_pauseAction);
	_toolBar->addAction(_stopAction);
	_toolBar->addSeparator();
	_toolBar->addWidget(_fpsEdit);
	_toolBar->addWidget(_placeholder);
	_toolBar->addWidget(_probabilityLabel);
	_toolBar->addWidget(_probabilitySlider);

	_simulation = Simulation::instance();

	_delayLabel = new QLabel("Global delay: 0.0 hr");
	_delayLabel->setAlignment(Qt::AlignCenter);
	_delayLabel->setMinimumWidth(200);

	_statusBar = new QStatusBar();
	_statusBar->addWidget(_delayLabel);

	QVBoxLayout* layout = new QVBoxLayout();
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);
	layout->addWidget(_menuBar);
	layout->addWidget(_toolBar);
	layout->addWidget(_simulation);
	layout->addWidget(_statusBar);
	setLayout(layout);

	setWindowIcon(QIcon(":/graphics/icons/linear.png"));
	setWindowTitle(tr("Air Traffic Control"));
	showMaximized();

	connect(_newAction, SIGNAL(triggered()), this, SLOT(newActionTriggered()));
	connect(_runAction, SIGNAL(triggered()), this, SLOT(runActionTriggered()));
	connect(_pauseAction, SIGNAL(triggered()), this, SLOT(pauseActionTriggered()));
	connect(_stopAction, SIGNAL(triggered()), this, SLOT(stopActionTriggered()));
	connect(_aboutAction, SIGNAL(triggered()), this, SLOT(aboutActionTriggered()));
	connect(_probabilitySlider, SIGNAL(valueChanged(int)), this, SLOT(sliderValueChanged(int)));
	connect(_mergeToggle, SIGNAL(toggled(bool)), this, SLOT(mergeToggled(bool)));
	connect(Controller::instance(), SIGNAL(notifyGlobalDelay(float)), this, SLOT(globalDelayChanged(float)));

	sliderValueChanged(0);
}

void MainWindow::newActionTriggered() {
	_simulation->pause();
	_runAction->setEnabled(true);
	_pauseAction->setEnabled(true);
	hide();
	emit notifyForm();
}

void MainWindow::runActionTriggered() {
	_simulation->start();
}

void MainWindow::pauseActionTriggered() {
	_simulation->pause();
}

void MainWindow::stopActionTriggered() {
	_simulation->pause();
	_runAction->setDisabled(true);
	_pauseAction->setDisabled(true);
}

void MainWindow::aboutActionTriggered() {
	QString text("<html><h1>Air Traffic Control version ");
	text = text + "1.0.0" + "</h1>"
		"<big>A tool for the simulation and automatic management of short-range air traffic control </big><br><br>"
		"Developed by <i>Michele Ferrazzano, Alessio Pittiglio, Stefano Pittiglio</i> "
		"under the supervision of Prof. Alessandro Bria (email: <a href=\"mailto:a.bria@unicas.it\">a.bria@unicas.it</a>)</li><br><br>"
		"For further info, please visit our <a href=\"https://github.com/alessiopittiglio/air-traffic-control\">website</a>.<br>"
		"<br><small>&copy; 2021 University of Cassino and Southern Lazio</small></html>";

	QMessageBox::about(this, "About Air Traffic Control", text);
}

void MainWindow::mergeToggled(bool checked) {
	Map::instance()->toogleGraph();
}

void MainWindow::sliderValueChanged(int p) {
	Map::instance()->setProbability(double(p / 100.0));
}

void MainWindow::globalDelayChanged(float f) {
	_delayLabel->setText("Global delay: " + QString::number(f, f, 2) + " hr");
}
