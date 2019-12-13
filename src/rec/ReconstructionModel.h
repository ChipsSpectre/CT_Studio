#pragma once

#include <vector>
#include <memory>
#include <chrono>
#include <random>

#include <Eigen/Eigen>

#include "Volume.h"
#include "RayTracing.h"
#include "CG.h"
#include "RegularizationContainer.h"
#include "NormalReconstructionContainer.h"
#include "Acquisition.h"

/**
 * Model class for reconstruction.
 * @brief The ReconstructionModel class
 */
class ReconstructionModel
{
private:
    /// the following two parameters can be only set in the constructor
    std::vector<AcquisitionPose> _poses;
    /// For the error use a second _mesurements vector or overwrite the pointer, if you can
    /// get it back. Don't remove the const!
    Eigen::VectorXf _measurements;
    
    std::unique_ptr<DataContainer> _cont;
    /// indicates whether reconstruction is noisy
    bool _noisy;
    /// To be able to easily remove the errors from the measurements they are stored extra
    Eigen::VectorXf _noise;

    /// the result of the _reconstruction
    std::shared_ptr<Volume> _reconstruction;
    
    /**
     * The reconstruction can be invalid if there is not enough information to restore there
     * volume or if lambda in the regularized reconstruction was too big
     * 
     * The result is that the reconstruction contains at least one NaN
     */
    bool _invalid;
    

public:
    
    std::unique_ptr<DataContainer> generateContainer(const VolumeBase& base, bool regularized, float lambda) {
        if(regularized){
            return std::make_unique<RegularizationContainer>(base, _poses, lambda);
        } else {
            return std::make_unique<NormalReconstructionContainer>(base, _poses);
        }
    }
    
    /**
     * Returns the measurements and adds noise to them if the
     * reconstruction should be done on noisy data.
     * @brief getMeasurements
     * @return Volume containing the (noisy) measurements.
     */
    Eigen::VectorXf getMeasurements(){
        return _noisy ? _measurements + _noise : _measurements;
    }
    
    /**
     * Recalculates the reconstruction using CG algorithm.
     * The results can be retrieved via rec() method.
     * @brief recalcReconstruction
     * @param iterations - number of iterations for CG
     */
    void recalcReconstruction(int iterations){
        _reconstruction = std::make_shared<Volume>(*_reconstruction, CG::conjugateGradient(iterations, *_cont, getMeasurements()));
        _invalid = _reconstruction->containsNan();
    }

    const VolumeBase& getVolumeBase(){
        return *_reconstruction;
    }

    /**
     * Creates a vector containing gaussian noise.
     * The average of the noise is zero.
     * @brief generateNoise
     * @param noise - intensity of noise
     * @return vector containing noise only
     */
    Eigen::VectorXf generateNoise(double noise){
        if (noise == 0.0) {
            return Eigen::VectorXf::Zero(_measurements.size());
        } else {
            static const unsigned int seed = static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()
                        % std::numeric_limits<unsigned int>::max());
            static std::mt19937 gen {seed};
            static std::normal_distribution<float> dist {};
            
            Eigen::VectorXf noiseVec {_measurements.size()};
            const float minMaxDiff = _measurements.maxCoeff() - _measurements.minCoeff();
            for(int i = 0; i < noiseVec.size(); ++i){
                noiseVec[i] = noise * minMaxDiff * dist(gen);
            }
            return noiseVec;
        }
        
    }
    
    bool invalid(){
        return _invalid;
    }
    
public:
    /**
     * Creates a new reconstruction model.
     * @brief ReconstructionModel
     * @param regularized - true indicates the use of regularized reconstruction
     * @param lambda - determines the strength of regularization
     * @param noisy - true indicates a reconstruction on noisy data
     * @param noise - intensity of noise
     * @param cgIterations - number of iterations to be used for CG algorithm initially
     * @param acq - details about the acquisition stored in a Acquisition struct.
     */
    ReconstructionModel (bool regularized, float lambda, bool noisy, float noise, int cgIterations, Acquisition&& acq)
    :
                _poses {std::move(acq.poses)},
                _measurements {std::move(acq.measurements)},
                _cont {generateContainer(acq.volBase, regularized, lambda)},
                _noisy {noisy}, _noise {generateNoise(noise)},
                _reconstruction {std::make_shared<Volume>(acq.volBase, CG::conjugateGradient(cgIterations, *_cont, getMeasurements()))}, _invalid {_reconstruction->containsNan()}
    {
    }
    
    /**
     * Get shared pointer to reconstruction result.
     * @brief getRec
     * @return
     */
    std::shared_ptr<const Volume> getRec() const{
        return _reconstruction;
    }    
    
    /**
     * Get const reference to reconstruction result.
     * @brief rec
     * @return
     */
    const Volume& rec(){
        return *_reconstruction;
    }
    
public slots:
    
    /**
     * This is used when only one of the parameters change.
     * It starts a recomputation of the reconstruction.
     */
    void changeReconstructionParams(bool regularized, float lambda, int iterations)
    {
        _cont = generateContainer(*_reconstruction, regularized, lambda);
        recalcReconstruction(iterations);
    }
    
    void changeNoise(bool noisy, float noise, int iterations){
        _noisy = noisy;
        if(_noisy){
            _noise = generateNoise(noise);
        }
        recalcReconstruction(iterations);
    }
    
    /**
     * Gets reconstruction volume's content.
     * @brief getContent
     * @return reconstruction volume's content
     */
    const Vec3D<float>& getContent() {
        return _reconstruction->content();
    }
};
