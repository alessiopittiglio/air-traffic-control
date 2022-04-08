#include "QuadTree.h"

QuadTree::QuadTree(int numRoute, QPoint topLeft, QPoint botRight) : QT_NODE_CAPACITY(numRoute) {
    boundary = QRect(topLeft, botRight);
    northWest = northEast = southWest = southEast = nullptr;
}

// insert a line into the QuadTree
bool QuadTree::insert(QLine l) {
    // ignore objects that do not belong in this quad tree
    if (!containsLine(boundary, l))
        return false;

    // if there is space in this quad tree and if doesn't have subdivisions, add the object here
    if (lines.size() < QT_NODE_CAPACITY && northWest == nullptr) {
        lines.push_back(l);
        return true;
    }

    // otherwise, subdivide and then add the line to whichever node will accept it
    if (northWest == nullptr) {
        subdivide();

        // redistributes parent's routes to children
        for (int i = 0; i < this->lines.size(); i++) {
            northWest->insert(this->lines[i]);
            northEast->insert(this->lines[i]);
            southWest->insert(this->lines[i]);
            southEast->insert(this->lines[i]);
        }
    }

    northWest->insert(l);
    northEast->insert(l);
    southWest->insert(l);
    if (southEast->insert(l))
        return true;

    // otherwise, the line cannot be inserted for some unknown reason (this should never happen)
    return false;
}

void QuadTree::subdivide() {
    int newX = (boundary.topLeft().x() + boundary.bottomRight().x()) / 2;
    int newY = (boundary.topLeft().y() + boundary.bottomRight().y()) / 2;

    northWest = new QuadTree(QT_NODE_CAPACITY, QPoint(boundary.topLeft().x(), boundary.topLeft().y()), QPoint(newX, newY));
    northEast = new QuadTree(QT_NODE_CAPACITY, QPoint(newX, boundary.topLeft().y()), QPoint(boundary.bottomRight().x(), newY));
    southWest = new QuadTree(QT_NODE_CAPACITY, QPoint(boundary.topLeft().x(), newY), QPoint(newX, boundary.bottomRight().y()));
    southEast = new QuadTree(QT_NODE_CAPACITY, QPoint(newX, newY), QPoint(boundary.bottomRight().x(), boundary.bottomRight().y()));
}

void QuadTree::clear() {
    if (this == nullptr) 
        return;
    
    lines.clear();

    northWest->clear();
    delete northWest;
    northWest = nullptr;

    northEast->clear();
    delete northEast;
    northEast = nullptr;

    southWest->clear();
    delete southWest;
    southWest = nullptr;

    southEast->clear();
    delete southEast;
    southEast = nullptr;
}

void QuadTree::getLeaves(std::vector<QuadTree*>& leaves) {
    // stop when a leaf is found
    if (northWest != nullptr)
        northWest->getLeaves(leaves);
    else {
        leaves.push_back(this);
        return;
    }

    if (northEast != nullptr)
        northEast->getLeaves(leaves);

    if (southWest != nullptr)
        southWest->getLeaves(leaves);
       
    if (southEast != nullptr)
        southEast->getLeaves(leaves);
}
