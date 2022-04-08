#include "Map.h"
#include "Plane.h"
#include "QuadTree.h"
#include "Region.h"
#include "utils.h"

// singleton design pattern
Map* Map::instance() {
    static Map uniqueInstance;
    return &uniqueInstance;
}

// merging predicate
bool aMergingPredicate(const int maxNumRoute, Region* r1, Region* r2) {
    std::vector<QLine> tmp = r1->_routes;
    
    for (auto route : r2->_routes)
        r1->_routes.push_back(route);

    // remove duplicates
    auto end = r1->_routes.end();
    for (auto it = r1->_routes.begin(); it != end; ++it)
        end = std::remove(it + 1, end, *it);
    r1->_routes.erase(end, r1->_routes.end());

    if (r1->_routes.size() <= maxNumRoute)
        return true;
    else
        r1->_routes = tmp;

    return false;
}

void Map::load(QGraphicsScene* scene) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> x(0, scene->width() - RADIUS_NODE * 2);
    std::uniform_int_distribution<> y(0, scene->height() - RADIUS_NODE * 2);

    int numberDrawnAirports = 0, numAttempts = 0;

    for (int i = 0; i < _numAirport; i++) {
        QPoint temp(x(gen), y(gen));

        if (i == 0) {
            _nodes.push_back(temp);
            scene->addEllipse(temp.x(), temp.y(), RADIUS_NODE * 2, RADIUS_NODE * 2, Qt::NoPen, QBrush(QColor(0, 0, 0)));
            numberDrawnAirports = 1;
        }
        else {
            for (auto& j : _nodes) {
                if (distance(j, temp) < D_MIN) {
                    numAttempts++;
                    if (numAttempts < 100000) {
                        i--;
                        break;
                    }

                    if (numAttempts == 100000) {
                        int myRes = QMessageBox::question(0, "Warning", "Not enough space for " + 
                                    QString::number(_numAirport) + " airports. \nStart the simulation with " +
                                    QString::number(numberDrawnAirports) + " airports?", QMessageBox::Yes | QMessageBox::No);

                        if (myRes == QMessageBox::No) {
                            QApplication::quit();
                            std::exit(EXIT_SUCCESS);
                        }
                        _numAirport = numberDrawnAirports;
                        break;
                    }
                }
                else if (j == _nodes.back()) {
                    _nodes.push_back(temp);
                    scene->addEllipse(temp.x(), temp.y(), RADIUS_NODE * 2, RADIUS_NODE * 2, Qt::NoPen, QBrush(QColor(0, 0, 0)));
                    numberDrawnAirports++;
                    numAttempts = 0;
                }
            }
        }
    }

    for (int i = 0; i < _nodes.size(); i++)
        for (int j = 0; j < i; j++)
            if (distance(_nodes[i], _nodes[j]) <= D_MAX) {
                QPoint p1(_nodes[i].x() + RADIUS_NODE, _nodes[i].y() + RADIUS_NODE);
                QPoint p2(_nodes[j].x() + RADIUS_NODE, _nodes[j].y() + RADIUS_NODE);

                if (degree(p1) < _maxDegree && degree(p2) < _maxDegree) {
                    _lines.push_back(QLine(p1, p2));
                    scene->addLine(p1.x(), p1.y(), p2.x(), p2.y(), QPen(QBrush(QColor(150, 150, 150)), 2));
                }
            }

    _quadtree = new QuadTree(_numRoute, QPoint(0, 0), QPoint(scene->width(), scene->height()));

    for (int i = 0; i < _lines.size(); i++)
        _quadtree->insert(_lines[i]);

    drawSector(scene, _quadtree);

    std::vector<QuadTree*> leaves;
    _quadtree->getLeaves(leaves);

    // generate regions for merging
    for (auto leaf : leaves)
        _regions.push_back(new Region(leaf->getBoundary(), leaf->getLines()));

    // run merging
    merge(_regions, aMergingPredicate);

    // display result
    for (auto r : _regions) {
        r->vertex();
        _sectorPostMerge.push_back(scene->addPolygon(r->shape(), QPen(QBrush(QColor(0, 112, 192)), 2)));
    }
        
    for (auto r : _regions) {
        _sectorBackground.push_back(scene->addPolygon(r->shape(), Qt::NoPen, QBrush(QColor(255, 0, 0, 50))));
        _sectorBackground.back()->setVisible(false);
    }

    // make graph not visible by default
    toogleGraph();
}

int Map::degree(QPoint p) {
    int count = 0;

    for (int i = 0; i < _lines.size(); i++) {
        if (_lines[i].p1() == p)
            count++;

        if (_lines[i].p2() == p)
            count++;
    }

    return count;
}

void Map::drawSector(QGraphicsScene* scene, QuadTree* quadtree) {
    _sectorPreMerge.push_back(scene->addRect(quadtree->getBoundary(), QPen(QBrush(QColor(255, 0, 0)), 2)));

    // terminate here, if there are no children
    if (quadtree->ptrNorthWest() == nullptr)
        return;

    drawSector(scene, quadtree->ptrNorthWest());
    drawSector(scene, quadtree->ptrNorthEast());
    drawSector(scene, quadtree->ptrSouthWest());
    drawSector(scene, quadtree->ptrSouthEast());
}

void Map::spawnPlanes(QGraphicsScene* scene) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 1);

    for (auto l : _lines) {
        if (dis(gen) < _spawnProbability * distance(l.p1(), l.p2()) / D_MAX) {
            Plane* p = new Plane(l.p1() - QPoint(RADIUS_NODE * 3, RADIUS_NODE * 3), l.p2() - QPoint(RADIUS_NODE * 3, RADIUS_NODE * 3));
            
            scene->addItem(p);
            scene->addItem(new Tag(p));
        }
            
        if (dis(gen) < _spawnProbability * distance(l.p1(), l.p2()) / D_MAX) {
            Plane* p = new Plane(l.p2() - QPoint(RADIUS_NODE * 3, RADIUS_NODE * 3), l.p1() - QPoint(RADIUS_NODE * 3, RADIUS_NODE * 3));

            scene->addItem(p);
            scene->addItem(new Tag(p));
        }
    }
}

void Map::clean() {
    _nodes.clear();
    _lines.clear();
    _quadtree->clear();

    for (auto s : _sectorPreMerge)
        s = nullptr;
    _sectorPreMerge.clear();

    for (auto s : _sectorBackground)
        s = nullptr;
    _sectorBackground.clear();

    for (auto s : _sectorPostMerge)
        s = nullptr;
    _sectorPostMerge.clear();

    for (auto r : _regions)
        r = nullptr;
    _regions.clear();
}

void Map::backgroundVisible(Region* r, QGraphicsScene* scene) {
    QPainterPath path;
    path.addPolygon(r->shape());

    for (auto s : _sectorBackground)
        if (s->shape() == path)
            s->setVisible(true);
}

void Map::backgroundNotVisible(Region* r, QGraphicsScene* scene) {
    QPainterPath path;
    path.addPolygon(r->shape());

    for (auto s : _sectorBackground) {
        if (s->shape() == path)
            if(s->isVisible())
                s->setVisible(false);
    }
}
