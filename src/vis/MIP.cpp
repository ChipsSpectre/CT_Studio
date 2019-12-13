#include "MIP.h"

float MIP::calculateMIP(const Volume& vol, Eigen::Vector3f pos, Eigen::Vector3f direction, float stepSize)
{
    float sizeX = vol.getNumVoxels()[0] *
                    vol.getSpacing()[0];
    int steps = sizeX/stepSize;

    for(int i = 0; i<3; ++i)
    {
        if(std::abs(direction(i)) < 0.0001)
        {
            direction(i) = 0;
        }
    }
    Line3f ray(pos, direction);
    // intersection between current pixel and volume
    Eigen::Vector3f intersect = RayTracing::boxIntersect(vol.getBoundingBox(), ray);

    float max = 0.0;
    // NaN check (NaN are odd in comparison and will not execute inner block)
    if(intersect(0) == intersect(0))
    {
        for(int i = 0; i<steps; ++i)
        {
            // calculate intensity using trilinear interpolation
            float intensity = vol.getVoxelLinearPhysical(intersect);
            if(intensity > max)
            {
                max = intensity;
            }
            intersect += (stepSize) * direction;
        }
    }
    return max;
}
