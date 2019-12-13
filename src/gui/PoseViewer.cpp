#include "PoseViewer.h"

#include <QFileDialog>

PoseViewer::PoseViewer(AcquisitionModel& model) : 
    _zoomBox{new QSpinBox {}}, 
    _raysBox {new QCheckBox {}}, 
    _poseCount1 {new QSpinBox {}},
    _poseCount2 {new QSpinBox {}},
    _generatePosesButton {new QPushButton{"Generate Poses"}},
    _poseDisplay{new PoseDisplay {model}},
    _poseDisplay2{new PoseDisplay {model, 0}},
    _poseDisplay3{new PoseDisplay {model, 1}},
    _savePoseButton {new QPushButton{"Store Current Pose"}}, 
    _deletePoseButton {new QPushButton{"Delete Current Pose"}}, 
    _clearButton {new QPushButton{"Clear All Poses"}},
    _verticalPixels {new QSpinBox{}},
    _horizontalPixels {new QSpinBox{}},
    _detectorWidth {new QDoubleSpinBox {}},
    _detectorHeight {new QDoubleSpinBox {}},
    _detectorSouceDistance {new QDoubleSpinBox {}},
    _loadFileButton {new QPushButton {"Load New File"}},
     _model {model},
     _showRays {false},
     _zoom {100}
{
    QGridLayout *layout = new QGridLayout {};
    QGridLayout *viewsLayout = new QGridLayout {};
	QHBoxLayout *layoutBoxes = new QHBoxLayout {};
    QHBoxLayout *layoutButtons = new QHBoxLayout {};
    QHBoxLayout *generatorLayout = new QHBoxLayout {};
    QHBoxLayout *layoutVerticalPix = new QHBoxLayout {};
    QHBoxLayout *layoutHorizontalPix = new QHBoxLayout {};
    QHBoxLayout *layoutDetectorWidth = new QHBoxLayout {};
    QHBoxLayout *layoutDetectorHeight = new QHBoxLayout {};
    QHBoxLayout *layoutDetectorSouceDistance = new QHBoxLayout {};
    
    QLabel* raysLabel = new QLabel("Show Rays");
    QLabel* zoomLabel = new QLabel("Zoom");

    _zoomBox->setMinimum(1);
    _zoomBox->setMaximum(1000);
    _zoomBox->setValue(100);

    viewsLayout->addWidget(_poseDisplay, 0, 0);
    viewsLayout->addWidget(_poseDisplay2, 1, 1);
    viewsLayout->addWidget(_poseDisplay3, 1, 0);
    
    //_zoomBox.setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    raysLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    zoomLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    
    //_zoomBox.setAlignment(Qt::AlignLeft);
    layoutBoxes->addWidget(raysLabel);
    layoutBoxes->addWidget(_raysBox);
    layoutBoxes->addWidget(zoomLabel);
    layoutBoxes->addWidget(_zoomBox);
    layoutBoxes->setAlignment(Qt::AlignLeft);
    
    layout->addLayout(viewsLayout, 0, 0);
    layout->addLayout(layoutBoxes, 1, 0);
    
    
    layoutButtons->addWidget(_savePoseButton);
    layoutButtons->addWidget(_deletePoseButton);
    layoutButtons->addWidget(_clearButton);
    layout->addLayout(layoutButtons, 3, 0);
    
    
    layoutVerticalPix->addWidget(new QLabel{"#Vertical Detector Pixels"});
    layoutVerticalPix->addWidget(_verticalPixels);
    _verticalPixels->setRange(2, 400);
    _verticalPixels->setValue(5);
    layout->addLayout(layoutVerticalPix, 4, 0);
    
    layoutHorizontalPix->addWidget(new QLabel{"#Horizontal Detector Pixels"});
    layoutHorizontalPix->addWidget(_horizontalPixels);
    _horizontalPixels->setRange(2, 400);
    _horizontalPixels->setValue(5);
    layout->addLayout(layoutHorizontalPix, 5, 0);
    
    layoutDetectorWidth->addWidget(new QLabel{"Detector Width in Meter"});
    layoutDetectorWidth->addWidget(_detectorWidth);
    layout->addLayout(layoutDetectorWidth, 6, 0);
    _detectorWidth->setValue(0.2);
    _detectorWidth->setSingleStep(0.1);
    _detectorWidth->setRange(0.01, 10);
    
    layoutDetectorHeight->addWidget(new QLabel{"Detector Height in Meter"});
    layoutDetectorHeight->addWidget(_detectorHeight);
    layout->addLayout(layoutDetectorHeight, 7, 0);
    _detectorHeight->setValue(0.2);
    _detectorHeight->setSingleStep(0.1);
    _detectorHeight->setRange(0.01, 10);
    
    layoutDetectorSouceDistance->addWidget(new QLabel{"Detector-Source-Distance (x * Volume Diagonal)"});
    layoutDetectorSouceDistance->addWidget(_detectorSouceDistance);
    layout->addLayout(layoutDetectorSouceDistance, 8, 0);
    _detectorSouceDistance->setValue(1.5);
    _detectorSouceDistance->setSingleStep(0.1);
    _detectorSouceDistance->setRange(1.01, 10);
    
    connect(_verticalPixels, &QSpinBox::editingFinished, this, &PoseViewer::verticalPixelsChanged);
    connect(_horizontalPixels, &QSpinBox::editingFinished, this, &PoseViewer::horizontalPixelsChanged);
    connect(_detectorWidth, &QDoubleSpinBox::editingFinished, this, &PoseViewer::detectorWidthChanged);
    connect(_detectorHeight, &QDoubleSpinBox::editingFinished, this, &PoseViewer::detectorHeightChanged);
    connect(_detectorSouceDistance, &QDoubleSpinBox::editingFinished, this, &PoseViewer::detSourceDistanceChanged);
    
    
    QLabel* countLabel1 = new QLabel("Circles");
    countLabel1->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    QLabel* countLabel2 = new QLabel("Max Poses per Circle");
    countLabel2->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    
    generatorLayout->addWidget(countLabel1);
    generatorLayout->addWidget(_poseCount1);
    generatorLayout->addWidget(countLabel2);
    generatorLayout->addWidget(_poseCount2);
    generatorLayout->addWidget(_generatePosesButton);
    
    _poseCount1->setMinimum(1);
    _poseCount1->setValue(30);
    _poseCount2->setMinimum(1);
    _poseCount2->setValue(30);
    layout->addLayout(generatorLayout, 9, 0);
    
    layout->addWidget(_loadFileButton, 10, 0);
    connect(_loadFileButton, &QPushButton::pressed, this, &PoseViewer::loadFile);
    
    setLayout(layout);
    
    connect(_generatePosesButton, &QPushButton::pressed, this, &PoseViewer::generatePoses);
    
    connect(_savePoseButton, &QPushButton::pressed, &_model, &AcquisitionModel::savePose);
    connect(_deletePoseButton, &QPushButton::pressed, &_model, &AcquisitionModel::deletePose);
    connect(_clearButton, &QPushButton::pressed, &_model, &AcquisitionModel::clearPoses);
    
    connect(_savePoseButton, &QPushButton::pressed, &_model, &AcquisitionModel::savePose);
    connect(_deletePoseButton, &QPushButton::pressed, &_model, &AcquisitionModel::deletePose);
    connect(_clearButton, &QPushButton::pressed, &_model, &AcquisitionModel::clearPoses);

    
    connect(_raysBox, SIGNAL(stateChanged(int)), _poseDisplay, SLOT(setShowRays(int)));
    connect(_zoomBox, SIGNAL(valueChanged(int)), _poseDisplay, SLOT(setZoom(int)));
    
    connect(_raysBox, SIGNAL(stateChanged(int)), _poseDisplay2, SLOT(setShowRays(int)));
    connect(_zoomBox, SIGNAL(valueChanged(int)), _poseDisplay2, SLOT(setZoom(int)));
    
    connect(_raysBox, SIGNAL(stateChanged(int)), _poseDisplay3, SLOT(setShowRays(int)));
    connect(_zoomBox, SIGNAL(valueChanged(int)), _poseDisplay3, SLOT(setZoom(int)));

   //connect(&_poseDisplay, &PoseDisplay::sceneChanged(), this, PoseView::sceneChanged()));
}

void PoseViewer::loadFile() {
    QString filename = QFileDialog::getOpenFileName(this, "Open File", "", "Medical image data (*.edf)");
    if(filename.isEmpty()){
        return;
    }
    if(!_model.loadFile(filename)){
        QMessageBox::warning(this, "Unable to Open File", "The file couldn't be read!");
    }
    detSourceDistanceChanged();
    detectorWidthChanged();
    detectorHeightChanged();
    horizontalPixelsChanged();
    verticalPixelsChanged();
}
