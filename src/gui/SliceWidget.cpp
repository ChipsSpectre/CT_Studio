#include "SliceWidget.h"

#include <QMessageBox>

void SliceWidget::paintEvent(QPaintEvent*) {
    if(!_model){
        return;
    }
    QPainter painter(this);
    if(_model->invalid()){
        painter.fillRect(this->rect(), QBrush(QColor(Qt::black)));
        return;
    }

    const Vec3D<float>& content = _model->getContent();
    const Eigen::Vector3f& spacing = _model->getVolumeBase().getSpacing();
    if(content.totalSize() == 0){
        return;
    }

    float maxColor = _model->rec().maxEntry();
    float colorCoeff = 255.0/maxColor;
    if(_status == 2) {   
        const float spacingRatio = spacing.x()/spacing.y();
        int maxPixelWidth = 1.0*width()/(content.sizeX())/spacingRatio;
        int maxPixelHeight = 1.0*height()/(content.sizeY());
           
        int pixelSize = std::min(maxPixelWidth, maxPixelHeight);
            
        int pixelWidth = pixelSize * spacingRatio;
        int pixelHeight = pixelSize;
        
        for(int i = 0; i< content.sizeX(); ++i) {
            for(int j = 0; j<content.sizeY(); ++j) {
                int curr = std::abs(content.get(i,j,_currSlice)) * colorCoeff;
                QColor color = QColor::fromRgb(curr, curr, curr);
                painter.fillRect(QRect(i*pixelWidth, j*pixelHeight, pixelWidth, pixelHeight), QBrush(color));
            }
        }
    } else if(_status == 1) {
        const float spacingRatio = spacing.x()/spacing.z();
        int maxPixelWidth = 1.0*width()/(content.sizeX())/spacingRatio;
        int maxPixelHeight = 1.0*height()/(content.sizeZ());
            
        int pixelSize = std::min(maxPixelWidth, maxPixelHeight);
            
        int pixelWidth = pixelSize * spacingRatio;
        int pixelHeight = pixelSize;
            
        for(int i = 0; i< content.sizeX(); ++i) {
            for(int j = 0; j<content.sizeZ(); ++j) {
                int curr = std::abs(content.get(i,_currSlice,j)) * colorCoeff;

                QColor color = QColor::fromRgb(curr, curr, curr);
                painter.fillRect(QRect(i*pixelWidth, j*pixelHeight, pixelWidth, pixelHeight), QBrush(color));
            }
        }
    } else {
        const float spacingRatio = spacing.y()/spacing.z();
        int maxPixelWidth = 1.0*width()/(content.sizeY())/spacingRatio;
        int maxPixelHeight = 1.0*height()/(content.sizeZ());
            
        int pixelSize = std::min(maxPixelWidth, maxPixelHeight);
            
        int pixelWidth = pixelSize * spacingRatio;
        int pixelHeight = pixelSize;
            
        for(int i = 0; i< content.sizeY(); ++i) {
            for(int j = 0; j<content.sizeZ(); ++j) {
                int curr = std::abs(content.get(_currSlice,i,j)) * colorCoeff;

                QColor color = QColor::fromRgb(curr, curr, curr);
                painter.fillRect(QRect(i*pixelWidth, j*pixelHeight, pixelWidth, pixelHeight), QBrush(color));
            }
        }
    }

}

void SliceWidget::wheelEvent(QWheelEvent* event) {
    if(event->delta() == 0) {
        // no real motion! do nothing!
        return;
    }
    if(event->delta() < 0) {
        // go back
        if(_currSlice > 0) {
            _currSlice--;
            emit sliceChanged();
            update();
        }
    }
    else {
        // go forward
        if(_currSlice < slices()-1) {
            _currSlice++;
            emit sliceChanged();
            update();
        }
    }
}

bool SliceWidget::checkModelInvalidity() {
    if(_model->invalid()){
        QMessageBox::warning(this, "Invalid Reconstruction", "There was a problem in the reconstruction. This can happen when there is not enough information to restore the volume (and there are too many cg-iterations). It can also happen when lambda is too big.");
        return true;
    }
    return false;
}
