#pragma once

#include <QWidget>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QToolBar>
#include <QLineEdit>
#include <QLabel>
#include <QSlider>
#include <QStatusBar>

#include "Simulation.h"
#include "config.h"

class MainWindow : public QWidget {
	Q_OBJECT
private:
	QMenuBar* _menuBar;

	QMenu* _fileMenu;
	QAction* _newAction;

	QMenu* _settingsMenu;
	QAction* _mergeToggle;

	QMenu* _simulationMenu;
	QAction* _runAction;
	QAction* _pauseAction;
	QAction* _stopAction;

	QMenu* _helpMenu;
	QAction* _aboutAction;

	QToolBar* _toolBar;
	QLineEdit* _fpsEdit;
	QWidget* _placeholder;
	QLabel* _probabilityLabel;
	QSlider* _probabilitySlider;

	Simulation* _simulation;

	QStatusBar* _statusBar;
	QLabel* _delayLabel;
public:
	MainWindow();

public slots:
	void newActionTriggered();
	void runActionTriggered();
	void pauseActionTriggered();
	void stopActionTriggered();
	void mergeToggled(bool checked);
	void aboutActionTriggered();
	void sliderValueChanged(int p);
	void globalDelayChanged(float d);
signals:
	void notifyForm();
};
