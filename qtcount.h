#ifndef _QTCOUNT_H_
#define _QTCOUNT_H_

#include <cmath>
#include <utility>

#include "cs221util/PNG.h"
#include "cs221util/RGBAPixel.h"
#include "quadtree.h"

using namespace std;
using namespace cs221util;

class qtcount : public quadtree {
public:
    qtcount(PNG& im) : quadtree(im) {}

private:
    bool prunable(Node* node, const int tol) const;

    // bool getColorDist(qtcount::Node *& node, const int tol, qtcount::Node *& subtree);

    // vector<Node *> getLeaf(Node * node, vector<Node *> leaves) const;

    bool prunableHelper(Node * node, RGBAPixel avg, int tol) const;
};

#endif