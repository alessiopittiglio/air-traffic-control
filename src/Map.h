#pragma once

#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QMessageBox>
#include <random>

#include "QuadTree.h"
#include "Region.h"
#include "Tag.h"
#include "config.h"

class Map {
private:
    int _numAirport;
    int _numRoute;
    int _maxDegree;
    int _maxCapacity;
    double _spawnProbability;

    std::vector<QPoint> _nodes; // graph nodes
    std::vector<QLine> _lines;

    QuadTree* _quadtree;

    std::vector <QGraphicsItem*> _sectorPreMerge;   // graph for visual debug
    std::vector <QGraphicsItem*> _sectorBackground;
    std::vector <QGraphicsItem*> _sectorPostMerge;
    std::vector<Region*> _regions;

    int degree(QPoint p);
public:
    // singleton design pattern
    static Map* instance();

    // setters
    void setNumAirport(int numAirport) { _numAirport = numAirport; }
    void setNumRoute(int numRoute) { _numRoute = numRoute; }
    void setMaxDegree(int maxDegree) { _maxDegree = maxDegree; }
    void setProbability(double spawnProbability) { _spawnProbability = spawnProbability; }
    void setMaxCapacity(int maxCapacity) { _maxCapacity = maxCapacity; }

    // load all items in the given scene
    void load(QGraphicsScene* scene);

    void drawSector(QGraphicsScene* scene, QuadTree* sector);
    void spawnPlanes(QGraphicsScene* scene);
    void backgroundVisible(Region* r, QGraphicsScene* scene);
    void backgroundNotVisible(Region* r, QGraphicsScene* scene);
    void clean();

    // getters
	int numRoute() { return _numRoute; }
	int maxCapacity() { return _maxCapacity; }
	double spawnProbability() { return _spawnProbability; }
    std::vector<QPoint> nodes() { return _nodes; }
    std::vector<QLine> lines() { return _lines; }
    std::vector<QGraphicsItem*> sectorPostMerge() { return  _sectorPostMerge; }
    std::vector<Region*> regions() { return _regions; }

    // tooglers
    void toogleGraph() {
        for (auto& i : _sectorPreMerge) 
            i->setVisible(!i->isVisible());

        if (_sectorPreMerge[0]->isVisible())
            for (auto& i : _sectorPostMerge) i->hide();
        else
            for (auto& i : _sectorPostMerge) i->show();
    }
};
