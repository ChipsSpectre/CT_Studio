#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>

#include <QImage>
#include <QMainWindow>
#include <QPushButton>
#include <QString>
#include <QVBoxLayout>
#include <QRect>

#include "AcquisitionWidget.h"
#include "Acquisition.h"
#include "PoseViewer.h"
#include "ReconstructionViewer.h"
#include "VisualizationWidget.h"

#include <iostream>
#include <QApplication>
#include <QDesktopWidget>
#include <QTimer>

/**
 * The main window contains all widgets of the application.
 * 
 * Different contexts are combined using a structure with tabs.
 */
class MainWindow : public QMainWindow {
public:
    MainWindow();

private slots:
    void requestedAquisition();
    
    void requestedReconstruction();
    
private:
    AcquisitionWidget _aWidget;
    ReconstructionViewer _rWidget;
    VisualizationWidget _vWidget;
};

#endif // MAINWINDOW_H
