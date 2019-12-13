#include "ResultWidget.h"
#include <algorithm>

#include <iostream>

void ResultWidget::recalcProject() {
    switch(_widgetStack->currentIndex()){
        case 0:
            paint2DVec(_model.getLastProj());
            break;
        case 1:
            paint2DVec(_model.getProj());
            break;
        default:
            paint2DVec(_model.getLastProj());
            break;
    }
}

ResultWidget::ResultWidget(AcquisitionModel& model, QWidget* parent)  
    : QWidget {parent}, _widgetStack{new QTabWidget{}}, 
    _drawWidgetSingle{new QWidget{}}, _drawWidgetAll{new QWidget{}}, 
    _state {DrawState::Single}, _model {model}, 
    _image {} {
    
    QVBoxLayout *mainLayout = new QVBoxLayout {};;
    recalcProject();
        
    /*
     * Set the Background of the drawWidgets to black
    */
    QPalette pal = palette();
    pal.setColor(QPalette::Background, Qt::black);
    _drawWidgetSingle->setPalette(pal);
    _drawWidgetSingle->setAutoFillBackground(true);
    _drawWidgetAll->setPalette(pal);
    _drawWidgetAll->setAutoFillBackground(true);
    
    //see eventFilter
    _drawWidgetSingle->installEventFilter(this);
    _drawWidgetAll->installEventFilter(this);
        
    _widgetStack->addTab(_drawWidgetSingle, "Latest Projection");
    _widgetStack->addTab(_drawWidgetAll, "All Projections");
    _widgetStack->setCurrentIndex(0);
    mainLayout->addWidget(_widgetStack);
    QObject::connect(_widgetStack, &QTabWidget::currentChanged, this,
                     &ResultWidget::tabChanged);
    this->setLayout(mainLayout);
}

bool ResultWidget::eventFilter(QObject* watched, QEvent* event) {
    if(event->type() == QEvent::Paint && ( watched == _drawWidgetSingle ||
        watched == _drawWidgetAll)){
        QWidget& widget = currWidget();
        QSize size = widget.size();
        
        //TODO if the image is not a square it shouldn't be painted as a square
        int drawSize = std::min(widget.width(), widget.height());
        
        QRect drawRect{0, 0, drawSize, drawSize}; 
        
        QPainter painter {&widget};
        painter.drawImage(drawRect, _image);

        return true;
    } else {
        return QObject::eventFilter(watched, event);
    }
}

QWidget& ResultWidget::currWidget() {
    switch(_widgetStack->currentIndex()){
        case 0:
            return *_drawWidgetSingle;
        case 1:
            return *_drawWidgetAll;
        default:
            return *_drawWidgetSingle;
       }
}

float ResultWidget::maxPixel(const Eigen::VectorXf& projection) {
    float max = std::numeric_limits<float>::min();
    for(int i = 0; i < projection.size(); ++i){
        max = projection[i] > max ? projection[i] : max;
    }
    return max;
}
