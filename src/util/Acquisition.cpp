#include "Acquisition.h"

Acquisition::Acquisition(const VolumeBase& volBase, const std::vector<AcquisitionPose>& poses, 
    const Eigen::VectorXf& measurements
) : volBase {volBase}, poses {poses}, measurements {measurements} {
    
}