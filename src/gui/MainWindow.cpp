#include "MainWindow.h"

MainWindow::MainWindow()
: _aWidget{"cube"}, _rWidget {}, _vWidget {} {
    QTabWidget* tabWidget = new QTabWidget();

    tabWidget->addTab(&_aWidget, "Acquisition");
    tabWidget->addTab(&_rWidget, "Reconstruction");
    tabWidget->addTab(&_vWidget, "Visualization");
    setCentralWidget(tabWidget);
    
    connect(&_rWidget, &ReconstructionViewer::requestAcquisition, this, &MainWindow::requestedAquisition);
    connect(&_vWidget, &VisualizationWidget::requestRecVolume, this, &MainWindow::requestedReconstruction);

    QTimer::singleShot(0, this, SLOT(showFullScreen()));
}

void MainWindow::requestedAquisition(){
    _rWidget.setAcq(_aWidget.getAcq());
}

void MainWindow::requestedReconstruction(){
    _vWidget.setRec(_rWidget.getRec());
}
