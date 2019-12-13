#pragma once
#ifndef VISUALIZATIONWIDGET_H
#define VISUALIZATIONWIDGET_H

#include <iostream>
#include <memory>
#include <vector>
#include <array>

#include <QAbstractSlider>
#include <QColorDialog>
#include <QFileDialog>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QMessageBox>
#include <QObject>
#include <QPushButton>
#include <QSlider>
#include <QSpinBox>
#include <QWidget>

#include "DVRWidget.h"
#include "MPRWidget.h"
#include "VisualizationModel.h"
#include "Volume.h"

#include "DVRControlWidget.h"
#include "MPRControlWidget.h"
#include "AxisWidget.h"

/**
 * Widget to configure and execute visualization algorithms.
 *
 * Possible algorithms:
 *  2D-MPR (multiple planar reconstruction)
 *  3D-DVR (direct volume rendering)
 * @brief The VisualizationWidget class
 */
class VisualizationWidget : public QWidget
{
    Q_OBJECT
private:
    ///manages all data related to visualization
    VisualizationModel _visModel;

    ///loads the Volume from a file
    QPushButton *_loadFileButton;    
    ///loads the Volume from the ReconstructionPose
    QPushButton *_loadRecButton;
    ///displays the current color for mpr
    QLabel *_mprColorLabel;
    ///opens a dialog to choose the color for visualization for mpr
    QPushButton *_mprSelectColorButton;
    ///resets color of transfer function to default values for mpr
    QPushButton *_mprColorResetButton;
    ///shows the dimensions of the volume on screen
    QLabel *_volumeInfoLabel;
    ///renders the 2D MPR visualization
    MPRWidget *_mprWidget;
    ///paints the direct volume rendering (DVR)
    DVRWidget *_dvrWidget;
    ///displays the current color for dvr
    QLabel *_dvrColorLabel;
    ///opens a dialog to choose the color for visualization for dvr
    QPushButton *_dvrSelectColorButton;
    ///resets color of transfer function to default values for dvr
    QPushButton *_dvrColorResetButton;    
    ///paints the axes for the mpr to visualize the current rotation
    AxisWidget *_axisWidget;    
    DVRControlWidget* _dvrControlWidget;
    MPRControlWidget* _mprControlWidget;

    void mprUpdateColorLabel()
    {

    }

    void dvrUpdateColorLabel()
    {

    }

    /**
     *  Updates the info label which
     *  displays the current volume size.
     */
    void updateVolumeInfo();

    /**
     * Helper method, called after change of volume data.
     * Updates the user interface accordingly.
     * @brief updateVolumeChanged
     */
    void updateVolumeChanged();

    /**
     * Reset the DRW widget to its default position and
     * calibrate the camera for a new volume.
     * @brief updateDRWWidget
     */
    void updateDVRWidget();

public:    
    /**
     * Construct a new Visualization Widget and setup
     * the whole user interface.
     * @brief VisualizationWidget
     */
    VisualizationWidget();
    
    /**
     * Updates the current volume.
     * This method makes it possible to load reconstruction
     * results into the visualization widget.
     * @brief setRec
     * @param vol - shared pointer to reference to new volume.
     */
    void setRec(const std::shared_ptr<const Volume>& vol);
    
signals:
    /**
     * emit this event if you want the volume from the requisition. It is set through a call of setRec.
     */
    void requestRecVolume();
    
public slots:
    void mprResetColor()
    {
        _mprWidget->setColorRange(0, _visModel.volume().maxEntry());
        _mprWidget->setColor(QColor::fromRgb(255, 255, 255));
        mprUpdateColorLabel();
        update();
    }

    void dvrResetColor()
    {
        _dvrWidget->setColorRange(0, _visModel.volume().maxEntry());
        _dvrWidget->setColor(QColor::fromRgb(255, 255, 255));
        dvrUpdateColorLabel();
        update();
    }

    /**
     * Loads a 3D medical image file into a Volume.
     *
     * After that, the visualization widgets are reset to the default viewpoints.
     * @brief loadFromFile
     */
    void loadFromFile();
};

#endif // VISUALIZATIONWIDGET_H
