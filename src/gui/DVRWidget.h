#pragma once

#include <algorithm>
#include <iostream>

#include <QObject>
#include <QPainter>
#include <QWidget>

#include <QKeyEvent>

#include "DVRCamera.h"
#include "DVRModel.h"
#include "MIP.h"
#include "RayTracing.h"
#include "VisualizationModel.h"
#include "Volume.h"
#include "CameraPose.h"

/**
 * Implementation of orthographic
 * direct volume rendering (DVR).
 *
 * Currently only one axis roation (X value stays fixed)!
 * @brief The DVRWidget class
 */
class DVRWidget : public QWidget
{
    Q_OBJECT
private:
    VisualizationModel& _visModel;
    DVRModel& _dvrModel;
    

public:
    virtual void keyPressEvent(QKeyEvent *event) override;
    
    
    DVRWidget(VisualizationModel& visModel);

    void paintEvent(QPaintEvent* p_e) override;

    void setColor(QColor color);

    QColor color() const;


    void setStepSize(float stepSize);

    void setColorRange(float from, float to);
    
    void updateVolume(Volume vol);
    
public slots:
    void changedPose();
    
};
