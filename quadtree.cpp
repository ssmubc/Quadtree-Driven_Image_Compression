/**
 *
 * quadtree (pa3)
 * quadtree.cpp
 * This file will be used for grading.
 *
 */

#include "quadtree.h"
#include <limits.h>
using namespace std;

// Node constructor, given.
quadtree::Node::Node(pair<int, int> ul, int d, RGBAPixel a, double v)
    : upLeft(ul), dim(d), avg(a), var(v), NW(nullptr), NE(nullptr), SE(nullptr), SW(nullptr) {}

// quadtree destructor, given.
quadtree::~quadtree() {
    clear();
}
// quadtree copy constructor, given.
quadtree::quadtree(const quadtree& other) {
    copy(other);
}
// quadtree assignment operator, given.
quadtree& quadtree::operator=(const quadtree& rhs) {
    if (this != &rhs) {
        clear();
        copy(rhs);
    }
    return *this;
}


quadtree::quadtree(PNG& imIn) {
    /* Your code here! */

    stats data_img = stats(imIn);
    pair<int, int> upperLeft = make_pair(0, 0);
    int dimension = log2(imIn.width());
    root = buildTree(data_img, upperLeft, dimension);
    edge = pow(2, dimension);
    
}



quadtree::Node* quadtree::buildTree(stats& s, pair<int, int> ul, int dim) {
    /* Your code here! */
    quadtree::Node * node = new quadtree::Node(ul, dim, s.getAvg(ul, dim), s.getVar(ul, dim));

    // do this recursively

    // base case: leaf node
    if (dim == 0) {      // the dimension is decremented by 1 as per instructions until it reaches 0, where 2^0 = 1
        return node;
    } else {
        int childDimension = dim - 1;
        node->NW = buildTree(s, ul, childDimension); // does not change ul because its top left is the same as parent
        node->NE = buildTree(s, make_pair(ul.first + pow(2, childDimension), ul.second), childDimension);
        node->SE = buildTree(s, make_pair(ul.first + pow(2, childDimension), ul.second + pow(2, childDimension)), childDimension);
        node->SW = buildTree(s, make_pair(ul.first, ul.second + pow(2, childDimension)), childDimension);
    }

    return node;
}

    /**
     * Render returns a PNG image consisting of the pixels
     * stored in the tree. It may be used on pruned trees. Draws
     * every leaf node's square onto a PNG canvas using the
     * average color stored in the node.
     */
PNG quadtree::render() const {
    /* Your code here! */
    if (root == NULL) {  // in the case where quadtree does not have any nodes
        return PNG();
    }
    PNG image = PNG((unsigned int) edge, (unsigned int) edge);
    renderHelperFunction(image, root);
    return image;
}

void quadtree::renderHelperFunction(PNG& image, quadtree::Node* node) const {   // will be a leaf node if if-cond is met
    if (node == NULL) return;

    if (node->NE == NULL && node->NW == NULL && node->SE == NULL && node->SW == NULL) {
        for(int i = node->upLeft.first; i < node->upLeft.first + pow(2, node->dim); i++) {
            for (int j = node->upLeft.second; j < node->upLeft.second + pow(2, node->dim); j++) {
                RGBAPixel * imPixel = image.getPixel(i, j);
                *imPixel = node->avg;
            }
        }
    } else {
        renderHelperFunction(image, node->NE);
        renderHelperFunction(image, node->NW);
        renderHelperFunction(image, node->SE);
        renderHelperFunction(image, node->SW);
    }
}



    /*
     * The idealPrune function can be considered to be the inverse
     * of the pruneSize function. It takes as input a number of leaves
     * and returns the minimum tolerance that would produce that resolution
     * upon a prune. It does not change the structure of the tree.
     */
    
int quadtree::idealPrune(const int leaves) const {
    /* Your code here! */

    if (!root->NW) return 0;

    int low = 0;
    int upper = 3*root->var;
    

    while (low < upper) {
        int mid = (upper + low) / 2;
        // if (pruneSize(mid) == leaves) {
        //     return mid;
        // } else if (pruneSize(mid) < leaves) {
        //     upper = mid - 1;
        // } else {
        //     low = mid + 1;
        // }
        if (pruneSize(mid) <= leaves) {
            upper = mid;
        } else {
            low = mid + 1;
        }
    }
    return upper;
}
    /*
     * The pruneSize function takes a tolerance as input, and returns
     * the number of leaves that would result if the tree were to
     * be pruned with that tolerance. Consistent with the definition
     * of prune, a node is counted if prunable returns true.
     *
     * You may want a recursive helper function for this one.
     */
int quadtree::pruneSize(const int tol) const {
    /* Your code here! */
    return pruneSizeHelperFunction(root, tol);
}

int quadtree::pruneSizeHelperFunction(Node * tree, int tol) const {
    // !!!
    if (!tree->NW) {
        return 1;
    }

    if (prunable(tree, tol)) {
        return 1;
    }
    // return size;
    return pruneSizeHelperFunction(tree->NE, tol) + pruneSizeHelperFunction(tree->NW, tol) + pruneSizeHelperFunction(tree->SE, tol) + pruneSizeHelperFunction(tree->SW, tol);
}



void quadtree::prune(const int tol) {
    /* Your code here! */
    pruneHelperFunction(root, tol);
}

void quadtree::pruneHelperFunction(Node *& subtree, int tol) {
    if (subtree == NULL) {
        return;
    }
    if (prunable(subtree, tol)) {
        clearNode(subtree->NW);
        clearNode(subtree->NE);
        clearNode(subtree->SE);
        clearNode(subtree->SW);
        subtree->NW = nullptr;
        subtree->NE = nullptr;
        subtree->SW = nullptr;
        subtree->SE = nullptr;
    } else {
        // !!!
        pruneHelperFunction(subtree->NW, tol);
        pruneHelperFunction(subtree->NE, tol);
        pruneHelperFunction(subtree->SW, tol);
        pruneHelperFunction(subtree->SE, tol);
    }
}

void quadtree::clearNode(Node *&subtree) {
    if (subtree == NULL) {
        return;
    }
    clearNode(subtree->NW);
    clearNode(subtree->NE);
    clearNode(subtree->SW);
    clearNode(subtree->SE);
    delete subtree;
    subtree = NULL;
}

void quadtree::clear() {
    /* your code here */
    clearHelperFunction(root);
    edge = 0;
}

void quadtree::clearHelperFunction(Node *subtree) {
    if (subtree == NULL) {
        return;
    }
    clearHelperFunction(subtree->NW);
    clearHelperFunction(subtree->NE);
    clearHelperFunction(subtree->SW);
    clearHelperFunction(subtree->SE);
    delete subtree;
    subtree = NULL;
}


    /**
     * Copies the parameter other quadtree into the current quadtree.
     * Does not free any memory. Called by copy constructor and op=.
     * You may want a recursive helper function for this one.
     * @param other The quadtree to be copied.
     */

// quadtree::Node::Node(pair<int, int> ul, int d, RGBAPixel a, double v)
//     : upLeft(ul), dim(d), avg(a), var(v), NW(nullptr), NE(nullptr), SE(nullptr), SW(nullptr) {}

void quadtree::copy(const quadtree& orig) {
    /* your code here */
    edge = orig.edge;
    copyHelperFunction(root, orig.root);
}

// pass by reference to change subtree
void quadtree::copyHelperFunction(Node* &subtree, const Node* otherSubtree) {

    if (otherSubtree == nullptr) {
        return;
    } else {
        subtree = new Node(otherSubtree->upLeft, otherSubtree->dim, otherSubtree->avg, otherSubtree->var);

        // Copy each child recursively
        copyHelperFunction(subtree->NW, otherSubtree->NW);
        copyHelperFunction(subtree->NE, otherSubtree->NE);
        copyHelperFunction(subtree->SE, otherSubtree->SE);
        copyHelperFunction(subtree->SW, otherSubtree->SW);
    }
}