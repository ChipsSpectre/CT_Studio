#include "DVRControlWidget.h"

DVRControlWidget::DVRControlWidget(DVRModel& model)
    :
    QGroupBox {"Direct Volume Rendering"},
    _model {model},
    _camera {model.getCameraPose()},
    _rotY {new BoxSlider(0.f, 360.f, 1, 2.5f)},
    _rotZ {new BoxSlider(0.f, 360.f, 1, 2.5f)},
    _zoom {new BoxSlider(0.f, 4.f, 4, 0.01f )},
    _distance {new BoxSlider(0.f, 1.5f, 4, 0.01f)},
    _step {new BoxSlider(0.f, 0.5f, 5, 0.0005f)},
    _perspective {new QCheckBox()},
    _func {new TransferFuncWidget(_model.transferFunction())},
    _layout {new QGridLayout()}
{
    
    QLabel* rotYLabel  = new QLabel("Y Rotation");
    QLabel* rotZLabel  = new QLabel("Z Rotation");
    QLabel* zoomLabel  = new QLabel("Zoom");
    QLabel* distanceLabel = new QLabel("Distance");
    QLabel* stepLabel = new QLabel("Step Size");
    QLabel* perspectiveLabel = new QLabel("Perspective");
    
    _layout->addWidget(perspectiveLabel, 0, 0);
    _layout->addWidget(rotYLabel, 1, 0);
    _layout->addWidget(rotZLabel, 2, 0);
    _layout->addWidget(zoomLabel, 3, 0);
    _layout->addWidget(distanceLabel, 4, 0);
    _layout->addWidget(stepLabel, 5, 0);
    
    _layout->addWidget(_perspective, 0, 1);
    _layout->addWidget(_rotY, 1, 1);
    _layout->addWidget(_rotZ, 2, 1);
    _layout->addWidget(_zoom, 3, 1);
    _layout->addWidget(_distance, 4, 1);
    _layout->addWidget(_step, 5, 1);
    
    _layout->addWidget(_func, 6, 0, 1, 2);
    
    _layout->setAlignment(Qt::AlignTop);
    
    
    setLayout(_layout);
    
    //connects
    connect(_perspective, &QCheckBox::stateChanged, &_model, &DVRModel::setPerspective);
    connect(_rotY, &BoxSlider::valueChanged, &_model, &DVRModel::setRotationYDeg);
    connect(_rotZ, &BoxSlider::valueChanged, &_model, &DVRModel::setRotationZDeg);
    connect(_distance, &BoxSlider::valueChanged, &_model, &DVRModel::setDistance);
    connect(_zoom, &BoxSlider::valueChanged, &_model, &DVRModel::setZoom);
    connect(_step, &BoxSlider::valueChanged, &_model, &DVRModel::setStepSize);
    
    connect(_func, &TransferFuncWidget::functionChanged, &_model, &DVRModel::redraw);
    
    
    connect(&model, &DVRModel::updateRotationY, this, &DVRControlWidget::updateRotationY);
    connect(&model, &DVRModel::updateRotationZ, this, &DVRControlWidget::updateRotationZ);
    connect(&model, &DVRModel::updateZoom, this, &DVRControlWidget::updateZoom);
    connect(&model, &DVRModel::updateDistance, this ,&DVRControlWidget::updateDistance);
    connect(&model, &DVRModel::updateStepSize, this ,&DVRControlWidget::updateStepSize);
    
    connect(&model, &DVRModel::volumeChanged, this, &DVRControlWidget::changedVolume);
    
    connect(&model, &DVRModel::functionChanged, _func, &TransferFuncWidget::changedFunction);
    
    updateRotationY();
    updateRotationZ();
    updateZoom();
    updateDistance();
    updateStepSize();
}

void DVRControlWidget::changedVolume(const Volume& vol)
{
    _distance->increaseRange(0.f, 4 * vol.getBoundingBox().diagonal().norm());
}


void DVRControlWidget::updateRotationY()
{ _rotY->changedValue(_model.getRotationYDeg()); }
void DVRControlWidget::updateRotationZ()
{ _rotZ->changedValue(_model.getRotationZDeg()); }
void DVRControlWidget::updateZoom()
{ _zoom->changedValue(_model.getZoom()); }
void DVRControlWidget::updateDistance()
{ _distance->changedValue(_model.getDistance()); }
 void DVRControlWidget::updateStepSize()
{ _step->changedValue(_model.stepSize());}