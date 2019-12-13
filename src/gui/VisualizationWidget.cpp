#include "VisualizationWidget.h"

//void VisualizationWidget::mprUpdateColorLabel()
//{
//    _mprColorLabel->setStyleSheet( "border: 1px solid black; background-color: " + _mprWidget->color().name() );
//}

//void VisualizationWidget::dvrUpdateColorLabel()
//{
//    _dvrColorLabel->setStyleSheet( "border: 1px solid black; background-color: " + _dvrWidget->color().name() );
//}

void VisualizationWidget::updateVolumeInfo()
{
    Eigen::Vector3i size = _visModel.volume().getNumVoxels();
    int x = size[0], y = size[1], z = size[2];
    _volumeInfoLabel->setText("Volume size: ("+QString::number(x)+","+QString::number(y)+","+QString::number(z)+")");
}

void VisualizationWidget::updateVolumeChanged()
{
    updateDVRWidget();
    updateVolumeInfo();
}

void VisualizationWidget::updateDVRWidget()
{
    _dvrWidget->setColorRange(0, _visModel.volume().maxEntry());
}

VisualizationWidget::VisualizationWidget() :
    _visModel {},
    _loadFileButton {new QPushButton{"Load from file"}},
    _loadRecButton {new QPushButton{"Load reconstruction"}},
    _mprColorLabel {new QLabel {}},
    _mprSelectColorButton {new QPushButton{"Select color for MPR"}},
    _mprColorResetButton {new QPushButton{"Default Color"}},
    _dvrSelectColorButton {new QPushButton{"Select color for DVR"}},
    _dvrColorResetButton {new QPushButton{"Default Color"}},
    _volumeInfoLabel {new QLabel{}},
    _dvrColorLabel {new QLabel{}},
    _mprWidget {new MPRWidget{_visModel}},
    _dvrWidget {new DVRWidget{_visModel}},
    _axisWidget {new AxisWidget {_visModel.getMPRModel()}},
    _dvrControlWidget {new DVRControlWidget(_visModel.getDVRModel())},
    _mprControlWidget {new MPRControlWidget(_visModel.getMPRModel())}
{
    ///all layout items for visulisation are composed in the main layout
    QHBoxLayout *mainLayout = new QHBoxLayout {};
    ///manages all menu items
    QGridLayout *menuLayout = new QGridLayout {};
    ///manages the color-choosing related widgets for mpr
    QHBoxLayout *mprColorLayout = new QHBoxLayout {};
    ///manages the color-choosing related widgets for dvr
    QHBoxLayout *dvrColorLayout = new QHBoxLayout {};

    mprUpdateColorLabel();
    mprColorLayout->addWidget(_mprColorLabel);
    mprColorLayout->addWidget(_mprSelectColorButton);
    mprColorLayout->addWidget(_mprColorResetButton);

    dvrUpdateColorLabel();
    dvrColorLayout->addWidget(_dvrColorLabel);
    dvrColorLayout->addWidget(_dvrSelectColorButton);
    dvrColorLayout->addWidget(_dvrColorResetButton);

    menuLayout->addWidget(_volumeInfoLabel);
    menuLayout->addItem(mprColorLayout, 2, 0);
    menuLayout->addItem(dvrColorLayout, 4, 0);

    menuLayout->addWidget(_loadFileButton, 0, 0);
    menuLayout->addWidget(_loadRecButton, 1, 0);

    menuLayout->addWidget(_mprControlWidget, 3, 0);
    menuLayout->addWidget(_dvrControlWidget, 5, 0);
    
    menuLayout->addWidget(_loadFileButton, 0, 0);
    menuLayout->addWidget(_loadRecButton, 0, 1);
    
    mainLayout->addItem(menuLayout);
    QVBoxLayout *mprLayout = new QVBoxLayout {};
    mprLayout->addWidget(_mprWidget);
    mprLayout->addWidget(_axisWidget);
    mprLayout->setStretch(0, 5);
    mprLayout->setStretch(1, 1);
    
    mainLayout->addItem(mprLayout);
    //mainLayout->addWidget(_axisWidget);
    mainLayout->addWidget(_dvrWidget);
    
    mainLayout->setStretch(0, 3);
    mainLayout->setStretch(1, 5);
    mainLayout->setStretch(2, 5);

    setLayout(mainLayout);

    connect(_loadFileButton, &QPushButton::pressed, this, &VisualizationWidget::loadFromFile);
    connect(_loadRecButton, &QPushButton::pressed, this, &VisualizationWidget::requestRecVolume);
    //connect(_mprSelectColorButton, &QPushButton::pressed, this, &VisualizationWidget::mprChangeColor);
    connect(_mprColorResetButton, &QPushButton::pressed, this, &VisualizationWidget::mprResetColor);
    //connect(_dvrSelectColorButton, &QPushButton::pressed, this, &VisualizationWidget::dvrChangeColor);
    connect(_dvrColorResetButton, &QPushButton::pressed, this, &VisualizationWidget::dvrResetColor);
}

void VisualizationWidget::setRec(const std::shared_ptr<const Volume>& vol){
    if(!vol){
        QMessageBox::warning(nullptr, "No Reconstruction Present or Invalid Reconstruction!", "You have to generate a valid reconstruction before it can be visualized!");
        return;
    }
    _visModel.setVolume(*vol);
    updateVolumeChanged();
}


void VisualizationWidget::loadFromFile(){
    // TODO: Support loading from file
}
