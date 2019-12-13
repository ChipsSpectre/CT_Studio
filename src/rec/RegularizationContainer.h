#pragma once

#include <iostream>

#include <Eigen/Eigen>

#include "DataContainer.h"

/**
 * This data container is used for representing the system matrix B
 * in reconstruction scenarios with Tikhonov regularization.
 * @brief The NormalReconstructionContainer class
 */
class RegularizationContainer : public DataContainer
{
private:
    float _lambda;
public:    
    /**
     * Creates a new RegularizationContainer
     * by calling the constructor of the superclass.
     * @brief RegularizationContainer
     * @param volume
     * @param poses
     */
    RegularizationContainer(const VolumeBase& volume, const std::vector<AcquisitionPose>& poses, float lambda)
        : DataContainer(volume, poses), _lambda {lambda}
    {
    }
    
    virtual Eigen::VectorXf mult(const Eigen::VectorXf vector) const override
    {
        //calculates A^T*A*x
        Eigen::VectorXf res = backwardProj(forwardProj(vector));
        
        //adds lambda*I*x = lambda * x
        res += _lambda * vector;
        
        return res;
    }
};
