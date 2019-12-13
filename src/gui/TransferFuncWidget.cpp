#include "TransferFuncWidget.h"


TransferFuncWidget::TransferFuncWidget(TransferFunction& function)
    :
    QGroupBox("Transfer Function"),
    _function {function},
    _iMin {new QDoubleSpinBox},
    _iMax {new QDoubleSpinBox},
    _color {new QPushButton{"Color"}},
    _layout {new QHBoxLayout}
{
    QLabel* minLabel = new QLabel("min");
    QLabel* maxLabel = new QLabel("max");
    minLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    maxLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    
    _iMin->setRange(0.f, 100.f);
    _iMax->setRange(0.f, 100.f);
    
    _iMin->setDecimals(4);
    _iMax->setDecimals(4);
    
    
    _layout->addWidget(minLabel);
    _layout->addWidget(_iMin);
    
    _layout->addWidget(maxLabel);
    _layout->addWidget(_iMax);
    
    _layout->addWidget(_color);
    
    setLayout(_layout);
    
    connect(_iMin, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &TransferFuncWidget::setIMin);
    connect(_iMax, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &TransferFuncWidget::setIMax);
    connect(_color, &QPushButton::pressed, this, &TransferFuncWidget::changeColor);
}

void TransferFuncWidget::setIMin(float min)
{
    if(!_function.empty())
    {
        _function.getPieces()[0].setIntensity0(min);
    }
    emit functionChanged();
}
void TransferFuncWidget::setIMax(float max)
{
    if(!_function.empty())
    {
        _function.getPieces()[0].setIntensity1(max);
    }
    emit functionChanged();
}
void TransferFuncWidget::changeColor()
{
    if(!_function.empty())
    {
        QColor newColor = QColorDialog::getColor(_function.color(0), parentWidget());
        _function.getPieces()[0].setColor(newColor);

    }
}

void TransferFuncWidget::changedFunction()
{
    if(!_function.empty())
    {
        _iMin->setValue(_function.getPieces()[0].getIntensity0());
        _iMax->setValue(_function.getPieces()[0].getIntensity1());
    }
}
