#include "PoseDisplay.h"

void PoseDisplay::paintEvent(QPaintEvent*)
{
	QPainter painter(this);
    QPen pen;

    // xc and yc are the center of the widget's rect.
    float x_center = width() * 0.5;
    float y_center = height() * 0.5;

    painter.setPen(Qt::black);

    // draw the cross lines.
    painter.drawLine(x_center, rect().top(), x_center, rect().bottom());
    painter.drawLine(rect().left(), y_center, rect().right(), y_center);

    for(unsigned int i = 0; i < _model.getPoses()->size(); i++)
    {
        if(i == _model.getPoses()->size() - 1)
        { paintPose(painter, (*_model.getPoses())[i], false, false);}
        else
        { paintPose(painter, (*_model.getPoses())[i], true, true); }
    }
    
    //paint the bounding box of the main volume
    auto boundingBox = _model.getBoundingBox();
    auto topLeftCorner = boundingBox.corner(Eigen::AlignedBox3f::TopLeftFloor);
    auto lowerRightCorner = boundingBox.corner(Eigen::AlignedBox3f::BottomRightFloor);
    
    auto minCorner = boundingBox.min();
    auto maxCorner = boundingBox.max();
    
    
    pen.setColor(Qt::black);
    painter.setPen(pen);
    painter.resetTransform();
    painter.translate(x_center, y_center);
    painter.drawRect( minCorner(_xAxis) *_zoom, minCorner(_yAxis) *_zoom,
                      (maxCorner(_xAxis) - minCorner(_xAxis)) *_zoom,
                      (maxCorner(_yAxis) - minCorner(_yAxis)) *_zoom);
                     
    
    /*
    //painter.setWidth(2)
    painter.translate(
        x_center + pose_->getDetectorCenter()(0) * zoom_, 
        y_center - pose_->getDetectorCenter()(1) * zoom_
       );
    painter.rotate(-pose_->getRotation() / M_PI * 180);

    // we need to move the rectangle that we draw by rx and ry so it's in the center.
    float rx = 0;
    float ry = -pose_->getDetectorWidth() * zoom_ * 0.5;
    painter.drawRect(QRect(rx, ry, 4, pose_->getDetectorWidth() * zoom_));
    */
}

void PoseDisplay::paintPose(QPainter& painter, AcquisitionPose& pose, bool lowOpacity = false, bool onlySources = false)
{
    float x_center = width() * 0.5;
    float y_center = height() * 0.5;
    
    QPen pen;
    const Eigen::Vector2i detectorSize = _model.getDetectorSize();
    const Eigen::Vector3f sourcePos = pose.getSourcePosition();
    
    if(!onlySources)
    {
        painter.resetTransform();
        
        pen.setColor(QColor(196, 196, 196));
        painter.setPen(pen);
        if(_showRays)
        {
            for(int i = 0; i < detectorSize.x(); i++)
            {
                for(int j = 0; j < detectorSize.y(); j++)
                {
                    Eigen::Vector3f pixel = pose.getPixelCenter(i, j);
                    painter.drawLine(
                        x_center + sourcePos(_xAxis) * _zoom,
                        y_center - sourcePos(_yAxis) * _zoom,
                        x_center + pixel(_xAxis) * _zoom,
                        y_center - pixel(_yAxis) * _zoom);
                }
            }
        }

        if(lowOpacity)
        { pen.setColor(QColor(0, 0, 0, 32)); }
        else
        { pen.setColor(Qt::black); }
        painter.setPen(pen);
        
        
        // rays on corners are always drawn
        for(int x = 0, i = 0; i < 2; i++)
        {
            for(int y = 0, j = 0; j < 2; j++)
            {
                Eigen::Vector3f pixel = pose.getPixelCenter(x, y);

                    painter.drawLine(
                        x_center + sourcePos(_xAxis) * _zoom,
                        y_center - sourcePos(_yAxis) * _zoom,
                        x_center + pixel(_xAxis) * _zoom,
                        y_center - pixel(_yAxis) * _zoom);
                y = detectorSize.y() - 1;
            }
            x = detectorSize.x() - 1;
        }
        //painter.setBrush(Qt::white);

        //painter.translate(x_center, y_center);
        
        painter.resetTransform();
        if(lowOpacity)
        { pen.setColor(QColor(0, 0, 255, 32));}
        else
        { pen.setColor(Qt::blue); }
        painter.setPen(pen);

        //painter.scale(zoom_, zoom_);
        
        std::array<Eigen::Vector3f, 4> detCorners = pose.getDetectorCorners();

        
        for(int i = 0; i < 4; i++)
        {
            painter.drawLine(
                x_center + detCorners[i](_xAxis) * _zoom,
                y_center - detCorners[i](_yAxis) * _zoom,
                x_center + detCorners[(i + 1) % 4](_xAxis) * _zoom,
                y_center - detCorners[(i + 1) % 4](_yAxis) * _zoom
            );
        }
        
    }

    pen.setColor(Qt::red);
    if(lowOpacity)
    { pen.setColor(QColor(255, 0, 0, 32)); }
    else
    { pen.setColor(Qt::red); }
    pen.setWidth(2);
    
    painter.resetTransform();
    painter.translate(
        x_center + sourcePos(_xAxis) * _zoom,
        y_center - sourcePos(_yAxis) * _zoom
    );
    painter.setPen(pen);
    painter.drawEllipse(-2, -2, 4, 4);
    

}

void PoseDisplay::setShowRays(int state)
{
    if(state == 0)
    {
        _showRays = false;
    }
    else
    {
        _showRays = true;
    }
    update();
}
void PoseDisplay::setZoom(int zoom)
{
    _zoom = zoom * 5;
    update();
}

PoseDisplay::PoseDisplay(AcquisitionModel& model) :
    PoseDisplay(model, 2)
{
    setFocusPolicy(Qt::ClickFocus);
    //grabKeyboard();
}
PoseDisplay::PoseDisplay(AcquisitionModel& model, int axis) :
    _showRays (false), _zoom (500), _axis(axis), _model {model}
{
    if(_axis == 2)
    {
        _xAxis = 0;
        _yAxis = 1;
    }
    else if(_axis == 0)
    {
        _xAxis = 1;
        _yAxis = 2;
    }
    else if(_axis == 1)
    {
        _xAxis = 0;
        _yAxis = 2;
    }
}
/*
void PoseDisplay::mousePressEvent(QMouseEvent* event)
{
    grabKeyboard();
}*/

void PoseDisplay::keyPressEvent(QKeyEvent* event)
{
    if(event->key() == Qt::Key_Left) {
        _model.updateRotation(RotationAxis::Z, 0.05f);
        //emit sceneChanged();
        emit _model.poseChanged();
        //update();
    } else if(event->key() == Qt::Key_Right) {  
        _model.updateRotation(RotationAxis::Z, -0.05f);
        //emit sceneChanged();
        emit _model.poseChanged();
        //update();
    } else if(event->key() == Qt::Key_Up) {
        _model.updateRotation(RotationAxis::Y, -0.05f);
        //emit sceneChanged();
        emit _model.poseChanged();
        //update();
    } else if(event->key() == Qt::Key_Down) {
        _model.updateRotation(RotationAxis::Y, 0.05f);
        //emit sceneChanged();
        emit _model.poseChanged();
        //update();
    }
}
