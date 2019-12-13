#ifndef AXISWIDGET_H
#define AXISWIDGET_H

#include <array>

#include <QWidget>
#include <QPainter>
#include <Eigen/Geometry>

#include "MPRModel.h"

class AxisWidget : public QWidget
{
    Q_OBJECT
private:
    const MPRModel& _model; 
    
    
public:
    AxisWidget(const MPRModel& model) : _model {model} {
        connect(&_model, &MPRModel::redraw, this, &AxisWidget::test);
    }

    void paintEvent(QPaintEvent *) override;
    

public slots:
    
    void test (){
        update();
    }
};

#endif // AXISWIDGET_H
