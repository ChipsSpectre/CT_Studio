#include "MPRControlWidget.h"


MPRControlWidget::MPRControlWidget(MPRModel& model)
    :
    QGroupBox ("MPR"),
    _model(model),
    _posX {new BoxSlider(-.2f, .2f, 4, 0.001f)},
    _posY {new BoxSlider(-.2f, .2f, 4, 0.001f)},
    _posZ {new BoxSlider(-.2f, .2f, 4, 0.001f)},
    _rotY{new BoxSlider(0.f, 360.f, 1, 2.5f)},
    _rotZ{new BoxSlider(0.f, 360.f, 1, 2.5f)},
    _zoom{new BoxSlider(0.f, 4.f, 4, 0.01f )},
    _func{new TransferFuncWidget(_model.transferFunction())},
    _layout{new QGridLayout}
{
    QLabel* posXLabel = new QLabel("X Position");
    QLabel* posYLabel = new QLabel("Y Position");
    QLabel* posZLabel = new QLabel("Z Position");
    
    QLabel* rotYLabel = new QLabel("Y Rotation");
    QLabel* rotZLabel = new QLabel("Z Rotation");
    
    QLabel* zoomLabel = new QLabel("Zoom");
    
    _layout->addWidget(posXLabel, 0, 0);
    _layout->addWidget(posYLabel, 1, 0);
    _layout->addWidget(posZLabel, 2, 0);
    _layout->addWidget(rotYLabel, 3, 0);
    _layout->addWidget(rotZLabel, 4, 0);
    _layout->addWidget(zoomLabel, 5, 0);
    
    
    _layout->addWidget(_posX, 0, 1);
    _layout->addWidget(_posY, 1, 1);
    _layout->addWidget(_posZ, 2, 1);
    _layout->addWidget(_rotY, 3, 1);
    _layout->addWidget(_rotZ, 4, 1);
    _layout->addWidget(_zoom, 5, 1);
    _layout->addWidget(_func, 6, 0, 1, 2);
    
    _layout->setAlignment(Qt::AlignTop);
    
    setLayout(_layout);
    
    connect(_posX, &BoxSlider::valueChanged, &_model, &MPRModel::setPositionX);
    connect(_posY, &BoxSlider::valueChanged, &_model, &MPRModel::setPositionY);
    connect(_posZ, &BoxSlider::valueChanged, &_model, &MPRModel::setPositionZ);
    
    connect(_rotY, &BoxSlider::valueChanged, &_model, &MPRModel::setRotationYDeg);
    connect(_rotZ, &BoxSlider::valueChanged, &_model, &MPRModel::setRotationZDeg);
    
    connect(_zoom, &BoxSlider::valueChanged, &_model, &MPRModel::setZoom);
    
    connect(_func, &TransferFuncWidget::functionChanged, &_model, &MPRModel::redraw);
    
    connect(&model, &MPRModel::volumeChanged, this, &MPRControlWidget::changedVolume);
    
    connect(&model, &MPRModel::functionChanged, _func, &TransferFuncWidget::changedFunction);
    
    updatePositionX();
    updatePositionY();
    updatePositionZ();
    updateRotationY();
    updateRotationZ();
    updateZoom();
}

void MPRControlWidget::changedVolume(const Volume& vol)
{
    float diag = vol.getBoundingBox().diagonal().norm() / 2.f;
    _posX->increaseRange(-diag, diag);
    _posY->increaseRange(-diag, diag);
    _posZ->increaseRange(-diag, diag);
}


void MPRControlWidget::updatePositionX()
{ _posX->changedValue(_model.getPositionX()); }
void MPRControlWidget::updatePositionY()
{ _posY->changedValue(_model.getPositionY()); }
void MPRControlWidget::updatePositionZ()
{ _posZ->changedValue(_model.getPositionZ()); }

void MPRControlWidget::updateRotationY()
{ _rotY->changedValue(_model.getRotationYDeg()); }
void MPRControlWidget::updateRotationZ()
{ _rotZ->changedValue(_model.getRotationZDeg()); }

void MPRControlWidget::updateZoom()
{ _zoom->changedValue(_model.getZoom()); }