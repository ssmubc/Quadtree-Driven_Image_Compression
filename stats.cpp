#include "stats.h"
#include <stdio.h>
#include <string.h>
#include <cmath>


stats::stats(PNG& im) {
    /* Your code here!! */

    int width = im.width();
    int height = im.height();

    // resizing to ensure that the cumulative sum vectors have enough space to 
    // for cumulative sums of all pixels in the image, regardless of the 
    // dimensions of the image.
    // which are one pixel outside the range of the image so do (width + 1) and (height + 1)


    // set all values to zero

    sumRed.resize(width, vector<long>(height, 0));
    sumGreen.resize(width, vector<long>(height, 0));
    sumBlue.resize(width, vector<long>(height, 0));
    sumsqRed.resize(width, vector<long>(height, 0));
    sumsqGreen.resize(width, vector<long>(height, 0));
    sumsqBlue.resize(width, vector<long>(height, 0));


    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            sumRed[i][j] = 0;
            sumsqRed[i][j] = 0;
            sumGreen[i][j] = 0;
            sumsqGreen[i][j] = 0;
            sumBlue[i][j] = 0;
            sumsqBlue[i][j] = 0;
        }
    }


    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            RGBAPixel * pixel = im.getPixel(i, j);

            if (i == 0 && j == 0) {
                sumRed[i][j] = pixel->r;
                sumsqRed[i][j] = pow(pixel->r, 2);
                
                sumGreen[i][j] = pixel->g;
                sumsqGreen[i][j] = pow(pixel->g, 2);

                sumBlue[i][j] = pixel->b;
                sumsqBlue[i][j] = pow(pixel->b, 2);

            } else if (i == 0 && j != 0) {
                sumRed[i][j] = sumRed[i][j - 1] + pixel->r;
                sumsqRed[i][j] = sumsqRed[i][j - 1] + pow(pixel->r, 2);

                sumGreen[i][j] = sumGreen[i][j - 1] + pixel->g;
                sumsqGreen[i][j] = sumsqGreen[i][j - 1] + pow(pixel->g, 2);

                sumBlue[i][j] = sumBlue[i][j - 1] + pixel->b;
                sumsqBlue[i][j] = sumsqBlue[i][j - 1] + pow(pixel->b, 2);

            } else if (j == 0 && i != 0) {
                sumRed[i][j] = sumRed[i - 1][j] + pixel->r;
                sumsqRed[i][j] = sumsqRed[i - 1][j] + pow(pixel->r, 2);

                sumGreen[i][j] = sumGreen[i - 1][j] + pixel->g;
                sumsqGreen[i][j] = sumsqGreen[i - 1][j] + pow(pixel->g, 2);

                sumBlue[i][j] = sumBlue[i - 1][j] + pixel->b;
                sumsqBlue[i][j] = sumsqBlue[i - 1][j] + pow(pixel->b, 2);

            } else {
                sumRed[i][j] = sumRed[i][j-1] + sumRed[i-1][j] - sumRed[i-1][j-1] + pixel->r; // for cumulative
                sumsqRed[i][j] = sumsqRed[i][j-1] + sumsqRed[i-1][j] - sumsqRed[i-1][j-1] + pow(pixel->r, 2);

                sumBlue[i][j] = sumBlue[i][j-1] + sumBlue[i-1][j] - sumBlue[i-1][j-1] + pixel->b; // for cumulative
                sumsqBlue[i][j] = sumsqBlue[i][j-1] + sumsqBlue[i-1][j] - sumsqBlue[i-1][j-1] + pow(pixel->b, 2);

                sumGreen[i][j] = sumGreen[i][j-1] + sumGreen[i-1][j] - sumGreen[i-1][j-1] + pixel->g;
                sumsqGreen[i][j] = sumsqGreen[i][j-1] + sumsqGreen[i-1][j] - sumsqGreen[i-1][j-1] + pow(pixel->g, 2);
            }
        }
    }
}


long stats::getSum(char channel, pair<int, int> ul, int dim) {
    /* Your code here!! */

    // find the bottom right corner

    int sideLength = pow(2, dim);   // the side length 
    int xt = ul.first;    // to get the top left x coord
    int yt = ul.second;   // to get the top left y coord

    int xb = xt + sideLength - 1;  // to get the bottom right x coord
    int yb = yt + sideLength - 1; // to get the bottom right y coord

    // !!!
    // previously: xt >= sideLength || yt >= sideLength
    if (xb >= (int) sumRed.size() || yb >= (int) sumRed[0].size()) {
        return 0;   // handle case where the square is out of bounds of PNG
    }

    if (channel == 'r') {
        if (xt == 0 && yt == 0) {
            return sumRed[xb][yb];
        } else if (xt == 0 && yt != 0) {
            return sumRed[xb][yb] - sumRed[xb][yt-1];            
        } else if (yt == 0 && xt != 0) {
            return sumRed[xb][yb] - sumRed[xt-1][yb];
        } else {
            return sumRed[xb][yb] - sumRed[xb][yt - 1] - sumRed[xt - 1][yb] + sumRed[xt - 1][yt - 1]; 
        }

    } else if (channel == 'g') {
        if (xt == 0 && yt == 0) {
            return sumGreen[xb][yb];
        } else if (xt == 0 && yt != 0) {
            return sumGreen[xb][yb] - sumGreen[xb][yt-1]; 
        } else if (yt == 0 && xt != 0) {
            return sumGreen[xb][yb] - sumGreen[xt-1][yb];
        } else {
            return sumGreen[xb][yb] - sumGreen[xb][yt - 1] - sumGreen[xt - 1][yb] + sumGreen[xt - 1][yt - 1];
        }
        
    } else if (channel == 'b') {
        if (xt == 0 && yt == 0) {
            return sumBlue[xb][yb];
        } else if (xt == 0 && yt != 0) {
            return sumBlue[xb][yb] - sumBlue[xb][yt-1];
        } else if (yt == 0 && xt != 0) {
            return sumBlue[xb][yb] - sumBlue[xt-1][yb];
        } else {
            return sumBlue[xb][yb] - sumBlue[xb][yt - 1] - sumBlue[xt - 1][yb] + sumBlue[xt - 1][yt - 1];
        }
        
    } else {
        return 0;   // the case where channel is not equal to r, g, or b
    }    
}



long stats::getSumSq(char channel, pair<int, int> ul, int dim) {
    /* Your code here!! */

    int sideLength = pow(2, dim);   // the side length 
    int xt = ul.first;    // to get the top left x coord
    int yt = ul.second;   // to get the top left y coord

    int xb = xt + sideLength - 1;  // to get the bottom right x coord
    int yb = yt + sideLength - 1; // to get the bottom right y coord

    if (channel == 'r') {
        if (xt == 0 && yt == 0) {
            return sumsqRed[xb][yb];
        } else if (xt == 0 && yt != 0) {
            return sumsqRed[xb][yb] - sumsqRed[xb][yt-1];
        } else if (yt == 0 && xt != 0) {
            return sumsqRed[xb][yb] - sumsqRed[xt-1][yb];
        } else {
            return sumsqRed[xb][yb] - sumsqRed[xb][yt-1] - sumsqRed[xt-1][yb] + sumsqRed[xt-1][yt-1];
        }
    } else if (channel == 'g') {
        if (xt == 0 && yt == 0) {
            return sumsqGreen[xb][yb];
        } else if (xt == 0 && yt != 0) {
            return sumsqGreen[xb][yb] - sumsqGreen[xb][yt-1];
        } else if (yt == 0 && xt != 0) {
            return sumsqGreen[xb][yb] - sumsqGreen[xt-1][yb];
        } else {
            return sumsqGreen[xb][yb] - sumsqGreen[xb][yt - 1] - sumsqGreen[xt - 1][yb] + sumsqGreen[xt - 1][yt - 1];
        }

    } else if (channel == 'b') {
        if (xt == 0 && yt == 0) {
            return sumsqBlue[xb][yb];
        } else if (xt == 0 && yt != 0) {
            return sumsqBlue[xb][yb] - sumsqBlue[xb][yt-1];
        } else if (yt == 0 && xt != 0) {
            return sumsqBlue[xb][yb] - sumsqBlue[xt-1][yb];
        } else {
            return sumsqBlue[xb][yb] - sumsqBlue[xb][yt - 1] - sumsqBlue[xt - 1][yb] + sumsqBlue[xt - 1][yt - 1];
        }

    } else {
        return 0;   // the case where channel is not equal to r, g, or b
    }    
}

long stats::rectArea(int dim) {
    /* Your code here!! */
    int sideLength = pow(2, dim);
    
    long numPixels = sideLength * sideLength;
    return numPixels;
}


// given a rectangle, compute its sum of squared deviations from mean, over all color channels.
// see written specification for a description of this function.
double stats::getVar(pair<int, int> ul, int dim) {

    long sumOfRed = getSum('r', ul, dim);
    long sumOfGreen = getSum('g', ul, dim);
    long sumOfBlue = getSum('b', ul, dim);

    long squareOfRed = getSumSq('r', ul, dim);
    long squareOfGreen = getSumSq('g', ul, dim);
    long squareOfBlue = getSumSq('b', ul, dim);

    long area = rectArea(dim);
    
    double varianceR = squareOfRed - (sumOfRed * sumOfRed)/area;
    double varianceG = squareOfGreen - (sumOfGreen * sumOfGreen)/area;
    double varianceB = squareOfBlue - (sumOfBlue * sumOfBlue)/area;

    return varianceR + varianceG + varianceB;
}



RGBAPixel stats::getAvg(pair<int, int> ul, int dim) {
    /* Your code here!! */

    long redValue = getSum('r', ul, dim);
    long greenValue = getSum('g', ul, dim);
    long blueValue = getSum('b', ul, dim);

    int avgRed = redValue / rectArea(dim);
    int avgGreen = greenValue / rectArea(dim);
    int avgBlue = blueValue / rectArea(dim);

    
    return RGBAPixel(avgRed, avgGreen, avgBlue);
}