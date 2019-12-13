#include "MPRWidget.h"

#include <iostream>

MPRWidget::MPRWidget(VisualizationModel& visModel)
    : _visModel {visModel},
      _mprModel {visModel.getMPRModel()}
{
    connect(&_mprModel, &MPRModel::redraw, this, &MPRWidget::changedPose);
}

void MPRWidget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    const Volume& vol = _visModel.volume();
    //_mprModel.transferFunction().setRange(0, 0, vol.maxEntry());
    
    if(vol.getTotalVoxelCount() == 0)
    {
        painter.drawText(width()/2, height()/2, "Error: No data loaded!");
        return;
    }
    
    painter.fillRect(0, 0, width(), height(), QColor::fromRgb(0,0,0));

    float scale = std::min(static_cast<float>(width()) / _mprModel.getPixelHorizontal(), static_cast<float>(height()) / _mprModel.getPixelVertical());
    painter.scale(scale, scale);
    
    for(int x = 0; x < _mprModel.getPixelHorizontal(); x++)
    {
        for(int y = 0; y < _mprModel.getPixelVertical(); y++)
        {
            //std::cout << _mprModel.getPixel(x, y) << std::endl;
            Eigen::Vector3f pixel = _mprModel.getPixel(x, y);
            float intensity = vol.getVoxelLinearPhysical(pixel);
            QColor color = _mprModel.transferFunction().classify(intensity);
            painter.fillRect(x, y, 1, 1, color);
        }
    }

}
void MPRWidget::setColor(QColor color)
{
    _mprModel.setColor(color);
}

QColor MPRWidget::color() const
{
    return _mprModel.color();
}

void MPRWidget::changedPose()
{
    update();
}
