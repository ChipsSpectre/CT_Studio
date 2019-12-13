cmake_minimum_required(VERSION 3.5.1)

# specifiy visualization source files
set(VIS_SRCS
        CameraPose.cpp
        DVRCamera.cpp
        DVRModel.cpp
        LinearPiece.cpp
        MIP.cpp
        MPRModel.cpp
        TransferFunction.cpp
        VisualizationModel.cpp
        )
set(VIS_HDRS
        CameraPose.h
        DVRCamera.h
        DVRModel.h
        LinearPiece.h
        MIP.h
        MPRModel.h
        TransferFunction.h
        VisualizationModel.h
        )

add_library(part2_lib_vis ${VIS_HDRS} ${VIS_SRCS} )
target_link_libraries(part2_lib_vis Qt5::Widgets)
