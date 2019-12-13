cmake_minimum_required(VERSION 3.5.1)

# specifiy reconstruction source files
set(REC_SRCS
        CG.cpp
        DataContainer.cpp
        NormalReconstructionContainer.cpp
        ReconstructionModel.cpp
        RegularizationContainer.cpp
        )
set(REC_HDRS
        CG.h
        DataContainer.h
        NormalReconstructionContainer.h
        ReconstructionModel.h
        RegularizationContainer.h
        )

add_library(part2_lib_rec ${REC_HDRS} ${REC_SRCS} )
target_link_libraries(part2_lib_rec Qt5::Widgets)
