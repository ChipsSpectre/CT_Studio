#ifndef TRANSFERFUNCWIDGET_H
#define TRANSFERFUNCWIDGET_H

#include <QWidget>
#include <QGroupBox>
#include <QSpinBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>

#include "TransferFunction.h"

class TransferFuncWidget : public QGroupBox
{
    Q_OBJECT
private:
    
    TransferFunction& _function;
    
    QDoubleSpinBox* _iMin;
    QDoubleSpinBox* _iMax;
    QPushButton* _color;
    
    QHBoxLayout* _layout;
    
public:
    TransferFuncWidget(TransferFunction& function);
    
private:
    void setIMin(float min);
    void setIMax(float max);
    void changeColor();

public slots:
    void changedFunction();

signals:
    void functionChanged();

    
};

#endif