#include <math.h>

#include <iostream>

#include "CameraPose.h"



CameraPose::CameraPose()
    :
    CameraPose(200, 200)
{}


CameraPose::CameraPose(int pixelHorizontal, int pixelVertical, float distance)
    :
    Pose(Eigen::Vector3f(-1.f, 0.f, 0.f), distance),
    _pxlHorizontal{pixelVertical},
    _pxlVertical{pixelVertical},
    _down{0.f, 0.f, -1.f},
    _right{0.f, 1.f, 0.f},
    _screenCenter{getPoint() - getNormal() * 0.5f}
{
    setZoom(1.f);
}

void CameraPose::setZoom(float zoom)
{
    _zoom = zoom;
    float screenWidth = 0.5f / _zoom;
    _pixelSize = screenWidth / _pxlHorizontal;
}
float CameraPose::getZoom()
{
    return _zoom;
}

int CameraPose::getPixelHorizontal() const
{
    return _pxlHorizontal;
}
int CameraPose::getPixelVertical() const
{
    return _pxlVertical;
}

Eigen::ParametrizedLine<float, 3> CameraPose::getRayPerspective(int horizontal, int vertical)
{
    Eigen::Vector3f pixel = getPixel(horizontal, vertical, _screenCenter);
    Eigen::Vector3f direction = pixel - getPoint();
    direction.normalize();
    return Eigen::ParametrizedLine<float, 3>(getPoint(), direction);
}
Eigen::ParametrizedLine<float, 3> CameraPose::getRayOrthogonal(int horizontal, int vertical)
{
    return Eigen::ParametrizedLine<float, 3>(getPixel(horizontal, vertical, getPoint()), -getNormal());
}

void CameraPose::setDistance(float distance)
{
    Pose::setDistanceToCenter(distance);
    updatePose();
}
float CameraPose::getDistance()
{
    return getPoint().norm();
}


void CameraPose::updatePose()
{
    Pose::updatePose();
    _screenCenter = getPoint() - getNormal() * 0.5f;
    _down = getRotation() * Eigen::Vector3f(0.f, 0.f, -1.f);
    _right = getRotation() * Eigen::Vector3f(0.f, 1.f, 0.f);
}

Eigen::Vector3f CameraPose::getPixel(int horizontal, int vertical)
{
    return getPixel(horizontal, vertical, getCenter());
}

Eigen::Vector3f CameraPose::getPixel(int horizontal, int vertical, Eigen::Vector3f screenCenter)
{
    Eigen::Vector3f position = screenCenter;
    
    float stepX = (_pxlHorizontal / 2.f - 0.5f) * _pixelSize;
    float stepY = (_pxlVertical / 2.f - 0.5f) * _pixelSize;
    
    position = position - stepX * _right;
    position = position - stepY * _down;
    
    position = position + _right * _pixelSize * horizontal;
    position = position + _down * _pixelSize * vertical;
    
    return position;
}