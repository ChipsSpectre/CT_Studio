#include "AcquisitionPose.h"

#include <iostream>

AcquisitionPose::AcquisitionPose(float s2dd, float detW, float detH, int pixelH, int pixelV) :
    Pose(Eigen::Vector3f(-s2dd / 2.f, 0.f, 0.f)),
    _s2dd(s2dd),
    _det(s2dd / 2.f, 0.f, 0.f),
    _detUpl(s2dd / 2.f, detW/2.f, detH/2.f),
    _detWidth(detW), 
    _detHeight(detH), 
    _pxlHorizontal(pixelH), 
    _pxlVertical(pixelV)
{
}

Eigen::Vector3f AcquisitionPose::getSourcePosition() const
{
    return getPoint();
}

Eigen::Vector3f AcquisitionPose::getDetectorCenter() const
{
    return _det;
}
float AcquisitionPose::getDetectorWidth() const
{ return _detWidth;}


int AcquisitionPose::getPixelHorizontal() const
{ return _pxlHorizontal; }
int AcquisitionPose::getPixelVertical() const
{ return _pxlVertical; }


void AcquisitionPose::updatePose()
{
    Pose::updatePose();
    _det        = getRotation() * Eigen::Vector3f(_s2dd / 2.f, 0, 0) + Pose::getCenter();
    
    _detUpl    = getRotation() * Eigen::Vector3f(_s2dd / 2.f, _detWidth / 2.f, _detHeight / 2.f) + Pose::getCenter();
}


Eigen::Vector3f AcquisitionPose::getDetectorUpperLeft() const
{
    return _detUpl;
}
Eigen::Vector3f AcquisitionPose::getDetectorLowerLeft() const
{
    return getRotation() * Eigen::Vector3f(_s2dd / 2.f, _detWidth / 2.f, -_detHeight / 2.f) + Pose::getCenter();
}
Eigen::Vector3f AcquisitionPose::getDetectorLowerRight() const
{
    return getRotation() * Eigen::Vector3f(_s2dd / 2.f, -_detWidth / 2.f, -_detHeight / 2.f) + Pose::getCenter();
}
Eigen::Vector3f AcquisitionPose::getDetectorUpperRight() const
{
    return getRotation() * Eigen::Vector3f(_s2dd / 2.f, -_detWidth / 2.f, _detHeight / 2.f) + Pose::getCenter();
}
std::array<Eigen::Vector3f, 4> AcquisitionPose::getDetectorCorners() const
{
    return std::array<Eigen::Vector3f, 4> {getDetectorUpperLeft(), getDetectorLowerLeft(), getDetectorLowerRight(), getDetectorUpperRight()};
}

Eigen::ParametrizedLine<float, 3> AcquisitionPose::getRay(int horizontal, int vertical)  const
{
    Eigen::Vector3f pixel_center = getPixelCenter(horizontal, vertical);

    Eigen::Vector3f dir = pixel_center - getSourcePosition();
    dir.normalize();

    return Eigen::ParametrizedLine<float, 3>(getSourcePosition(), dir);
}

Eigen::Vector3f AcquisitionPose::getPixelCenter(int horizontal, int vertical) const
{
    Eigen::Vector3f right = getRotation() * Eigen::Vector3f(0.f, -1.f, 0.f);
    Eigen::Vector3f down = getRotation() * Eigen::Vector3f(0.f, 0.f, -1.f);

    Eigen::Vector3f pxl_dist_h = right * (_detWidth / _pxlHorizontal);
    Eigen::Vector3f pxl_dist_v = down * (_detHeight / _pxlVertical);

    Eigen::Vector3f pixel_center = getDetectorUpperLeft() + pxl_dist_h * (1. / 2.) + pxl_dist_h * horizontal + pxl_dist_v * (1. / 2.) + pxl_dist_v * vertical;

    return pixel_center;
}
