#include "ReconstructionViewer.h"

ReconstructionViewer::ReconstructionViewer() 
    : _statusView {new QLabel {}}, 
    _axisSlider {new QSlider {}}, 
    _sWidget {new SliceWidget {}}, 
    _loadButton {new QPushButton{"Load acquisition"}}, 
    _iterationLabel{new QLabel {}}, _iterationSlider{new QSlider {}}, 
    _regCheckBox{new QCheckBox{"Regularized Reconstruction"}}, _regLambda{new QDoubleSpinBox {}}, 
    _updateButton{new QPushButton{"Update reconstruction"}}, 
    _noiseSpinBox{new QDoubleSpinBox{}}, _noiseCheckBox {new QCheckBox{"Activate Gaussian Noise"}},
    _updateNoiseButton {new QPushButton {"Update Noise"}} {
    QGridLayout *layout = new QGridLayout {};
    QGridLayout *sliderLayout = new QGridLayout {};
    QHBoxLayout *controlLayout = new QHBoxLayout {};
    QHBoxLayout *iterationLayout = new QHBoxLayout {};
    QHBoxLayout *regLayout = new QHBoxLayout {};
    QHBoxLayout *noiseLayout = new QHBoxLayout {};

    updateStatus();
    controlLayout->addWidget(_statusView);

    _axisSlider->setRange(1, 3);
    _axisSlider->setOrientation(Qt::Horizontal);
    _axisSlider->setValue(3); // Z is default direction
    _axisSlider->setSingleStep(1);
    QLabel *label1 = new QLabel("X", this);
    QLabel *label2 = new QLabel("Y", this);
    QLabel *label3 = new QLabel("Z", this);
    sliderLayout->addWidget(_axisSlider, 0, 0, 1, 2);

    sliderLayout->addWidget(label1, 1, 0, 1, 1);
    sliderLayout->addWidget(label2, 1, 1, 1, 1);
    sliderLayout->addWidget(label3, 1, 2, 1, 1);
    controlLayout->addItem(sliderLayout);

    layout->setRowStretch(0, 3);
    layout->setRowStretch(1, 1);

    layout->addWidget(_sWidget);
    layout->addItem(controlLayout, 1, 0);
    layout->addWidget(_loadButton, 2, 0);

    iterationLayout->addWidget(_iterationLabel);
    _iterationSlider->setRange(1, 100);
    _iterationSlider->setOrientation(Qt::Horizontal);
    _iterationSlider->setValue(40);
    iterationLayout->addWidget(_iterationSlider);

    layout->addItem(iterationLayout, 3, 0);

    regLayout->addWidget(_regCheckBox);
    updateRegText();
    _regLambda->setButtonSymbols(QAbstractSpinBox::NoButtons);
    regLayout->addWidget(_regLambda);
    _regLambda->setRange(0, 5000);
    _regLambda->setPrefix("λ=");
    _regLambda->setValue(10);
    layout->addItem(regLayout, 4, 0);

    _updateButton->setEnabled(false);
    layout->addWidget(_updateButton, 5, 0);
    noiseLayout->addWidget(_noiseCheckBox);
    noiseLayout->addWidget(_noiseSpinBox);
    layout->addItem(noiseLayout, 6, 0);
        
    layout->addWidget(_updateNoiseButton, 7, 0);

    setLayout(layout);

    connect(_sWidget, &SliceWidget::sliceChanged, this, &ReconstructionViewer::updateStatus);
    connect(_axisSlider, &QSlider::valueChanged, _sWidget, &SliceWidget::updateStatus);
    connect(_loadButton, &QPushButton::pressed, this, &ReconstructionViewer::requestAcquisition);

    connect(_iterationSlider, &QSlider::valueChanged, this, &ReconstructionViewer::updateStatus);
    connect(_regCheckBox, &QCheckBox::stateChanged, this, &ReconstructionViewer::updateRegText);

    connect(_iterationSlider, &QSlider::valueChanged, this, &ReconstructionViewer::enableUpdate);
    connect(_regCheckBox, &QCheckBox::stateChanged, this, &ReconstructionViewer::enableUpdate);
    connect(_regLambda, &QDoubleSpinBox::editingFinished, this, &ReconstructionViewer::enableUpdate);
    connect(_noiseSpinBox, &QDoubleSpinBox::editingFinished, this,
                &ReconstructionViewer::enableUpdate);
    connect(_noiseCheckBox, &QCheckBox::stateChanged, this,
                &ReconstructionViewer::enableUpdate);
    connect(_noiseCheckBox, &QCheckBox::stateChanged, this, &ReconstructionViewer::updateSpinBox);

    connect(_updateButton, &QPushButton::pressed, this, &ReconstructionViewer::updateReconstructionParams);

    connect(_updateNoiseButton, &QPushButton::pressed, this, &ReconstructionViewer::updateNoise);
    _noiseSpinBox->setMaximum(1000.);
    _noiseSpinBox->setMinimum(0.);
    _noiseSpinBox->setDecimals(2);
    _noiseSpinBox->setSuffix("%");
    _noiseSpinBox->setValue(4);
    _noiseSpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    _noiseSpinBox->setKeyboardTracking(false);
        
    updateRegText();
    updateSpinBox();
    updateStatus();
    
}
