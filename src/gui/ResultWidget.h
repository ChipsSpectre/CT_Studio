#ifndef RESULTWIDGET_H
#define RESULTWIDGET_H

#include <limits>
#include <algorithm>

#include <QWidget>
#include <QPainter>
#include <QBrush>
#include <QTabWidget>
#include <QObject>
#include <QVBoxLayout>
#include <QEvent>
#include <QImage>
#include <QVector>

#include "AcquisitionModel.h"

enum class DrawState{
    Single = 0,
    All = 1,
    Angle = 2
};

/*
 * Draws the forward Projection of the current aquisition pose
 */
class ResultWidget : public QWidget
{
    Q_OBJECT    
private:    
    /*
     * returns the widget that is currently shown
     * used to see which widget has to be updated
     */
    
    QTabWidget *_widgetStack;
    QWidget *_drawWidgetSingle;
    QWidget *_drawWidgetAll;
    DrawState _state;
    AcquisitionModel &_model;
    /*
     * stores the current visualisation of the x-ray simulation.
     * 
     * Currently always uses the 8 bit format
     */
    QImage _image;
    
    QWidget& currWidget();
    
    /*
     * generates a vector of length 256 that contains all grey colors that are possible
     * in the qt _image
     * 
     * This is given to stored image which is in the 8bit format
     */
    static QVector<QRgb> generateBlackWhiteColorTable(){
        QVector<QRgb> vec {};
        for(int i = 0; i < 256; ++i){
            vec.push_back(qRgb(i, i, i));
        }
        return vec;
    }
    
    /*
     * paints a two dimensional vector defined by its number of rows (first pair attribute)
     * and its content (second pair attribute)
     */
    void paint2DVec(std::pair<int, const Eigen::VectorXf> vec2D){
        const Eigen::VectorXf& vec = vec2D.second;
        const int rows = vec2D.first;
        const int cols = vec.size()/rows;
        const float max = maxPixel(vec);
        assert(rows*cols == vec.size());
        
        if(vec.size() == 0){
            //fill the image with one black pixel
            _image = QImage {1,1, QImage::Format_Indexed8};
            _image.setColorTable(generateBlackWhiteColorTable());
            _image.setPixel(0,0,0);
            return;
        }  
        //Only resize the image if its actually needed. Otherwise simply override the old image
        if(cols != _image.width() || rows != _image.height()){
            _image = QImage {cols, rows, QImage::Format_Indexed8};
            _image.setColorTable(generateBlackWhiteColorTable());
        }
        
        for(int y = 0; y < rows; ++y){
            const int yInd = y * cols;
            for(int x = 0; x < cols; ++x){
                //There were some negative values, so to avoid bugs, currValue is at least zero
                float currValue = std::max(vec[yInd + x], 0.0f);
                int colorVal = 255*currValue/max;
                _image.setPixel(x, y, colorVal);
            }
        }
      
        update();
    }
    
    /*
     * Find the maximum pixel value from the forward Projection.
     * This pixel will always be the white pixel.
     */
    float maxPixel(const Eigen::VectorXf& projection);
public:
    ResultWidget(AcquisitionModel& model, QWidget *parent = nullptr);
public slots:
    
    /*
     * When the pose (or the volume) was updated, the forward Projection has to be 
     * recalculated and redrawn
     */
    void recalcProject();
    
protected slots:
    
    /*
     * offers parameter compatibility with the signal of the tabWidget
     */
    void tabChanged(int){
        recalcProject();
    }
    
    void paintEvent(QPaintEvent *event) override{
        //for some reason update wasn't called automatically
        //even though the drawWidgets are children of this widget
        currWidget().update();
    }
    
    /*
     * Does the drawing.
     * paintEvent can't be used, since it it isn't allowed to draw on children, so this
     * is the workaround that avoids creating a subclass
     * To be get the events of the children this has to be registered in the 
     * children as listener (see constructor)
     */
    bool eventFilter(QObject * watched, QEvent * event) override;

};

#endif // RESULTWIDGET_H
