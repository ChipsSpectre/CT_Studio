#ifndef MPRCONTROLWIDGET_H
#define MPRCONTROLWIDGET_H


#include <QWidget>
#include <QLabel>
#include <QSlider>
#include <QSpinBox>
#include <QGridLayout>
#include <QColorDialog>
#include <QObject>
#include <QGroupBox>

#include "MPRModel.h"
#include "BoxSlider.h"
#include "TransferFuncWidget.h"

class MPRControlWidget : public QGroupBox
{
    Q_OBJECT
private:
    MPRModel& _model;
    
    
    BoxSlider* _posX;
    BoxSlider* _posY;
    BoxSlider* _posZ;
    
    BoxSlider* _rotY;
    BoxSlider* _rotZ;
    
    BoxSlider* _zoom;
    
    TransferFuncWidget* _func;


    QGridLayout* _layout;
    
    
public:
    MPRControlWidget(MPRModel& model);

public slots:
    void updatePositionX();
    void updatePositionY();
    void updatePositionZ();
    
    void updateRotationY();
    void updateRotationZ();
    
    void updateZoom();
    
    void changedVolume(const Volume& vol);
    
};

#endif