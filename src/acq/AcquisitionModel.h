#ifndef ACQUISITIONMODEL_H
#define ACQUISITIONMODEL_H

#include <string>
#include <memory>
#include <array>
#include <cmath>
#include <tuple>

#include <QObject>
#include <QFile>
#include <QTextStream>

#include "AcquisitionPose.h"
#include "RayTracing.h"
#include "Volume.h"
#include "Acquisition.h"
#include "PhantomGenerator.h"

/*
 * Represents an axis of rotation.
 * If something is rotated aroundthe z-axis it is rotated in the x-y-plane
 */
enum class RotationAxis : bool {
    Z,
    Y
};

class AcquisitionModel : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief AcquisitionModel. Creates an acquisition model and loads an empty 10x10x10 volume
     */
    AcquisitionModel(std::string phantomName);

    void loadPhantom(int sideLength, std::string phantomName) {
        _volume = PhantomGenerator::createPhantom(sideLength, phantomName);
    }

    /**
     * @brief loadImage. Loads a new EDF image into the acquisition model.
     * @param path - file system path determining location of EDF image.
     * @return returns true if the volume data was correctly read
     */
    bool loadFile(QString path);
    
    /**
     * The next few methods offer an interface to pose that is shown in the the gui.
     * See AcquisitionPose for documentation.
     */
    void updateRotation(RotationAxis axis, float angle);
    /**
     * Returns the corners of the detector in the following order:
     * UpperLeft, LowerLeft, LowerRight, UpperRight
     */
    std::array<Eigen::Vector3f, 4> getDetector() const;
    Eigen::Vector2i getDetectorSize() const;
    Eigen::Vector3f getSourcePosition() const;
    Eigen::Vector3f getPixelCenter(int i, int j) const;
    
    
    /*
     * returns the bouding Box of the volume
     */
    Eigen::AlignedBox3f getBoundingBox() const;
  
    /**
     * @brief writeImage. Writes an EDF iamge into a specified path
     * @param path - file system path determining the destination of the image.
     */
    void writeImage(std::string path) const;
    
    /*
     * Deletes all stored poses
     */
    void clearPoses();
    /*
     * Deletes the current Pose. If its the last one reset it to the default pose.
     */
    void deletePose();
    /*
     * Saves the current pose, so that a new one can be defined by the user
     */
    void savePose();
    
    
    std::shared_ptr<std::vector<AcquisitionPose>> getPoses()
    { return std::make_shared<std::vector<AcquisitionPose>>(_poses); }
    
    
    /**
     * Returns a reference to the volume (containing the raw data).
     * @brief volume
     * @return
     */
    const Volume& volume() const
    { return _volume; }
    

    void addSphericalPoses(int circles, int equatorialCount, float range = 2.f * M_PI);
    
    /*
     * returns a pair of the number of rows (=poses) in the _measurements arrey
     * and the measurements themselves
     * This may not be the safest way to acess the vector for painting it but
     * probably the fastest.
     * 
     * TODO right now the Projection vector is copied unecessarily but a refernce
     * didn't work. Shouldn't be a problem most of the time; can be fixed later
     */
    std::pair<int, const Eigen::VectorXf> getProj() {
        return std::make_pair(_poses.size(), _measurements); 
    }
    /*
     * returns only the last projection
     */
    std::pair<int, const Eigen::VectorXf> getLastProj();
    
    
    const Eigen::VectorXf& getProjection(){
        return _measurements;
    }
    
    /*
     * returns the acquisition information needed for the reconstruction if requested
     */
    Acquisition getAcq() const {
        return Acquisition {_volume, _poses, _measurements};
    }
    
    /*
     * changes the size of the detector and/ or the number of pixels
     * since other functions assume that all poses have the same structure, this
     * requires that old poses are deleted
     * 
     * return true if poses have to be reset or deleted
     */
    void changePoseParams(float sourceDetDistance, float detectorWidth, float detectorHeight,
                          int horizontalPixels, int verticalPixels){
        _posePrototype = AcquisitionPose {sourceDetDistance*_volume.getBoundingBox().diagonal().norm(), 
            detectorWidth, detectorHeight, horizontalPixels, verticalPixels};
        clearPoses();
        emit poseChanged();
    }
    
    /*
     * returns the parameters for a pose constructor in the same order as they are used
     * in the constructor:
     * detector-source-distance, detector width, detector height, #horizontal pixels
     * #vertical pixels
     * 
     * It is important to note that the distance is given relative to the length of the
     * volume distance.
     */
    std::tuple<float, float, float, int, int> currPrototype(){
        return std::make_tuple(_posePrototype.getDetectorSourceDistance()/volume().getBoundingBox().diagonal().norm(),
                                _posePrototype.getDetectorWidth(), _posePrototype.getDetectorHeight(), 
                               _posePrototype.getPixelHorizontal(), _posePrototype.getPixelVertical());
    }
    
    /*
     * return false if and only if there is only a single element in _poses, which was
     * not modified (meaning it equals the stored prototype)
     */
    bool containsModifiedElements(){
        return _poses.size() > 1 || !_poses.front().equals(_posePrototype);
    }
    
signals:  
    //emited when the acquistion pose changes (because of user action)
    void poseChanged();
private:
    void addCircularPoses(int count, float angle, float range = 2.f * M_PI);
    
    /*
     * adds a new default AcquisitionPose
     */
    void addDefaultPose();
    
    /**
     * Should be called when new poses were added
     * Updates the _measurements vector
     */
    void updateProjection();
    
    /*
     * This is somewaht useless currently. I will use it for optimisation later.
     * Just use updateProjection for now instead
     */
    void updateLastProjection();
    
    /*
     * The pose that is shown on the screen and which can be modified, is the last pose
     * inserted into the poses stack.
     * It is called the current Pose.
     */
    AcquisitionPose& currPose();
    
    const AcquisitionPose& currPose() const;
    
    AcquisitionPose& currPoseChecked();
    
    const AcquisitionPose& currPoseChecked() const;
    
    const Eigen::Vector3f FIXED_BOX_SIZE = Eigen::Vector3f(0.15, 0.15, 0.25);
    bool _filled;
    Volume _volume;
    AcquisitionPose _posePrototype;
    /*
     * Is effectively a stack. May later be replaced with std::stack TODO
     * Stores older poses that were created by the user, so that a ForwardProjection on 
     * more than one pose can be calculated.
     * There has to be alway one element in this stack, which is the one shown in the gui.
     */
    std::vector<AcquisitionPose> _poses;
    
    Eigen::VectorXf _measurements;
    

};

#endif // ACQUISITIONMODEL_H
