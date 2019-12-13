cmake_minimum_required(VERSION 3.5.1)

# specify gui source files
set(GUI_SRCS
        MainWindow.cpp
        AcquisitionWidget.cpp
        DVRWidget.cpp
        MPRWidget.cpp
        PoseViewer.cpp
        PoseDisplay.cpp
        ReconstructionViewer.cpp
        ResultWidget.cpp
        SliceWidget.cpp
        VisualizationWidget.cpp
        DVRControlWidget.cpp
        MPRControlWidget.cpp
        BoxSlider.cpp
        TransferFuncWidget.cpp
        AxisWidget.cpp
        )
set(GUI_HDRS
        MainWindow.h        
        AcquisitionWidget.h
        DVRWidget.h
        MPRWidget.h
        PoseViewer.h
        PoseDisplay.h
        ReconstructionViewer.h
        ResultWidget.h
        SliceWidget.h
        VisualizationWidget.h
        DVRControlWidget.h
        MPRControlWidget.h
        BoxSlider.h
        TransferFuncWidget.h
        AxisWidget.h
        )

add_library(part2_lib_gui ${GUI_HDRS} ${GUI_SRCS} )
target_link_libraries(part2_lib_gui part2_lib_vis Qt5::Widgets)
