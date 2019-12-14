#pragma once

#include <iostream>
#include <string>

#include "Volume.h"

/**
 * The phantom generater creates phantom data on-the-fly.
 */
class PhantomGenerator {
private:
static
    const int STRINGS_EQUAL = 0;
public:
static Volume createPhantom(int sideLength, std::string phantomName) {
    int n = sideLength;
    Eigen::VectorXf data(n*n*n);
    if(phantomName == "cube") {
        for(int i = 0; i<n*n*n; i++) {
            int x = i % n;
            int y = (i / n) % n;
            int z = (i / (n*n)) % n;
            int low = n/2 - 2;
            int high = n/2 + 2;
            if(low <= x && x <= high && low <= y && y <= high && 
                low <= z && z <= high) {
                data(i) = 1;
            } else {
                data(i) = 0;
            }
        }

    } else {
        if(phantomName == "sticks") {
            for(int i = 0; i<n*n*n; i++) {
                int x = i % n;
                int y = (i / n) % n;
                int low = n/4;
                int high = 3*n/4;
                if(low <= x && x <= high && low <= y && y <= high) {
                    data(i) = 1;
                } else {
                    data(i) = 0;
                }
            }
        } else {
            if(phantomName == "box") {
                for(int i = 0; i<n*n*n; i++) {
                    int x = i % n;
                    int y = (i / n) % n;
                    int z = (i / (n*n)) % n;
                    data(i) = 0;
                    // hard tissue stick
                    int low = n/4;
                    int high = 3*n/4;
                    if(low <= x && x <= high && low <= y && y <= high && z >= low && 
                        z <= high) {
                        data(i) = 1;
                    }
                    // hole in the middle
                    low = 3*n/8;
                    high = 5*n/8;
                    if(low <= x && x <= high && low <= y && y <= high) {
                        data(i) = 0;
                    }
                }
            } else {
                throw std::invalid_argument("invalid phantom name!");
            }
        }
    }
    Eigen::Vector3f lowerLeft = Eigen::Vector3f::Zero();
    Eigen::Vector3f upperRight = Eigen::Vector3f::Zero();
    for(int i = 0; i<3; i++) {
        upperRight(i) = 0.2f;
    }
    Eigen::Vector3f dimSpacing = Eigen::Vector3f::Zero();
    float spacing = 0.2f / sideLength;
    for(int i = 0; i<3; i++) {
        dimSpacing(i) = spacing;
    }

    Volume vol(lowerLeft, upperRight, dimSpacing, std::move(data));
    return vol;
}
};