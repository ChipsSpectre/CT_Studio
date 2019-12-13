#include "MPRModel.h"

MPRModel::MPRModel()
    : 
      _transferFunction(TransferFunction(LinearPiece(0, 100, 0, 255, QColor::fromRgb(255,255,255)))),
      _color(QColor::fromRgb(255,255,255)),
      _pose(200, 200, 0.f)
{
}

MPRModel::MPRModel(TransferFunction transferFunction)
    : 
      _transferFunction(transferFunction)
{
}

int MPRModel::getPixelHorizontal()
{
    return _pose.getPixelHorizontal();
}
int MPRModel::getPixelVertical()
{
    return _pose.getPixelHorizontal();
}

Eigen::Vector3f MPRModel::getPixel(int horizontal, int vertical)
{
    return _pose.getPixel(horizontal, vertical);
}

void MPRModel::changedVolume(const Volume& vol)
{
    setColorRange(0.f, vol.maxEntry());
    emit volumeChanged(vol);
}

void MPRModel::setColorRange(float from, float to)
{
    _transferFunction.setRange(0, from, to);
    emit functionChanged();
}

void MPRModel::setZoom(float zoom)
{
    _pose.setZoom(zoom);
    emit redraw();
}
void MPRModel::setPositionX(float x)
{
    Eigen::Vector3f center = _pose.getCenter();
    _pose.setCenter(Eigen::Vector3f(x, center.y(), center.z()));
    emit redraw();
}
void MPRModel::setPositionY(float y)
{
    Eigen::Vector3f center = _pose.getCenter();
    _pose.setCenter(Eigen::Vector3f(center.x(), y, center.z()));
    emit redraw();
}
void MPRModel::setPositionZ(float z)
{
    Eigen::Vector3f center = _pose.getCenter();
    _pose.setCenter(Eigen::Vector3f(center.x(), center.y(), z));
    emit redraw();
}
void MPRModel::setRotationYDeg(float y)
{
    float radians = y / 180.f * M_PI;
    _pose.setRotationY(radians);
    emit redraw();
}
void MPRModel::setRotationZDeg(float z)
{
    float radians = z / 180.f * M_PI;
    _pose.setRotationZ(radians);
    emit redraw();
}

float MPRModel::getPositionX()
{
    return _pose.getCenter().x();
}
float MPRModel::getPositionY()
{
    return _pose.getCenter().y();
}
float MPRModel::getPositionZ()
{
    return _pose.getCenter().z();
}

float MPRModel::getRotationYDeg()
{
    float deg = _pose.getRotationY() / M_PI * 180.f;
    return deg;
}
float MPRModel::getRotationZDeg()
{
    float deg = _pose.getRotationZ() / M_PI * 180.f;
    return deg;
}

float MPRModel::getZoom()
{
    return _pose.getZoom();
}
