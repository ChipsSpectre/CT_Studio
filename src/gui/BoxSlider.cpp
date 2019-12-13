#include "BoxSlider.h"

BoxSlider::BoxSlider(float minValue, float maxValue, int decimal, float singleStep)
    :
    _justChanged {false},
    _slider {new QSlider()},
    _box {new QDoubleSpinBox()},
    _layout {new QHBoxLayout()}
{
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    _slider->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    _box->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    //_box->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    
    _slider->setOrientation(Qt::Horizontal);
    _slider->setRange(0, static_cast<int>((maxValue - minValue) / singleStep));
    _slider->setSingleStep(1);
    
    _box->setRange(minValue, maxValue);
    _box->setSingleStep(singleStep);
    _box->setDecimals(decimal);
    
    _layout->addWidget(_slider);
    _layout->addWidget(_box);
    _layout->setStretch(0, 2);
    _layout->setStretch(1, 1);
    
    setLayout(_layout);
    _layout->setContentsMargins(0, 0, 0, 0);
    
    connect(_box, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &BoxSlider::boxChanged);
    connect(_slider, &QSlider::valueChanged, this, &BoxSlider::sliderChanged);
}

void BoxSlider::boxChanged(float value)
{
    if (catchEvent()) return;
    
    float min = static_cast<float>(_box->minimum());
    float max = static_cast<float>(_box->maximum());
    float step = static_cast<float>(_box->singleStep());
    int iValue = static_cast<int>((value - min) / step);
    
    _slider->setValue(iValue);
    emit valueChanged(value);
}

void BoxSlider::sliderChanged(int value)
{
    if (catchEvent()) return;
    
    float min = static_cast<float>(_box->minimum());
    float max = static_cast<float>(_box->maximum());
    float step = static_cast<float>(_box->singleStep());
    
    float fValue = min + value * step;
    
    _box->setValue(fValue);
    emit valueChanged(fValue);
}


void BoxSlider::changedValue(float value)
{
    _box->setValue(value);
    boxChanged(value);
    //emit valueChanged(value);
}

void BoxSlider::setMax(float max)
{
    _box->setMaximum(max);
    float min = static_cast<float>(_box->minimum());
    max = static_cast<float>(_box->maximum());
    float step = static_cast<float>(_box->singleStep());
    _slider->setMaximum(static_cast<int>((max - min) / step));
    
    emit boxChanged(_box->value());
}
void BoxSlider::setMin(float min)
{
    _box->setMinimum(min);
    
    min = static_cast<float>(_box->minimum());
    float max = static_cast<float>(_box->maximum());
    float step = static_cast<float>(_box->singleStep());
    _slider->setMaximum(static_cast<int>((max - min) / step));
    
    emit boxChanged(_box->value());
}

void BoxSlider::increaseRange(float min, float max)
{
    if(min < _box->minimum())
    {
        setMin(min);
    }
    if(max > _box->maximum())
    {
        setMax(max);
    }
}


bool BoxSlider::catchEvent()
{
    if(_justChanged)
    {
        _justChanged = false;
        return true;
    }
    else
    {
        _justChanged = true;
        return false;
    }
}