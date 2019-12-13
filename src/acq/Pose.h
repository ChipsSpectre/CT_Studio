#ifndef POSE_H
#define POSE_H

#include <Eigen/Dense>

/*
 * represents a point that can be rotated around a center with respect to two different axis
 */

class Pose
{
private:
    Eigen::Vector3f _normalRef;

    Eigen::Vector3f _center;
    Eigen::Vector3f _normal;
    float           _distance;
    Eigen::Vector3f _point;
    
    Eigen::AngleAxisf _rotZ;
    Eigen::AngleAxisf _rotY;
    Eigen::AngleAxisf _rot;

        
public:
    Pose();
    Pose(Eigen::Vector3f point, Eigen::Vector3f center = Eigen::Vector3f(0, 0, 0));
    Pose(Eigen::Vector3f normal, float direction, Eigen::Vector3f center = Eigen::Vector3f(0, 0, 0));
    
    float getRotationZ() const;
    float getRotationY() const;
    Eigen::Matrix3f getRotation() const;
    
    void setRotation(float z, float y);
    void setRotationZ(float z);
    void setRotationY(float y);
    
    void addRotationZ(float rotZ);
    void addRotationY(float rotY);
    
    Eigen::Vector3f getCenter() const;
    void setCenter(Eigen::Vector3f center);
    

protected:
    Eigen::Vector3f getPoint() const;
    //void setPointRef(Eigen::Vector3f point);
    Eigen::Vector3f getNormal() const;
    
    void setDistanceToCenter(float distance);


    virtual void updatePose();  
};

#endif