#pragma once

#include <iostream>

#include <QWidget>
#include <QGridLayout>
#include <QSpinBox>
#include <QCheckBox>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
// #include <QScrollArea>


#include "AcquisitionModel.h"
#include "PoseDisplay.h"
#include "AcquisitionPose.h"


class PoseViewer : public QWidget
{
    Q_OBJECT
private:    
	// QScrollArea* scroll_area_;
	QSpinBox *_zoomBox;
	QCheckBox *_raysBox;
    
    /*
     * The number of positions for the automatic generation of Poses
     */
    QSpinBox *_poseCount1;
    QSpinBox *_poseCount2;
    QPushButton *_generatePosesButton;

	PoseDisplay *_poseDisplay;
    PoseDisplay *_poseDisplay2;
    PoseDisplay *_poseDisplay3;
    
    /*
     * Buttons to manipulate the pose stack in the AcquisitionModel
     */
    QPushButton *_savePoseButton;
    QPushButton *_deletePoseButton;
    QPushButton *_clearButton;
    
    /*
     * The next five widgets are used when the user wants to change aquisition pose parameters
     */ 
    QSpinBox *_verticalPixels;
    QSpinBox *_horizontalPixels;
    QDoubleSpinBox *_detectorWidth;
    QDoubleSpinBox *_detectorHeight;
    QDoubleSpinBox *_detectorSouceDistance;
    
    QPushButton *_loadFileButton;
    
    AcquisitionModel& _model;
	bool _showRays;
	int _zoom;
    
    /*
     * The next two methods work exactly the same way. Their only difference is that the first 
     * is used for QDoubleSpinBoxes and the next for normal Spinboxes.
     * 
     * They check if the box content changed and if there are user modified poses, asks
     * if they should be deleted before changing the parameteters (and deleting the poses)
     */
    void floatParamChanged(QDoubleSpinBox *box, float oldVal){
        if(oldVal == (float)box->value()){
           return; 
        }       
        
        if(_model.containsModifiedElements()){
             if(QMessageBox::Ok != QMessageBox::question(this, "Delete Saved Poses", "When pose parameters are changed, all saved poses have to be deleted. Do you want to do this?", QMessageBox::Ok | QMessageBox::No, QMessageBox::Ok)){
                box->setValue(oldVal);
                return;
            }
        }
        
        
        _model.changePoseParams((float)_detectorSouceDistance->value(), 
                                (float)_detectorWidth->value(), (float)_detectorHeight->value(), 
                                _horizontalPixels->value(), _verticalPixels->value());
    }
    
    void intParamChanged(QSpinBox *box, int oldVal){
        if(oldVal == box->value()){
           return; 
        }
        
        if(_model.containsModifiedElements()){
            if(QMessageBox::Ok != QMessageBox::question(this, "Delete Saved Poses", "When pose parameters are changed all saved poses have to be deleted. Do you want to do this?", QMessageBox::Ok | QMessageBox::No, QMessageBox::Ok)){
                box->setValue(oldVal);
                return;
            }
        }
        
        
        _model.changePoseParams((float)_detectorSouceDistance->value(), 
                                (float)_detectorWidth->value(), (float)_detectorHeight->value(), 
                                _horizontalPixels->value(), _verticalPixels->value());
    }
    
public:
    
    PoseViewer(AcquisitionModel& model);

public slots:
    
    /*
     * The next five methods are called if the user might have changed the pose parameters.
     * 
     * Because of implementation problems these could not be merged into one function.
     */
    void detSourceDistanceChanged(){
        float oldVal = std::get<0>(_model.currPrototype());
        floatParamChanged(_detectorSouceDistance, oldVal);
    }
    
    void detectorWidthChanged(){
        float oldVal = std::get<1>(_model.currPrototype());
        floatParamChanged(_detectorWidth, oldVal);
        
    }
    
    void detectorHeightChanged(){
        float oldVal = std::get<2>(_model.currPrototype());
        floatParamChanged(_detectorHeight, oldVal);
        
    }
    
    void horizontalPixelsChanged(){
        int oldVal = std::get<3>(_model.currPrototype());
        intParamChanged(_horizontalPixels, oldVal);
    }
    
    void verticalPixelsChanged(){
        int oldVal = std::get<4>(_model.currPrototype());
        intParamChanged(_verticalPixels, oldVal);
        
    }
    
    void generatePoses(){
        _model.addSphericalPoses(_poseCount1->value(), _poseCount2->value());
    }

    void loadFile();
};
