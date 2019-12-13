#pragma once

#include <iostream>

#include <Eigen/Eigen>

#include "definitions.h"
#include "RayTracing.h"
#include "VisualizationModel.h"

/**
 * Implements the MIP (maximum intensity projection) algorithm.
 * In this algorithm, a fixed step size is chosen and
 * a ray is traversed trough a volume with this fixed step size.
 * @brief The MIP class
 */
class MIP
{
private:
public:
    /**
     * Executes the MIP algorithm.
     * The ray for traversing the volume is defined by the
     * camera pixel's position and the direction of view.
     * @brief calculateMIP
     * @param vol - reference to the current volume
     * @param pos - position of the camera pixel
     * @param direction - direction of view
     * @param stepSize - fixed step size (determines which points
     * along the ray are defined.
     * @return
     */
    float calculateMIP(const Volume& vol, Eigen::Vector3f pos, Eigen::Vector3f direction, float stepSize);
};
