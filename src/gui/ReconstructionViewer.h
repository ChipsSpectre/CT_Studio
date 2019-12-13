#pragma once

#include <QCheckBox>
#include <QGridLayout>
#include <QLabel>
#include <QObject>
#include <QSlider>
#include <QTextEdit>
#include <QWidget>
#include <QPushButton>
#include <QDoubleSpinBox>

#include "SliceWidget.h"
#include "Acquisition.h"

class ReconstructionViewer : public QWidget
{
    Q_OBJECT
private:
    QLabel *_statusView;
    QSlider *_axisSlider;
    SliceWidget *_sWidget;

    QPushButton *_loadButton;

    QLabel *_iterationLabel;
    QSlider *_iterationSlider;

    QCheckBox *_regCheckBox;
    QDoubleSpinBox *_regLambda;

    QPushButton *_updateButton;
    
    //input of the percentage of normally distributed noise
    QDoubleSpinBox *_noiseSpinBox;
    //used to mark wheter the error should actually be aplied
    QCheckBox *_noiseCheckBox;
    
    QPushButton *_updateNoiseButton;

public:

    ReconstructionViewer();

    void setAcq(Acquisition&& acq){
            _sWidget->setAcq(_regCheckBox->isChecked(),
                            static_cast<float>(_regLambda->value()),
                            _noiseCheckBox->isChecked(),
                            _noiseSpinBox->value()/100,
                            _iterationSlider->value(),
                            std::move(acq));
    }
    
    //on request returns the volume
    std::shared_ptr<const Volume> getRec(){
        return _sWidget->getRec();
    }
    
    
signals:
    //calls this function if you need the acquistion information. it is set through setAcq
    void requestAcquisition();
    
public slots:    

    /**
     * This method ensures that it is only possible to
     * enter a lambda into the corresponding text field,
     * if the regularization mode was enabled.
     * @brief updateRegText
     */
    void updateRegText()
    {
        _regLambda->setEnabled(_regCheckBox->isChecked());
    }
    
    void updateSpinBox(){
        _noiseSpinBox->setEnabled(_noiseCheckBox->isChecked());
    }

    void updateStatus() {
        _statusView->setText(QString("Current slice: "+QString::number(_sWidget->currSlice()+1)+
                                    " of "+QString::number(_sWidget->slices())));
        _iterationLabel->setText(QString("Iterations: "+QString::number(_iterationSlider->value())));
    }

    void updateReconstructionParams()
    {
        _sWidget->recParamChanged(_regCheckBox->isChecked(), _regLambda->value(),
                                 _iterationSlider->value());
    }
    
    void updateNoise(){
        _sWidget->noiseChanged(_noiseCheckBox->isChecked(), _noiseSpinBox->value()/100, _iterationSlider->value());
    }

    /**
     * This slot makes it possible to update the reconstruction.
     * This method is called after the reconstruction parameters
     * were changed.
     * @brief enableUpdate
     */
    void enableUpdate()
    {
        if(!_sWidget->containsRec()){
            return;
        }
        _updateButton->setEnabled(true);
    }
};
