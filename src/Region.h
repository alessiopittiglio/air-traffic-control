#pragma once

#include <QPolygon>
#include <QLine>
#include <set>

class Region {
private:
    QPolygon _shape;
    std::set<Region*> _neighbors;
    bool _destroy;
    QRect _rect;

    int _top, _left, _bottom, _right;

    float* _vertx;
    float* _verty;

public:
    std::vector<QLine> _routes; // it could be a set so we don't have duplicates

    // constructor / deconstructor
    Region(QRect _rect, std::vector<QLine> _routes);
    ~Region();

    // merge with 'r'
    void mergeWith(Region* r);

    // return true if current region is adjacent with 'r'
    bool isAdjacent(Region* r);

    void insertNeighbor(Region* r);  

    void vertex();

    // getters
    QPolygon shape() { return _shape; }

    float* vertx() { return _vertx; }
    float* verty() { return _verty; }

    int top() { return _top; }
    int left() { return _left; }
    int bottom() { return _bottom; }
    int right() { return _right; }

    bool destroy() { return _destroy; }

    QRect rect() { return _rect; }

    std::vector<QLine> routes() { return _routes; }
    std::set<Region*> neighbors() { return _neighbors; };
};

// merge method
void merge(std::vector <Region*>& regions, bool (*predicate)(const int, Region*, Region*));
