#pragma once

#include "MainWindow.h"
#include "Map.h"

#include <QWidget>
#include <QLabel>
#include <QSpinBox>
#include <QPushButton>
#include <QMessageBox>

class Form : public QWidget {
    Q_OBJECT
private:
    QLabel* _routeLabel;
    QSpinBox* _routeSpinBox;

    QLabel* _airportLabel;
    QSpinBox* _airportSpinBox;

    QLabel* _maxDegreeLabel;
    QSpinBox* _maxDegreeSpinBox;

    QLabel* _maxCapacityLabel;
    QSpinBox* _maxCapacitySpinBox;

    QPushButton* _okButton;

    MainWindow* _mainWindow;

public:
    Form();
public slots:
    void okButtonClicked();
    void showForm();
};
