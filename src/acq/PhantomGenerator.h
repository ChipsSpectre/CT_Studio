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
static Volume createPhantom(std::string phantomName) {
    const std::string cubeName = "cube";
    if(phantomName.compare(cubeName) == STRINGS_EQUAL) {
        Eigen::VectorXf data(1000);
        for(int i = 0; i<1000; i++) {
            int x = i % 10;
            int y = (i / 10) % 10;
            int z = (i / 100) % 10;
            if(4 <= x && x <= 6 && 4 <= y && y <= 6 && 
                4 <= z && z <= 6) {
                data(i) = 1;
            } else {
                data(i) = 0;
            }
        }

        Eigen::Vector3f lowerLeft = Eigen::Vector3f::Zero();
        Eigen::Vector3f upperRight = Eigen::Vector3f::Zero();
        for(int i = 0; i<3; i++) {
            upperRight(i) = 0.1f;
        }
        Eigen::Vector3f dimSpacing = Eigen::Vector3f::Zero();
        for(int i = 0; i<3; i++) {
            dimSpacing(i) = 0.01f;
        }

        Volume vol(lowerLeft, upperRight, dimSpacing, std::move(data));
        return vol;
    } else {
        throw std::invalid_argument("invalid phantom name!");
    }
}
};