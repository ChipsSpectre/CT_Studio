#include "catch.hpp"

#include <cmath>
#include <iostream>

#include <Eigen/Eigen>

#include "DVRCamera.h"
#include "DVRModel.h"
#include "DVRWidget.h"
#include "MIP.h"
#include "TransferFunction.h"
#include "VisualizationModel.h"
#include "Volume.h"

using M=Eigen::VectorXf;

/**
 * use catch testing for local verification of results.
 * @brief TEST_CASE
 */
TEST_CASE("test dvr widget painting")
{
    int n = 2;
    M m(n*n*n);
    for(int i = 0; i<n; ++i)
    {
        for(int j = 0; j<n; ++j)
        {
            for(int k = 0; k<n; ++k)
            {
                m(i*n*n+j*n+k) = (i+j+k)/8.0;
            }
        }
    }

    Eigen::Vector3f lowerLeft(0, 0, 0);
    Eigen::Vector3f upperRight(2, 2, 2);
    Eigen::Vector3f dimSpacing(1, 1, 1);

    Volume vol(lowerLeft, upperRight, dimSpacing, std::move(m));


    SECTION("test camera calibration")
    {
        DVRCamera camera;
        for(int i = 0; i<=360; i+=15)
        {
            float radian = (i/180.0)*M_PI;
            Eigen::Vector3f c = camera.calculateCameraPosition(vol, radian);
            std::cout << c(0) << "," << c(1) << std::endl;
        }
    }

}
