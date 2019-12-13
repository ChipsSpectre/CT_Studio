#ifndef DVRMODEL_H
#define DVRMODEL_H

#include <Eigen/Eigen>
#include <QObject>
#include <QWidget>

#include "TransferFunction.h"
#include "CameraPose.h"
#include "Volume.h"
//#include "VisualizationModel.h"


class DVRModel : public QObject
{
    Q_OBJECT
private:
    float _stepSize;
    bool _perspective;
    TransferFunction _transferFunction;
    QColor _color;
    
    CameraPose _pose;
    
    //VisualizationModel& _model;
    
public:
    DVRModel(float stepSize, TransferFunction transferFunction)
        :
          _stepSize{stepSize},
          _perspective{false},
          _transferFunction {transferFunction},
          _color(transferFunction.color(0))
    {
    }

    void setTransferFunction(TransferFunction transferFunction)
    {
        _transferFunction = transferFunction;
    }
    TransferFunction& transferFunction()
    {
        return _transferFunction;
    }
    void setColorRange(float from, float to)
    {
        _transferFunction.setRange(0, from, to);
        emit functionChanged();
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

    void setStepSize(float stepsize)
    {
        _stepSize = stepsize;
        emit redraw();
    }

    float stepSize() const
    {
        return _stepSize;
    }

    
    CameraPose& getCameraPose()
    {
        return _pose;
    }
    
signals:
    void redraw();
    
    void updateRotationY();
    void updateRotationZ();
    void updateZoom();
    void updateDistance();
    void updateStepSize();
    
    // used by DVRControlWidget
    void volumeChanged(const Volume& vol);
    void functionChanged();
    
public slots:
    void changedVolume(const Volume& vol)
    {
        setColorRange(0, vol.maxEntry());
        emit volumeChanged(vol);
    }
    
    
    void setRotationYDeg(float y)
    {
        float radians = y / 180.f * M_PI;
        _pose.setRotationY(radians);
        emit redraw();
    }
    void setRotationZDeg(float z)
    {
        float radians = z / 180.f * M_PI;
        _pose.setRotationZ(radians);
        emit redraw();
    }
    void setDistance(float distance)
    {
        _pose.setDistance(distance);
        emit redraw();
    }
    void setZoom(float zoom)
    {
        _pose.setZoom(zoom);
        emit redraw();
    }
    void setPerspective(bool persp)
    {
        _perspective = persp;
        emit redraw();
    }
    
public:
    float getRotationYDeg()
    {
        float deg = _pose.getRotationY() / M_PI * 180.f;
        deg = fmod(deg, 360.f);
        if(deg < 0) deg = 360.f - deg;
        return deg;
    }
    
    float getRotationZDeg()
    {
        float deg = _pose.getRotationZ() / M_PI * 180.f;
        deg = fmod(deg, 360.f);
        if(deg < 0) deg = 360.f - deg;
        return deg;
    }
    
    float getDistance()
    {
        return _pose.getDistance();
    }
    
    float getZoom()
    {
        return _pose.getZoom();
    }
    
    bool getPerspective()
    {
        return _perspective;
    }
    
};

#endif // DVRMODEL_H
