#pragma once

#include <vector>

#include <Eigen/Eigen>

#include "DataContainer.h"

/**
 * This data container is used for representing the system matrix B
 * in reconstruction scenarios without regularization.
 * @brief The NormalReconstructionContainer class
 */
class NormalReconstructionContainer : public DataContainer
{
public:
    /**
     * Creates a new NormalReconstructionContainer
     * by calling the constructor of the superclass.
     * @brief NormalReconstructionContainer
     * @param volume
     * @param poses
     */
    NormalReconstructionContainer(const VolumeBase& volume, const std::vector<AcquisitionPose>& poses)
        : DataContainer(volume, poses) { 
            
    }
    
    virtual Eigen::VectorXf mult(Eigen::VectorXf vector) const override
    {
        return backwardProj(forwardProj(vector));
    }
};
