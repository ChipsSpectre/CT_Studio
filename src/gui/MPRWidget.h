#pragma once

#include <iostream>

#include <QObject>
#include <QPainter>
#include <QPen>
#include <QWidget>

#include "MPRModel.h"
#include "RayTracing.h"
#include "VisualizationModel.h"

class MPRWidget : public QWidget
{
    Q_OBJECT

private:
    VisualizationModel& _visModel;
    MPRModel& _mprModel;

public:
    /**
     * Creates a new MPRWidget and sets up
     * the corresponding model classes.
     * Some data specific for MPR is contained inside
     * of the special MPRModel.
     * @brief MPRWidget
     * @param visModel
     */
    MPRWidget(VisualizationModel& visModel);

    /**
     * This method is called to paint the MPR on the
     * screen, every time the size of the window changes
     * or the repaint() method was called.
     * @brief paintEvent
     * @param p_e - PaintEvent, not used for MPR
     */
    void paintEvent(QPaintEvent* p_e) override;

public slots:


    /**
     * Sets the color used for the MPR transfer
     * function visualization.
     * @brief setColor
     * @param color
     */
    void setColor(QColor color);

    /**
     * Gets the current color used for the MPR transfer
     * function visualization.
     * @brief color
     * @return
     */
    QColor color() const;


    void setColorRange(float from, float to)
    {
        _mprModel.setColorRange(from,to);
    }

public slots:
    void changedPose();

};
