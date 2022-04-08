#include "Region.h"
#include "Map.h"

Region::Region(QRect rect, std::vector<QLine> routes) : _routes(routes) {
    _rect = rect;
    _destroy = false;
    _shape = _rect;
    _top = _rect.top();
    _left = _rect.left();
    _bottom = _rect.bottom();
    _right = _rect.right();
    _vertx = nullptr;
    _verty = nullptr;
}

// deconstructor
Region::~Region() {
    // nothing to do
}

// merge with 'r'
void Region::mergeWith(Region *r) {
    // do nothing if r is not set
    if (!r)
        return;

    // merge shape
    r->_shape = r->_shape.united(this->_shape);

    // merge neighbors
    for (auto n : _neighbors)
        if (n != r)
            r->_neighbors.insert(n);

    // merge routes
    for (auto route : _routes)
        r->_routes.push_back(route);
    auto end = r->_routes.end();
    for (auto it = r->_routes.begin(); it != end; ++it)
        end = std::remove(it + 1, end, *it);
    r->_routes.erase(end, r->_routes.end());

    // update top, left, bottom, up
    for (int i = 0; i < r->_shape.size(); i++) {
        if (r->_shape[i].y() < _top)
            _top = r->_shape[i].y();
        if (r->_shape[i].x() < _left)
            _left = r->_shape[i].x();
        if (r->_shape[i].y() > _bottom)
            _bottom = r->_shape[i].y();
        if (r->_shape[i].x() > _right)
            _right = r->_shape[i].x();
    }

    // remove current region from other neighbors lists
    for (auto n : _neighbors)
        n->_neighbors.erase(this);

    // schedule for destroy
    _destroy = true;
}

// return true if current region is adjacent with 'r'
bool Region::isAdjacent(Region* r) {
    // for x-axis
    if (_left == r->_left || _left == r->_right || _right == r->_left || _right == r->_right) {
        // use only < when comparing top and r->bottom avoids sharing only a corner
        if (_top >= r->_top && _top < r->_bottom)
            return true;
        if (_bottom > r->_top && _bottom <= r->_bottom)
            return true;
        if (r->_top >= _top && r->_top < _bottom)
            return true;
        if (r->_bottom > _top && r->_bottom <= _bottom)
            return true;
    }

    // for y-axis
    if (_top == r->_top || _top == r->_bottom || _bottom == r->_top || _bottom == r->_bottom) {
        if (_left >= r->_left && _left < r->_right)
            return true;
        if (_right > r->_left && _right <= r->_right)
            return true;
        if (r->_left >= _left && r->_left < _right)
            return true;
        if (r->_right > _left && r->_right <= _right)
            return true;
    }

    return false;
}

void Region::insertNeighbor(Region* r) {
    _neighbors.insert(r);
}

void Region::vertex() {
    int dim = shape().size();

    _vertx = new float[dim];
    _verty = new float[dim];

    for (int z = 0; z < dim; z++)
        _vertx[z] = float(shape().at(z).x());
    for (int z = 0; z < dim; z++)
        _verty[z] = float(shape().at(z).y());

    // update top, left, bottom, up
    _left = shape().boundingRect().left();
    _right = shape().boundingRect().right();
    _top = shape().boundingRect().top();
    _bottom = shape().boundingRect().bottom();
}

void merge(std::vector <Region*>& regions, bool (*predicate)(const int, Region*, Region*)) {
    // find neighbors of each region
    for (int i = 0; i < regions.size(); i++)
        for (int j = 0; j < i; j++)
            if (regions[i]->isAdjacent(regions[j])) {
                regions[i]->insertNeighbor(regions[j]);
                regions[j]->insertNeighbor(regions[i]);
            }

    // go on with merging until no more merging occurs
    bool merged;
    do {
        // try to merge neighboring regions
        merged = false;
        for (auto r1 : regions) {
            for (auto r2 : r1->neighbors()) {
                // skip if one of the two regions is scheduled for destroy
                if (r1->destroy() || r2->destroy())
                    continue;

                // merge if predicate function returns true
                if (predicate(Map::instance()->numRoute(), r1, r2)) {
                    merged = true;
                    r1->mergeWith(r2);
                }
            }
        }

        // destroy regions that have been merged into other region
        for (auto it = regions.begin(); it != regions.end();) {
            if ((*it)->destroy())
                it = regions.erase(it);
            else
                ++it;
        }
    } while (merged);
}
