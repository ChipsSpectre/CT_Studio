#include <cmath>
#include <iostream>

#include "AcquisitionModel.h"

AcquisitionModel::AcquisitionModel(std::string phantomName)
    :  _filled {true}, _volume{PhantomGenerator::createPhantom(phantomName)},
    _posePrototype {_volume.getBoundingBox()},
    _poses { _posePrototype},
    _measurements {RayTracing::forwardProj(_volume, currPoseChecked())}
{
}


bool AcquisitionModel::loadFile(QString path) {
    // TODO
    std::cout << "Writing to disk currently not supported. " 
        << path.toUtf8().constData() << std::endl;

    return true;
}

void AcquisitionModel::updateRotation(RotationAxis axis, float angle){
    //see description of the rotation axis for why the if is as it is
    if(axis == RotationAxis::Z){
        currPoseChecked().addRotationZ(angle);
    } else if (axis == RotationAxis::Y) {
        currPoseChecked().addRotationY(angle);
    }
    updateLastProjection();
}

std::array<Eigen::Vector3f, 4> AcquisitionModel::getDetector() const{
    return std::array<Eigen::Vector3f, 4> {
        currPoseChecked().getDetectorUpperLeft(), currPoseChecked().getDetectorLowerLeft(),
        currPoseChecked().getDetectorLowerRight(), currPoseChecked().getDetectorUpperRight()
    };
}

Eigen::Vector2i AcquisitionModel::getDetectorSize() const{
    return Eigen::Vector2i {currPoseChecked().getPixelHorizontal(), currPoseChecked().getPixelVertical()};
}

Eigen::Vector3f AcquisitionModel::getSourcePosition() const{
    return currPoseChecked().getSourcePosition();
}

Eigen::Vector3f AcquisitionModel::getPixelCenter(int i, int j) const{
    return currPoseChecked().getPixelCenter(i, j);
}

void AcquisitionModel::writeImage(std::string path) const
{
    // TODO
    std::cout << "Writing to disk currently not supported." << path << std::endl;
}

Eigen::AlignedBox3f AcquisitionModel::getBoundingBox() const{
        return _volume.getBoundingBox();
    }

void AcquisitionModel::clearPoses() {
        _poses.clear();
        //since the stack must alway contain one element, add the default pose again
        addDefaultPose();
        updateProjection();
        emit poseChanged();
    }

void AcquisitionModel::deletePose() {
        //since there always has to be an element pop always works
        _poses.pop_back();
        if(_poses.empty()){
            addDefaultPose();
        }
        updateProjection();
        emit poseChanged();
    }

void AcquisitionModel::savePose() {
        addDefaultPose();
        updateProjection();
        emit poseChanged();
    }

AcquisitionPose& AcquisitionModel::currPose() {
        return _poses.back();
    }

const AcquisitionPose& AcquisitionModel::currPose() const {
        return _poses.back();
    }

AcquisitionPose& AcquisitionModel::currPoseChecked() {
        if(_poses.empty()){
            throw std::out_of_range("Acess on empty pose vector. Fix the AcquistionModel, so that doesn't happen!");
        }
        return _poses.back();
    }

const AcquisitionPose& AcquisitionModel::currPoseChecked() const {
        if(_poses.empty()){
            throw std::out_of_range("Acess on empty pose vector. Fix the AcquistionModel, so that doesn't happen!");
        }
        return _poses.back();
    }


void AcquisitionModel::addSphericalPoses(int circles, int equatorialCount, float range)
{
    _poses.pop_back();
    
    float distance = M_PI / circles;
    
    _poses.reserve(circles * equatorialCount);
    
    for(float yRot = - M_PI / 2.f + distance / 2.f; yRot < M_PI / 2.f; yRot += distance)
    {
        int count;
        float cosine = cosf(yRot);
        if(1.f > cosine * equatorialCount)
        {
            count = 1;
        }
        else
        {
            count = static_cast<int>(cosine * equatorialCount);
        }
        addCircularPoses(count, yRot, range);
    }
    updateProjection();
    emit poseChanged();
}

void AcquisitionModel::addCircularPoses(int count, float yAngle, float range)
{
    float distance = range;
    if(count > 1)
    {
        distance = range / (count);
    }
    for(int i = 0; i < count; i++)
    {
        AcquisitionPose pose {_posePrototype};
        pose.setRotation(distance / 2.f + i * distance, yAngle);
        _poses.push_back(pose);
    }
}

std::pair<int, const Eigen::VectorXf> AcquisitionModel::getLastProj()
{
    const AcquisitionPose currAcq = currPoseChecked();
    const int totalSize = currAcq.getPixelCount();
    const int offset = _measurements.size() - totalSize;
    
    
    Eigen::VectorXf proj {totalSize};
    for(int p = 0; p < totalSize; ++p){
        proj[p] = _measurements[offset + p];
    }
    return std::make_pair(currAcq.getPixelVertical(), proj);
}

void AcquisitionModel::addDefaultPose(){
    _poses.push_back(_posePrototype);
}

void AcquisitionModel::updateProjection(){
    _measurements = 
        RayTracing::forwardProj(_volume, _poses, _volume.content().rawVec());
    // TODO is this necessary??
    //emit poseChanged();
}

void AcquisitionModel::updateLastProjection(){
    const int rayCount = currPoseChecked().getPixelCount();
    const Eigen::VectorXf proj = RayTracing::forwardProj(volume(), currPoseChecked());
    const int offset = _measurements.size() - rayCount;
    assert(rayCount == proj.size());
    
    for(int x = 0 ; x < rayCount; ++x) {
        _measurements[offset + x] = proj[x];
    }
    emit poseChanged();
}
