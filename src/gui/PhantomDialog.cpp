#include "PhantomDialog.h"

PhantomDialog::PhantomDialog(QWidget* parent)
    :   _sideLength(10), _phantomName("cube"),
        _lengthSelector{new QSpinBox{}},
        _listWidget{new QListWidget{}},
        _loadButton{new QPushButton{"Load Phantom"}}
{
    _names.push_back("cube");
    _names.push_back("sticks");
    _names.push_back("box");

    // intro
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(new QLabel("Phantom selection"));
    // side length
    QHBoxLayout *lenLayout = new QHBoxLayout;
    lenLayout->addWidget(new QLabel("Side length (in voxels, isotropic)"));
    _lengthSelector->setValue(10);
    _lengthSelector->setSingleStep(1);
    _lengthSelector->setRange(3, 100);
    lenLayout->addWidget(_lengthSelector);
    layout->addLayout(lenLayout);
    // phantom name
    QHBoxLayout *nameLayout = new QHBoxLayout;
    nameLayout->addWidget(new QLabel("Select phantom name"));

    for(const auto& name : _names) {
        QListWidgetItem *newItem = new QListWidgetItem;
        newItem->setText(name);
        _listWidget->insertItem(0, newItem);
    }

    nameLayout->addWidget(_listWidget);
    layout->addLayout(nameLayout);

    // button to trigger loading
    layout->addWidget(_loadButton);
    // use main layout
    setLayout(layout);

    // set signals & slots
    connect(_loadButton, &QPushButton::pressed, this,
        &PhantomDialog::close);
    connect(_lengthSelector, QOverload<int>::of(&QSpinBox::valueChanged), 
        [=](int i){
            _sideLength = i;
        });
    connect(_listWidget, &QListWidget::itemSelectionChanged, [=]() {
        _phantomName = _names[_listWidget->count() - 1 - _listWidget->currentRow()];
    });
}

int PhantomDialog::getSideLength() {
    return _sideLength;
}

std::string PhantomDialog::getPhantomName() {
    return _phantomName;
}