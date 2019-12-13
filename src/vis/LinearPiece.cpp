#include "LinearPiece.h"

LinearPiece::LinearPiece(float intensity0, float intensity1, int opacity0, int opacity1, QColor rgb)
    :
    _intensity0(intensity0),
    _intensity1(intensity1),
    _opacity0(opacity0),
    _opacity1(opacity1),
    _rgb(rgb)
{}

QColor LinearPiece::apply(float intensity) const
{
    if(intensity < _intensity0)
    {
        // no opacity
        QColor color {_rgb};
        color.setAlpha(0);
        return color;
    }
    if(intensity > _intensity1)
    {
        // full opacity
        QColor color {_rgb};
        color.setAlpha(255);
        return color;
    }
    QColor color {_rgb};
    int opacity = static_cast<int>(_opacity0 + (_opacity1 - _opacity0) * (intensity - _intensity0) / (_intensity1 - _intensity0));
    color.setAlpha(std::max(std::min(opacity, 255), 0));
    return color;
}

void LinearPiece::setColor(QColor color)
{
    _rgb = color;
}

QColor LinearPiece::color() const
{
    return _rgb;
}

void LinearPiece::setIntensity0(float intensity0)
{
    _intensity0 = intensity0;
}

void LinearPiece::setIntensity1(float intensity1)
{
    _intensity1 = intensity1;
}


float LinearPiece::getIntensity0()
{
    return _intensity0;
}
float LinearPiece::getIntensity1()
{
    return _intensity1;
}