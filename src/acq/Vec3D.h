#ifndef VEC3D_H
#define VEC3D_H

#include <algorithm>
#include <stdexcept>
#include <vector>

#include <Eigen/Dense>

template <class T>
class Vec3D
{
    using index_type = int;
    using vector_type = Eigen::VectorXf;
    using size_vector_type = Eigen::Matrix<index_type, 3, 1>;
    using content_type = T;
    using typeRef = T&;
private:
    size_vector_type _sizes;
    
    vector_type _content;
        
    void checkBounds(index_type x, index_type y, index_type z) const {
        if(!(x >= 0 && x < sizeX() && y >= 0 && y < sizeY() && z >= 0 && z < sizeZ())){
            std::string exText{"Out of range in 3DImVec; max allowed sizes are: "};
            exText += " x: ";
            exText += std::to_string(sizeX());
            exText += " y: ";
            exText += std::to_string(sizeY());
            exText += " z: ";
            exText += std::to_string(sizeZ());
            
            exText += " but the given indices (up to size-1) were: ";
            exText += " x: ";
            exText += std::to_string(x);
            exText += " y: ";
            exText += std::to_string(y);
            exText += " z: ";
            exText += std::to_string(z);
        
            throw std::out_of_range {exText}; 
        }
    }
    
    void checkInvariant(int vecSize) const{
        if(vecSize != (sizeX()) * (sizeY()) * (sizeZ())){
            std::string exText {"The vector given as argument of the Vec3D constructor has size "};
            exText += std::to_string(vecSize);
            exText += " but the given size parameters indicate a size of ";
            exText += std::to_string((sizeX()) * (sizeY()) * (sizeZ()));

            throw std::length_error {exText};
        }
    }
    
public:
    
    Vec3D () : _sizes{size_vector_type::Zero()}, _content {} {    }

    Vec3D (index_type sizeX, index_type sizeY, index_type sizeZ, const vector_type& vec)
    : Vec3D {size_vector_type {sizeX, sizeY, sizeZ}, vec} {
    }

    Vec3D (index_type sizeX, index_type sizeY, index_type sizeZ, vector_type&& vec)
    : Vec3D {size_vector_type {sizeX, sizeY, sizeZ}, std::move(vec)} {        
    }
    
    Vec3D (Eigen::Vector3i sizeDims, const vector_type& vec)
    : _sizes {sizeDims}, _content {vec} {
        checkInvariant(vec.size());
    }
    
    Vec3D (Eigen::Vector3i sizeDims, vector_type&& vec)
    : _sizes {sizeDims}, _content {} {
        checkInvariant(vec.size());
        //move only when the invariant was already checked
        _content = std::move(vec);
    }
    
    index_type sizeX() const noexcept{
        return _sizes.x();
    }
    
    index_type sizeY() const noexcept{
        return _sizes.y();
    }
    
    index_type sizeZ() const noexcept{
        return _sizes.z();
    }
    
    int totalSize() const noexcept{
        return _content.size();
    }
    
    size_vector_type sizes() const noexcept{
        return _sizes;
    }
    
    template <class Vec>
    T& get(Vec vec) noexcept{
        return get(vec.x(), vec.y(), vec.z());
    }
    
    template <class Vec>
    const T& get(Vec vec) const noexcept{
        return get(vec.x(), vec.y(), vec.z());
    }
    
    template <class Vec>
    T& getChecked(Vec vec){
        return getChecked(vec.x(), vec.y(), vec.z());
    }
    
    template <class Vec>
    const T& getChecked(Vec vec) const{
        return getChecked(vec.x(), vec.y(), vec.z());
    }
    
    T& get(int x, int y, int z) noexcept{
        return _content[x + y * sizeX() + z * sizeX() * sizeY()];
    }
    
    const T& get(int x, int y, int z) const noexcept{
        return _content[x + y * sizeX() + z * sizeX() * sizeY()];
    }
    
    T& getChecked(index_type x, index_type y, index_type z){
        checkBounds(x,y,z);
        return get(x, y, z);
    }
    
    const T& getChecked(index_type x, index_type y, index_type z) const {
        checkBounds(x,y,z);
        return get(x, y, z);
    }
    
    vector_type& rawVec() noexcept{
        return _content; 
    }
    
    const vector_type& rawVec() const noexcept{
        return _content; 
    }
    
    content_type * rawData() noexcept{
        return _content.data(); 
    }
    
    content_type const * rawData() const noexcept{
        return _content.data(); 
    }

    /**
     * Convert the input coordinate into a numeric value.
     * @brief getIndex
     * @param input
     * @return
     */
    int getIndex(Eigen::Vector3i input) const
    {
        return input[0] + input[1] * sizeX() + input[2] * sizeX() * sizeY();
    }
    
    float maxEntry() {
        float max = std::numeric_limits<float>::min();
        for(int i = 0; i < totalSize(); ++i) {
            max = std::abs(_content[i]) > std::abs(max) ? std::abs(_content[i]) : max;
        }
        return max;
    }
    
    /**
     * Checks if the Vector contains at least one NaN
     **/
    bool containsNan(){
        //we assume that this normally doesn't happen
        bool ret = false;
        for(int i = 0; i < totalSize(); ++i){
            //if one nan is found ret is true and stays so until the return
            ret |= std::isnan(_content[i]);
        }
        return ret;
    }
};

#endif // VEC3D_H
