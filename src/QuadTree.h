#pragma once

#include <QLine>
#include <QRect>
#include <vector>

#include "utils.h"

class QuadTree {
private:
    // arbitrary constant to indicate how many elements can be stored in this quad tree node
    const int QT_NODE_CAPACITY;

    // axis-aligned bounding box stored 
    // to represent the boundaries of this quad tree
    QRect boundary;

    // lines in this quad tree node
    std::vector<QLine> lines;

    // children nodes
    std::vector <QuadTree*> children;

    // children
    QuadTree* northWest;
    QuadTree* northEast;
    QuadTree* southWest;
    QuadTree* southEast;

public:
    QuadTree(int numRoute, QPoint topLeft, QPoint botRight);
    bool insert(QLine p);
    void subdivide();

    void clear();

    std::vector<QLine> getLines() { return lines; };
    QRect getBoundary() { return boundary; };

    // get all leaves starting from current node
    void getLeaves(std::vector<QuadTree*>& leaves);

    QuadTree* ptrNorthWest() { return northWest; };
    QuadTree* ptrNorthEast() { return northEast; };
    QuadTree* ptrSouthWest() { return southWest; };
    QuadTree* ptrSouthEast() { return southEast; };
};
