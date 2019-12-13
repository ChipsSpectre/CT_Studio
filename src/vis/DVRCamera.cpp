#include "DVRCamera.h"

Eigen::Vector3f DVRCamera::calculateCameraPosition(const Volume& volume, float angle)
{
    if(volume.getTotalVoxelCount() == 0)
    {
        return Eigen::Vector3f(0,0,0); // volume was empty
    }

    Eigen::Vector3f direction(0,0,0);
    direction(0) += std::cos(angle);
    direction(1) += std::sin(angle);
    float size = volume.getNumVoxels()[0] *
                    volume.getSpacing()[0];
    for(int i = 0; i<3; ++i)
    {
        direction(i) *= 2*size;
    }

    // center is always centered at the origin, so just consider the direction
    return direction;
}
