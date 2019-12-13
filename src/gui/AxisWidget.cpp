#include "AxisWidget.h"

void AxisWidget::paintEvent(QPaintEvent*) {
    const int minSidelength = std::min(this->height(), this->width());
    const float vecLength = minSidelength / 2.f;
    auto rot = _model.getRotation();
    std::array<Eigen::Vector3f, 3> axesVecs {
        rot * Eigen::Vector3f::UnitX() * vecLength * 0.8f,
        rot * Eigen::Vector3f::UnitY() * vecLength * 0.8f, 
        rot * Eigen::Vector3f::UnitZ() * vecLength * 0.8f};
            
    std::array<int, 3> order {0, 1, 2};
        
    //hardcoded bubblesort for three elements
    if(axesVecs.at(0).z() > axesVecs.at(1).z()){
        std::swap(axesVecs.at(0), axesVecs.at(1));
        std::swap(order.at(0), order.at(1));
    }
    if(axesVecs.at(1).z() > axesVecs.at(2).z()){
        std::swap(axesVecs.at(1), axesVecs.at(2));
        std::swap(order.at(1), order.at(2));
    }
    if(axesVecs.at(0).z() > axesVecs.at(1).z()){
        std::swap(axesVecs.at(0), axesVecs.at(1));
        std::swap(order.at(0), order.at(1));
    }
        
    std::array<QString, 3> labels {"X", "Y", "Z"};
    std::array<QPen, 3> colors {QPen{QColor{Qt::red}}, QPen{QColor{Qt::green}}, QPen{QColor{Qt::blue}}};
        
    QPainter painter {this};
    painter.translate(this->width()/2, this->height()/2);
    painter.setPen(colors.at(order.at(0)));
    painter.drawLine(0, 0, axesVecs[0].x(), axesVecs[0].y());
    painter.drawText(axesVecs[0].x()*1.1, axesVecs[0].y()*1.1, labels.at(order.at(0)));
        
    painter.setPen(colors.at(order.at(1)));
    painter.drawLine(0, 0, axesVecs[1].x(), axesVecs[1].y());
    painter.drawText(axesVecs[1].x()*1.1, axesVecs[1].y()*1.1, labels.at(order.at(1)));
        
    painter.setPen(colors.at(order.at(2)));
    painter.drawLine(0, 0, axesVecs[2].x(), axesVecs[2].y());
    painter.drawText(axesVecs[2].x()*1.1, axesVecs[2].y()*1.1, labels.at(order.at(2)));
}
