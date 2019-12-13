#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>

#include <QImage>
#include <QMainWindow>
#include <QPushButton>
#include <QString>
#include <QVBoxLayout>

#include "AcquisitionWidget.h"
#include "PoseViewer.h"
#include "ReconstructionViewer.h"
#include "VisualizationWidget.h"

class MainWindow : public QMainWindow {
public:
    MainWindow()
    : _aWidget{"cube"}, _rWidget {}, _vWidget {} {
        QTabWidget* tabWidget = new QTabWidget();

        tabWidget->addTab(&_aWidget, "Acquisition");
        tabWidget->addTab(&_rWidget, "Reconstruction");
        tabWidget->addTab(&_vWidget, "Visualization");
        setCentralWidget(tabWidget);
        
        connect(&_rWidget, &ReconstructionViewer::requestAcquisition, this, &MainWindow::requestedAquisition);
        connect(&_vWidget, &VisualizationWidget::requestRecVolume, this, &MainWindow::requestedReconstruction);
    }

private slots:
    void requestedAquisition(){
        _rWidget.setAcq(_aWidget.getAcq());
    }
    
    void requestedReconstruction(){
        _vWidget.setRec(_rWidget.getRec());
    }
    
private:
    AcquisitionWidget _aWidget;
    ReconstructionViewer _rWidget;
    VisualizationWidget _vWidget;
};

#endif // MAINWINDOW_H
