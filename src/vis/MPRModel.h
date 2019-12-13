#pragma once
#ifndef MPRMODEL_H
#define MPRMODEL_H

#include <Eigen/Eigen>
#include <Eigen/Dense>
#include <QObject>

#include "TransferFunction.h"
#include "CameraPose.h"
#include "Volume.h"

typedef Eigen::Vector3f Position;

/**
 * The MPRModel contains all data needed only for the
 * MPR visualization process.
 * @brief The MPRModel class
 */
class MPRModel : public QObject
{
    Q_OBJECT
private:
    float _distance;
    Eigen::Vector3f _normal;
    TransferFunction _transferFunction;
    
    CameraPose _pose;
    
    QColor _color;

public:
    MPRModel();

    MPRModel(TransferFunction transferFunction);


    void setTransferFunction(TransferFunction transferFunction)
    {
        _transferFunction = transferFunction;
    }
    TransferFunction& transferFunction()
    {
        return _transferFunction;
    }

    void setColor(QColor color)
    {
        _color = color;
        _transferFunction.setColor(0, color);
    }

    const QColor& color() const
    {
        return _color;
    }
    
public:
    void changedVolume(const Volume& vol);
    
    void setColorRange(float from, float to);
    
    int getPixelHorizontal();
    int getPixelVertical();
    
    Eigen::Vector3f getPixel(int horizontal, int vertical);
    
    void setPositionX(float x);
    void setPositionY(float y);
    void setPositionZ(float z);
    
    void setRotationYDeg(float y);
    void setRotationZDeg(float z);
    
    void setZoom(float zoom);
    
    float getPositionX();
    float getPositionY();
    float getPositionZ();
    
    float getRotationYDeg();
    float getRotationZDeg();
    
    float getZoom();
    
    Eigen::Matrix3f getRotation() const{
        return _pose.getRotation();
    }
    
signals:
    void redraw();
    void volumeChanged(const Volume& vol);
    void functionChanged();
    
    
};

#endif //MPRMODEL_H
