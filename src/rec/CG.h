#pragma once

#include <Eigen/Eigen>

#include "DataContainer.h"
#include "Volume.h"

/**
 * This class implements the conjugate gradient algorithm.
 * @brief The CG class
 */
class CG
{

public:
    /**
     * Computes the conjugate gradient algorithm.
     * The initial guess x_0 is set to the zero vector.
     * @brief conjugateGradient
     * @param steps
     * @param b - solution vector (forward Projection)
     * @param A
     */
    static
    Eigen::VectorXf conjugateGradient(int steps, DataContainer& A, Eigen::VectorXf b);
};
