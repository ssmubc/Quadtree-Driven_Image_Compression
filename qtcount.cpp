#include "qtcount.h"
#include <stdio.h>
#include <iostream>
#include <vector>

using namespace std;

/*
*  A node is pruned if all of the leaves in its subtree have colour-distance within 
*  tolerance of its average. Distances between colours are computed as the sum, over each colour channel, 
   of the pixel value differences, squared. For reference, to create the pruned totem pole above, 
   we used a tolerance of 4000.
*/
bool qtcount::prunable(Node* node, const int tol) const {
    /* Your code here */

    // if (node == nullptr || node->NW == nullptr) {
    //     return false;
    // }
    
    return prunableHelper(node, node->avg, tol);
}

bool qtcount::prunableHelper(Node * node, RGBAPixel avg, int tol) const {
    bool isPrunable = false;

    if (!node->NW || !node->NE || !node->SW || !node->SE) {
        int rDist = avg.r - node->avg.r;
        int gDist = avg.g - node->avg.g;
        int bDist = avg.b - node->avg.b;
        int colorDist = rDist*rDist + gDist*gDist + bDist*bDist;
        isPrunable = colorDist <= tol;
        
    } else {
        isPrunable = prunableHelper(node->NW, avg, tol) && prunableHelper(node->NE, avg, tol) && prunableHelper(node->SE, avg, tol) && prunableHelper(node->SW, avg, tol);
    }

    return isPrunable;
}