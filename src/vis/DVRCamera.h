#pragma once

#include <Eigen/Eigen>

#include "Volume.h"

/**
 * Manages the camera for direct volume rendering,
 * i.e. is able to calibrate the camera (after a
 * volume change occurred in the gui).
 * @brief The DVRCamera class
 */
class DVRCamera
{
private:
public:
    Eigen::Vector3f calculateCameraPosition(const Volume& volume, float angle);
};
