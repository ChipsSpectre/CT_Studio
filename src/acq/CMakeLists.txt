cmake_minimum_required(VERSION 3.5.1)

# specify acquisition source files
set(ACQ_SRCS
        AcquisitionModel.cpp
        Pose.cpp
        AcquisitionPose.cpp
        RayTracing.cpp
        )
set(ACQ_HDRS
        Volume.h
        AcquisitionModel.h
        Pose.h
        AcquisitionPose.h
        RayTracing.h
        Vec3D.h
        definitions.h
        PhantomGenerator.h
        )

add_library(part2_lib_acq ${ACQ_HDRS} ${ACQ_SRCS} )
target_link_libraries(part2_lib_acq Qt5::Widgets)
