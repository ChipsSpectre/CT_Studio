#pragma once

#include <QWidget>
#include <QPainter>
#include <QKeyEvent>

#include "AcquisitionModel.h"

/*
 * Only paints the current aquisition pose
 */
class PoseDisplay : public QWidget
{
    Q_OBJECT
private:
    int _xAxis;
    int _yAxis;
    
    bool _showRays;
    int _zoom;
    int _axis;
	AcquisitionModel &_model;


public:

	PoseDisplay(AcquisitionModel& model);
	PoseDisplay(AcquisitionModel& model, int axis);

    virtual void paintEvent(QPaintEvent *) override;
    
    virtual void keyPressEvent(QKeyEvent * event) override;
    
    void mousePressEvent(QMouseEvent * ) override{
        setFocus(Qt::MouseFocusReason);
    }
    
public slots:
    void setShowRays(int state);
    void setZoom(int zoom);
    
private:
    void paintPose(QPainter& painter, AcquisitionPose& pose, bool lowOpacity, bool onlySources);
signals:
    void sceneChanged();

};
