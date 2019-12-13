#ifndef RAYTRACING_H
#define RAYTRACING_H
#include <cmath>
#include <iostream>
#include <limits>
#include <vector>

#include <Eigen/Geometry>
#include <Eigen/Dense>
#include <Eigen/Sparse>

#include "AcquisitionPose.h"
#include "definitions.h"
#include "Vec3D.h"
#include "Volume.h"

/**
 * This class contains all algorithms concerning ray tracing.
 * @brief The RayTracing class
 */
class RayTracing
{
    /**
     * return -1 if the number is negative otherwise +1
     */
    static
    int simpSign(float x){
        return std::signbit(x) ? -1 : 1;
    }

    /*
     * Used as a return value
     * Represents the the intersections points of a ray and a box if they never actually
     * Intersect.
     * Should never be used but caught before it can be used in a calculation
     * If available this is a signaling NaN which might make the usage of this in a 
     * calculation easier to spot
     */
    static constexpr float nanConst = std::numeric_limits<float>::has_signaling_NaN ?
            std::numeric_limits<float>::signaling_NaN() : std::numeric_limits<float>::quiet_NaN();
   

public:

    /**
     * return t so that ray.pointAt(t) is the actual intersection point
     */
    static float boxIntersectHelper(const Eigen::AlignedBox3f& box, const Line3f& ray) noexcept;
    
    /**
     *
     * @brief boxIntersect Returns the intersection point of the ray with the box.
     * @return an 3x1 Eigen Vector which contains the point of intersection.
     */
    static Eigen::Vector3f boxIntersect(const Eigen::AlignedBox3f& box, const Line3f& ray) noexcept;
    
    
    static float forwardProject(const VolumeBase& volume, const Line3f& ray, Eigen::VectorXf values);
    
    /**
     * Forward Projection for only 1 pose.
     * @brief forwardProject
     * @return the forward projection in a ROW MAJOR ORDER
     */
    static Eigen::VectorXf forwardProj(const Volume& vol, const AcquisitionPose& pose);
    
    /**
     * Computes A*values, where A is the system matrix of the tomographic problem
     */
    static Eigen::VectorXf forwardProj(const VolumeBase& vol, const std::vector<AcquisitionPose>& poses, const Eigen::VectorXf& values);

    /**
     * calculates the backwardProjection of the given ray and the given box.
     */
    static float backwardProj(const VolumeBase& box, const Line3f& ray, Eigen::VectorXf& backProj, float projValue);

    /**
     * calculates the backwardProjection of the given ray and the given box.
     */
    static Eigen::VectorXf backwardProj(const VolumeBase& box, const std::vector<AcquisitionPose> poses, const Eigen::VectorXf forwardP);
    
};

#endif // RAYTRACING_H
