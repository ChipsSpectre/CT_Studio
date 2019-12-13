#include "RayTracing.h"
#include <iostream>

float RayTracing::boxIntersectHelper(const Eigen::AlignedBox3f& box, const Line3f& ray) noexcept {
    //based on "An Efficient and Robust Ray–Box Intersection Algorithm"
    //link:  http://people.csail.mit.edu/amy/papers/box-jgt.pdf
    float tmin, tmax, tymin, tymax, tzmin, tzmax;
    const auto boxMin = box.min();
    const auto boxMax = box.max();
        
    {
        const float divx = 1/ray.direction().x(); 
        //if(ray.direction().x() >= 0 )
        if(!std::signbit(ray.direction().x())){
            tmin = (boxMin.x() - ray.origin().x()) * divx;
            tmax = (boxMax.x() - ray.origin().x()) * divx;
        } else {
            tmin = (boxMax.x() - ray.origin().x()) * divx;
            tmax = (boxMin.x() - ray.origin().x()) * divx;
        }
    }
        
    {
        const float divy = 1/ray.direction().y(); 
        //if(ray.direction().y() >= 0){
        if(!std::signbit(ray.direction().y())){
            tymin = (boxMin.y() - ray.origin().y()) * divy;
            tymax = (boxMax.y() - ray.origin().y()) * divy;
        } else {
            tymin = (boxMax.y() - ray.origin().y()) * divy;
            tymax = (boxMin.y() - ray.origin().y()) * divy;
        }
    }
        
    if ( (tmin > tymax)|| (tymin > tmax)){
        return nanConst;
    }
        
    if (tymin> tmin){
        tmin = tymin;
    }
        
    if (tymax < tmax){
        tmax = tymax;
    }
        
    {
        const float divz = 1/ray.direction().z(); 
        //if(ray.direction().z() >= 0){
        if(!std::signbit(ray.direction().z())){
            tzmin = (boxMin.z() - ray.origin().z()) * divz;
            tzmax = (boxMax.z() - ray.origin().z()) * divz;
        } else {
            tzmin = (boxMax.z() - ray.origin().z()) * divz;
            tzmax = (boxMin.z() - ray.origin().z()) * divz;
        }

    }

    if ( (tmin > tzmax)|| (tzmin > tmax)){
        return nanConst;
    }
    
    if (tzmin > tmin) {
        tmin = tzmin;
    }

    return tmin;
}


Eigen::Vector3f RayTracing::boxIntersect(const Eigen::AlignedBox3f& box, const Line3f& ray) noexcept
{
    return Eigen::Vector3f(ray.origin() + ray.direction() * boxIntersectHelper(box, ray));
}

float RayTracing::forwardProject(const VolumeBase& volume, const Line3f& ray, Eigen::VectorXf values) {
    const auto boundingBox = volume.getBoundingBox();
    const float tIntersect = boxIntersectHelper(boundingBox, ray);

    //The ray doesn't intersect the volume
    if(std::isnan(tIntersect)){
        return 0; //std::vector<Eigen::Vector3f> {}
    }

    const Eigen::Vector3f direction {ray.direction()};
    const Eigen::Vector3i step {simpSign(direction.x()), simpSign(direction.y()), simpSign(direction.z())};

    const Eigen::Vector3f voxel {volume.getSpacing()};
    const Eigen::Vector3i maxVoxel {volume.getNumVoxels() - Eigen::Vector3i {1, 1, 1}};

    const Eigen::Vector3f tDelta  = Eigen::Vector3f{voxel.cwiseQuotient(direction)}.cwiseAbs();

    const Eigen::Vector3f inPoint {ray.pointAt(tIntersect)};
    
    Eigen::Vector3i pos { (inPoint - boundingBox.min()).cwiseQuotient(voxel).cast<int>() };
    
    
    // rounding errors can cause the position to be outside the volume
    for(int i = 0; i < 3; i++)
    {
        if(pos(i) > maxVoxel(i))
        {
            pos(i) = maxVoxel(i);
        }
    }

    
    //calculate fraction

    //calculate the boundaries of a voxel in the direction of the ray
    const float boundX = step.x() <= 0 ? 0 : voxel.x();
    const float boundY = step.y() <= 0 ? 0 : voxel.y();
    const float boundZ = step.z() <= 0 ? 0 : voxel.z();
    const Eigen::Vector3f bounds {boundX, boundY, boundZ};
    
    //intersection point relative to initially intersected voxel
    const Eigen::Vector3f relativeIntersect {inPoint - boundingBox.min() - pos.cast<float>().cwiseProduct(voxel)};
    
    //the above in relative distance to the next hit boundary
    //const Eigen::Vector3f relativeIntersectFrac {(bounds - relativeIntersect).cwiseQuotient(voxel).cwiseAbs()};
    const Eigen::Vector3f relativeIntersectFrac {(bounds - relativeIntersect).cwiseAbs()};
    
    //Eigen::Vector3f tMax {tDelta.cwiseProduct(Eigen::Vector3f{1, 1, 1} - relativeIntersectFrac)};
    
    Eigen::Vector3f tMax = relativeIntersectFrac.cwiseQuotient(direction).cwiseAbs();
    // componentwise distance to next voxel in respective direction weighted with the direction of the ray
    //Eigen::Vector3f tMax {tDelta.cwiseProduct(relativeIntersectFrac)};
    
    
    float acc = 0;
    if(pos.x() > maxVoxel.x() || pos.y() > maxVoxel.y() || pos.z() > maxVoxel.z())
    {
        return acc;
    }

    for(;;) {
        
        acc += values[volume.coordinateToIndex(pos)];
        
        if(tMax.x() < tMax.y()) {
            if(tMax.x() < tMax.z()) {
                pos.x() += step.x();
                if(pos.x() < 0 || pos.x() > maxVoxel.x()) {
                    return acc;
                }
                tMax.x() += tDelta.x();
            } else {
                pos.z() += step.z();
                if(pos.z() < 0 || pos.z() > maxVoxel.z()) {
                    return acc;
                }
                
                tMax.z() += tDelta.z();
            }
        } else  {
            if(tMax.y() < tMax.z()) {
                pos.y() += step.y();
                if(pos.y() < 0 || pos.y() > maxVoxel.y()){
                    return acc;
                }
                
                tMax.y() += tDelta.y();
            } else  {
                pos.z() +=  step.z();
                if(pos.z() < 0 || pos.z() > maxVoxel.z()) {
                    return acc;
                }
                tMax.z() += tDelta.z();
            }
        }
    }       
}

Eigen::VectorXf RayTracing::backwardProj(const VolumeBase& box, const std::vector< AcquisitionPose > poses, const Eigen::VectorXf forwardP) {
    Eigen::VectorXf proj = Eigen::VectorXf::Zero(box.getTotalVoxelCount());
    if(poses.empty()){
        return proj;
    }
        
    if(poses.size() > static_cast<size_t>(std::numeric_limits<int>::max())){
        std::string errorMessage {"The given array was to big. It had size: "};
        errorMessage += std::to_string(poses.size());
        errorMessage += " but maximum allowed size is: ";
        errorMessage += std::to_string(std::numeric_limits<int>::max());
        throw std::logic_error {errorMessage};
    }
        
    auto horizonalPixels = poses[0].getPixelHorizontal();
    auto verticalPixels = poses[0].getPixelVertical();
        
    for(int p = 0; p < int(poses.size()); ++p){
        auto pIndex = p*horizonalPixels*verticalPixels;
        for(int y = 0; y < verticalPixels; ++y){
            auto yIndex = y * horizonalPixels;
            for(int x = 0; x < horizonalPixels; ++x){
                RayTracing::backwardProj(box, poses[p].getRay(x,y), proj, forwardP[x + yIndex + pIndex]);
            }
        }
    }
        
    return proj;
    }

float RayTracing::backwardProj(const VolumeBase& box, const Line3f& ray, Eigen::VectorXf& backProj, float projValue) {
    //based on "An Efficient and Robust Ray–Box Intersection Algorithm"
    //link:  http://people.csail.mit.edu/amy/papers/box-jgt.pdf
    const auto boundingBox = box.getBoundingBox();
    const float tIntersect = boxIntersectHelper(boundingBox, ray);

    //The ray doesn't intersect the volume
    if(std::isnan(tIntersect)){
        return 0; //std::vector<Eigen::Vector3f> {}
    }
        
    const Eigen::Vector3f direction {ray.direction()};
    const Eigen::Vector3i step {simpSign(direction.x()), simpSign(direction.y()), simpSign(direction.z())};
        
    const Eigen::Vector3f voxel {box.getSpacing()};
    const Eigen::Vector3i maxVoxel {box.getNumVoxels() - Eigen::Vector3i {1, 1, 1}};
    const Eigen::Vector3f tDelta {voxel.cwiseQuotient(direction).cwiseAbs()};
    const Eigen::Vector3f inPoint {ray.pointAt(tIntersect)};
    Eigen::Vector3i pos { (inPoint - boundingBox.min()).cwiseQuotient(voxel).cast<int>() };
        
    // rounding errors can cause the position to be outside the volume
    for(int i = 0; i < 3; i++) {
        if(pos(i) > maxVoxel(i)) {
                pos(i) = maxVoxel(i);
        }
    }
        
    //calculate fraction
    //calculate the boundaries of a voxel in the direction of the ray
    const float boundX = step.x() <= 0 ? 0 : voxel.x();
    const float boundY = step.y() <= 0 ? 0 : voxel.y();
    const float boundZ = step.z() <= 0 ? 0 : voxel.z();
    const Eigen::Vector3f bounds {boundX, boundY, boundZ};
    //intersection point relative to initially intersected voxel
    const Eigen::Vector3f relativeIntersect {inPoint - boundingBox.min() - pos.cast<float>().cwiseProduct(voxel)};
    //the above in relative distance to the next hit boundary
    //const Eigen::Vector3f relativeIntersectFrac {(bounds - relativeIntersect).cwiseQuotient(voxel).cwiseAbs()};
    const Eigen::Vector3f relativeIntersectFrac {(bounds - relativeIntersect).cwiseAbs()};
        
    Eigen::Vector3f tMax = relativeIntersectFrac.cwiseQuotient(direction).cwiseAbs();
    //Eigen::Vector3f tMax = relativeIntersectFrac.cwiseProduct(tDelta).cwiseAbs();
    //Eigen::Vector3f tMax {tDelta.cwiseProduct(Eigen::Vector3f {1, 1, 1} - relativeIntersectFrac)};
        
    float acc = 0;
    if(pos.x() > maxVoxel.x() || pos.y() > maxVoxel.y() || pos.z() > maxVoxel.z()) {
        return acc;
    }


    for(;;) {
        backProj(box.coordinateToIndex(pos)) += projValue;

        if(tMax.x() < tMax.y()) {
            if(tMax.x() < tMax.z()) {
                pos.x() += step.x();
                if(pos.x() < 0 || pos.x() > maxVoxel.x()) {
                    return acc;
                }
                tMax.x() += tDelta.x();
            } else  {
                pos.z() += step.z();
                if(pos.z() < 0 || pos.z() > maxVoxel.z()) {
                    return acc;
                }
                tMax.z() += tDelta.z();
            }
        } else  {
            if(tMax.y() < tMax.z()) {
                pos.y() += step.y();
                if(pos.y() < 0 || pos.y() > maxVoxel.y()){
                    return acc;
                }
                tMax.y() += tDelta.y();
            } else  {
                pos.z() +=  step.z();
                if(pos.z() < 0 || pos.z() > maxVoxel.z()) {
                    return acc;
                }
                tMax.z() += tDelta.z();
            }
        }
    }       
}

Eigen::VectorXf RayTracing::forwardProj(const Volume& vol, const AcquisitionPose& pose) {
    const int verticalPixels = pose.getPixelVertical();
    const int horizontalPixels = pose.getPixelHorizontal();
    Eigen::VectorXf proj {verticalPixels * horizontalPixels};
    //row Major representation
#pragma omp parallel for
    for(int y = 0; y < pose.getPixelVertical(); ++y){
        int yInd = y * horizontalPixels;
        for(int x = 0; x < pose.getPixelHorizontal(); ++x){
            proj[yInd + x] = RayTracing::forwardProject(vol, pose.getRay(x, y), vol.content().rawVec());
        }
    }
        
    return proj;
}

Eigen::VectorXf RayTracing::forwardProj(const VolumeBase& vol, const std::vector< AcquisitionPose >& poses, const Eigen::VectorXf& values) {
    
    Eigen::VectorXf proj {(int)poses.size()*poses[0].getPixelHorizontal()*poses[0].getPixelVertical()};
    int poseCount = poses.size();
    int verticalPixels = poses[0].getPixelVertical();
    int horizontalPixels = poses[0].getPixelHorizontal();
    
    #pragma omp parallel for
    for(int p = 0; p < poseCount;++p){
        for(int y = 0; y < verticalPixels; ++y){
            for(int x = 0; x < horizontalPixels; ++x){
                proj[x + y*horizontalPixels + p*horizontalPixels*verticalPixels] = RayTracing::forwardProject(vol, poses[p].getRay(x, y), values);
            }
        }
    }
    
    return proj;
}
