#pragma once

#include <algorithm>
#include <iostream>
#include <vector>
#include <memory>

#include <QGridLayout>
#include <QLabel>
#include <QPainter>
#include <QSlider>
#include <QWheelEvent>
#include <QWidget>
#include <QImage>

#include "Vec3D.h"
#include "Volume.h"
#include "ReconstructionModel.h"
#include "Acquisition.h"

class SliceWidget : public QWidget
{
    Q_OBJECT
private:
    int _currSlice;
    int _status;
    std::unique_ptr<ReconstructionModel> _model;
    QImage _image;
    
    bool checkModelInvalidity();

public:
    SliceWidget() : _currSlice(0), _status(2), _model {nullptr}, _image {} {
        QPalette pal = palette();
        pal.setColor(QPalette::Background, Qt::black);
        this->setPalette(pal);
        this->setAutoFillBackground(true);
    }

    virtual
    void paintEvent(QPaintEvent *event);

    void wheelEvent(QWheelEvent* event);

    int currSlice()
    {
        return _currSlice;
    }

    int slices()
    {
        if(!_model){
            return 0;
        }
        switch(_status)
        {
            case 0:
                return _model->rec().content().sizeX();
            case 1:
                return _model->rec().content().sizeY();
            default:
                return _model->rec().content().sizeZ();
        }
    }
    
    /*
     * called when the user requests a new Acquisition.
     * 
     * regularized defines which DataContainer to use
     * lambda is the lambda for the regularized Container, it is only used when
     * regularized is actually true, otherwise it is discarded
     */
    void setAcq(bool regularized, float lambda, bool noisy, float noise, int cgIterations, Acquisition&& acq){
        _model = std::make_unique<ReconstructionModel>(regularized, lambda, noisy, noise, cgIterations, std::move(acq));
        if(checkModelInvalidity()){
            return;
        }
        update();
        //Important reset, if the volume boundaries would recParamChanged
        _currSlice = 0;
        emit sliceChanged();
    }
    
    /*
     * called one of the parmeters is changed by the user in the ReconstructionView
     * 
     * param. description see above
     */
    void recParamChanged(bool regularized, float lambda, int cgIterations){
        if(!_model || checkModelInvalidity()){
            return;
        }
        _model->changeReconstructionParams(regularized, lambda, cgIterations);
        update();
    }
    
    void noiseChanged(bool noisy, float noise, int cgIterations){
        if(!_model || checkModelInvalidity()){
            return;
        }
        _model->changeNoise(noisy, noise, cgIterations);
        update();
    }
    
    std::shared_ptr<const Volume> getRec(){
        if(!_model || _model->invalid()){
            return nullptr;
        }
        return _model->getRec();
    }
    
    bool containsRec(){
        return bool(_model);
    }

signals:
    void sliceChanged();

public slots:

    void updateStatus(int newStatus)
    {
        newStatus--; //zero-indexed slice widget
        if(newStatus != _status)
        {
            _status = newStatus;
            _currSlice = 0;
        }
        update();
    }
};
