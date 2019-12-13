#pragma once

#include <Eigen/Eigen>
#include <Eigen/Geometry>

#include <vector>

#include "AcquisitionPose.h"
#include "RayTracing.h"
#include "Volume.h"


/**
 * This is the abstract container class representing the system matrix B.
 * The kind of B is defined in subclasses.
 *
 * The system matrix B is never completely stored, but only the vector
 * of acquisition poses.
 * @brief The DataContainer class
 */
class DataContainer{
protected:
    /**
     * The volume to be reconstructed.
     */
    const VolumeBase _base;
    /**
     * The acquisition poses needed for reconstruction.
     */
    const std::vector<AcquisitionPose>& _poses;
    
public:
    /**
     * Creates a new DataContainer and determines not only the volume,
     * but also the poses.
     * @brief DataContainer
     * @param volumeBase
     * @param poses
     */
    DataContainer(const VolumeBase& volumeBase, const std::vector<AcquisitionPose>& poses) 
        : _base {volumeBase}, _poses {poses} {
            
    }

    /**
     * Multiplication of the on-the-fly computed system matrix
     * with a given vector
     * @brief mult
     * @param vector - the vector to be multiplied with
     * @return result of multiplication in new vector
     */
    virtual Eigen::VectorXf mult(Eigen::VectorXf vector) const = 0;

    /**
     * computes A*x, where A is the system matrix of the tomographic problem (= forward projection)
     */
    Eigen::VectorXf forwardProj( const Eigen::VectorXf& x) const{
        return RayTracing::forwardProj(_base, _poses, x);
    }
    
    /**
     * computes A^T*x, where A is the system matrix of the tomographic problem (=backward projection)
     */
    Eigen::VectorXf backwardProj(const Eigen::VectorXf& x) const {
        return RayTracing::backwardProj(_base, _poses, x);
    }
    
    /**
     * Computes the total number of voxels in the volume.
     */
    int getTotalVoxelCount() const {
        return _base.getTotalVoxelCount();
    }
    
    /**
     * Gets the current volume.
     */
    const VolumeBase& getVolume() const{
        return _base;
    }
};
